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

#include "NSynth.h"

#include "MidiUtils.h"
#include <fstream>
#include <sstream>
#include <chrono>
#include <cmath>

void NSynth::setup(int resolution, const std::vector<int>& pitches, int length, int sampleRate) {
    this->resolution = resolution;
    this->pitches = pitches;
    int rows = resolution * resolution * pitches.size();
    std::cout << "[NSynth] setup: " << rows << "x" << length << std::endl;
    Multisampler::setup(rows, length, sampleRate);
}

bool NSynth::load(std::string filename) {
    if(filename == current_filename) {
        std::cout << "[NSynth] already loaded" << std::endl;
        return true;
    }
    
    clear();
    current_filename = "";
    
    auto start = get_time_ms();
    if(sources.load(filename + ".bin", rows(), cols()) != 0) {
        return false;
    }
    auto stop = get_time_ms();
    std::cout << "[NSynth] load time: " << (stop - start) << "ms" << std::endl;
    
    base_lookup = buildMultisampleLookup(pitches);
    set_interpolation_position(x, y);
    current_filename = filename;
    return true;
}

void NSynth::set_interpolation_position(float x, float y) {
    this->x = x;
    this->y = y;
    
    int xi = (int) round(x * (resolution - 1));
    int yi = (int) round(y * (resolution - 1));
    int index = yi * resolution + xi;
    int row_offset = index * pitches.size();
    
    lookup = base_lookup;
    for(auto& row_rate : lookup) {
        row_rate.first += row_offset;
    }
}
