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


import json, os, subprocess

#	preserve the working directory path
source_dir = os.getcwd()

#	load the settings file
settings = None
with open('../settings.json', 'r') as infile:
	settings = json.load(infile) 

#	launch the process
subprocess.call(["nsynth_save_embeddings", "--checkpoint_path=%s/magenta/models/nsynth/wavenet-ckpt/model.ckpt-200000" % settings['magenta_dir'], "--source_path=%s/audio_input" % source_dir, "--save_path=%s/embeddings_input" % source_dir, "--batch_size=64"])
