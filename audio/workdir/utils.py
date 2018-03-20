# Copyright 2017 Google Inc

# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at

#     https://www.apache.org/licenses/LICENSE-2.0

# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import os
import numpy as np

# extensions can be a single tring like '.png' or '.jpg'
# or a list of extensions. they should all be lowercase
# but the . is important.
def list_all_files(directory, extensions=None):
    for root, dirnames, filenames in os.walk(directory):
        for filename in filenames:
            base, ext = os.path.splitext(filename)
            joined = os.path.join(root, filename)
            if extensions is None or ( len(ext) and ext.lower() in extensions ):
                yield joined


def make_grid(res):
	x, y = np.meshgrid(np.linspace(0, 1, res), np.linspace(0, 1, res))
	x = x.reshape(-1)
	y = y.reshape(-1)
	return zip(x, y)

def get_weights(xy):
	corners = np.array([[0,0], [1,0], [0,1], [1,1]])
	distances = np.linalg.norm(xy - corners, axis=1)
	distances = np.maximum(1 - distances, 0)
	distances /= distances.sum()
	return distances


class hashabledict(dict):
    def __hash__(self):
        return hash(tuple(sorted(self.items())))

	
def get_description(combination, weights, pitch):
	meta = {'pitch': pitch}
	for i, w in zip(combination, weights):
		if w > 0:
			meta[i] = np.around(w, decimals=3)
	return hashabledict(meta)


def description_to_name(meta):
	attr = []
	for key in sorted(meta.keys()):
		attr.append(key)
		attr.append(meta[key])
	return '_'.join(map(str, attr))



def name_to_description(name):
	parts = name.split('_')
	pairs = zip(parts[::2], parts[1::2])
	meta = {key: float(value) for key, value in pairs if float(value) > 0}
	meta['pitch'] = int(meta['pitch'])
	return hashabledict(meta)

def get_filename(name):
	fname = 'audio_output/output_wav/gen_{}.wav'.format(name)
	return fname



# susceptible to false onsets (brief burst before the real onset)
def find_onset(audio, threshold_pct=0.1):
    abs_audio = np.abs(audio)
    threshold = threshold_pct * abs_audio.max()
    onset = np.argmax(abs_audio > threshold)
    return onset

def silence_ending(audio, max_value=3):
    abs_audio = np.abs(audio)
    offset = np.argmax(abs_audio[::-1] > max_value)
    audio[-(1+offset):] = 0

def fade_in(audio, samples=8):
    audio[:samples] *= np.linspace(0, 1, samples)

def fade_out(audio, samples=8):
    audio[-samples:] *= np.linspace(1, 0, samples)

def post_process(audio, sr, onset_search=0.25, walk_onset_back=16, final_length=60000):
    x = np.copy(audio).astype(np.float32)
    onset_search_end = int(onset_search * sr)
    onset = find_onset(x[:onset_search_end])
    onset -= walk_onset_back
    onset = max(onset, 0)
    pre_xlen = len(x)
    x = x[onset:] # crop starting at onset
    x = x[:final_length] # crop to final length
    
    # fix erroneously shortened samples
    if len(x) != 60000:
            padding_amount = final_length - len(x)
	    x = np.append(x, np.zeros(padding_amount))
        
    # silence_ending(x)
    fade_in(x)
    fade_out(x)
    return x.astype(audio.dtype), onset

