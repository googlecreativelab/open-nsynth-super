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

import json
import numpy as np
from tqdm import tqdm
from utils import *
from itertools import product
from scipy.io import wavfile

#       load the settings file
settings = None
with open('../settings.json', 'r') as infile:
        settings = json.load(infile)

#       constants and rearrangement of settings vars for processing
pitches = settings['pitches']
final_length = settings['final_length']
resolution = settings['resolution']
instrument_groups = [settings['pads']['NW'], settings['pads']['NE'], settings['pads']['SE'], settings['pads']['SW']]
combinations = sorted(product(*instrument_groups))
xy_grid = make_grid(resolution)


'''
	TODO - total rewrite of this to support non-monolithic npy
'''

names = []
for combination in combinations:
	for pitch in pitches:
		for xy in xy_grid:
			weights = get_weights(xy)
			meta = get_description(combination, weights, pitch)
			names.append(description_to_name(meta))





meta_to_filename = {}
for name in names:
	meta = name_to_description(name)
	meta_to_filename[meta] = get_filename(name)


err_rate = 0
total = 0
error_files = []

for combination in tqdm(combinations):
	pad = []
	i = 0

	for xy in xy_grid:
		weights = get_weights(xy)
		weights =  np.around(weights, decimals = 3)

		for pitch in pitches:
			meta = get_description(combination, weights, pitch)
			filename = meta_to_filename[meta]

			sr, audio = wavfile.read(filename)
			audio, onset = post_process(audio, sr, final_length)

			if len(audio) != 60000:
				error_files.append(filename)

			pad.append(audio)

	combination_name = '_'.join(combination)

	pad_out = np.asarray(pad)
	pad_out.tofile("pads_output/{}.bin".format(combination_name))