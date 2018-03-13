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
from itertools import product
from os.path import basename
from utils import *

#	load the settings file
settings = None
with open('../settings.json', 'r') as infile:
	settings = json.load(infile) 

#	constants and rearrangement of settings vars for processing
pitches = settings['pitches']
resolution = settings['resolution']
final_length = settings['final_length']
instrument_groups = [settings['pads']['NW'], settings['pads']['NE'], settings['pads']['SE'], settings['pads']['SW']]
combinations = sorted(product(*instrument_groups))
xy_grid = make_grid(resolution)


#	cache all embeddings
embeddings_lookup = {}

for filename in os.listdir('embeddings_input/'):
	# ignore all non-npy files
	if '.npy' in  filename:
		#	convert filename to reference key
		parts = basename(filename).split('_')
		reference = '{}_{}'.format(parts[0], parts[1])

		#	load the saved embedding
		embeddings_lookup[reference] = np.load("embeddings_input/%s" % filename)

def get_embedding(instrument, pitch):
	reference = '{}_{}'.format(instrument, pitch)
	return embeddings_lookup[reference]


done = set()
all_names = []
all_embeddings = []

for combination in tqdm(combinations):
	for pitch in  pitches:
		embeddings = np.asarray([get_embedding(instrument, pitch) for instrument in combination])
		
		for xy in xy_grid:
			weights = get_weights(xy)
			interpolated = (embeddings.T * weights).T.sum(axis=0)

			#	avoid repetition
			meta = get_description(combination, weights, pitch)
			if meta in done:
				continue
			done.add(meta)

			name = description_to_name(meta)

			#	reshape array
			interpolated = np.reshape(interpolated, (1,) + interpolated.shape)

			np.save('embeddings_output/' + name + '.npy', interpolated.astype(np.float32))
