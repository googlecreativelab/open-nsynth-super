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
import re
import librosa
import scipy.io
import numpy as np
import sys
from tqdm import tqdm

if len(sys.argv)<=1:
        print "Specify batch number"
        sys.exit(1)

# original_path = os.path.dirname(os.path.realpath(__file__))
original_path = os.path.join(os.curdir, 'audio_output/raw_wav/batch%s' % sys.argv[1])
cleaned_path = os.path.join(os.curdir, 'audio_output/cleaned_mp3')

if not os.path.exists(original_path):
        print("No input directory found")
        sys.exit(1)

if not os.path.exists(cleaned_path):
        os.mkdir(cleaned_path)


files = os.listdir(original_path)
files = [f for f in files if '.wav' in f]


print ("Normalising input WAVs")
for fpath in tqdm(files):
  audio, sr = librosa.core.load(os.path.join(original_path, fpath), sr=16000)
  d = audio[1:] - audio[:-1]
  d_thresh = np.where(np.abs(d) > 1.0)[0]
  clicks = [
      c for i, c in enumerate(d_thresh[:-1])
      if d_thresh[i] + 1 == d_thresh[i + 1]
  ]
  for click in clicks:
    audio[click + 1] = (audio[click] + audio[click + 2]) / 2.0

  # Replace %03d filenumber with with %04d
  r = re.compile('v1_([0-9]{3})_')
  if r.findall(fpath):
    n = int(r.findall(fpath)[0])
    new_fpath = r.sub('v1_{:04d}_'.format(n), fpath)
  else:
    new_fpath = fpath

  temp_file = os.path.join(cleaned_path, 'cleaned_' + new_fpath)
  with open(temp_file, 'w') as f:
    data_16bit = audio * 2**15
    scipy.io.wavfile.write(f, 16000, data_16bit.astype(np.int16))

  #Normalize audio level
  cleaned_file = os.path.join(cleaned_path, new_fpath)
  sox_cmd = "sox --norm=-12 '{}' '{}'".format(temp_file, cleaned_file)
  os.system(sox_cmd)

  # Convert to mp3
  os.system("lame --quiet '{}'".format(cleaned_file))

  # Cleanup - remove the wavs having converted to mp3
  os.remove(temp_file)
  os.remove(cleaned_file)

#       convert back to wav
if not os.path.exists('audio_output/output_wav/'):
        os.mkdir('audio_output/output_wav')

print ("Converting to WAV")
for fname in tqdm(os.listdir('audio_output/cleaned_mp3')):
        if '.mp3' in fname:
                new_fname = fname.replace('.mp3', '.wav')
                os.system('sox audio_output/cleaned_mp3/%s audio_output/output_wav/%s' % (fname, new_fname))