/*
Copyright 2017 Google Inc. All Rights Reserved.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
    http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#pragma once

#include <array>
#include <string>
#include <ofJson.h>


// Encapsulates the synth settings.
class Patch{
	public:
		float position = 0.0f;
		float attack = 1.0f;
		float decay = 1.0f;
		float sustain = 1.0f;
		float release = 1.0f;

		std::array<std::string, 4> instruments;

		std::array<float, 2> grid = {{0.5f, 0.5f}};

		ofJson toJson();
		bool fromJson(const ofJson &node);
};


// Saves and loads patches to a JSON file.
class Patches : public std::array<Patch, 4>{
	public:
		std::string filename;

		bool load();
		bool save();
};
