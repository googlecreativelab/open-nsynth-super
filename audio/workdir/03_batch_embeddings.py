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


import os, json

with open('../settings.json', 'r') as infile:
	settings = json.load(infile)

workdir = os.getcwd()
num_embeddings = len(os.listdir('embeddings_output'))
batch_size = num_embeddings / settings['gpus']

#	split the embeddings per gpu in folders
for i in range(0, settings['gpus']):
	foldername = 'embeddings_batched/batch%i' % i
	if not os.path.exists(foldername):
		os.mkdir(foldername)
	output_foldername = 'audio_output/batch%i' % i
	if not os.path.exists(output_foldername):
		os.mkdir(output_foldername)

#	shuffle to the folders
batch = 0
for filename in os.listdir('embeddings_output'):
	target_folder = workdir + '/embeddings_batched/batch%i/' % batch
	batch += 1
	if batch >= settings['gpus']:
		batch = 0
	os.rename(workdir + '/embeddings_output/' + filename, target_folder + filename)


