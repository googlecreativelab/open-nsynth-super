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

#include <map>
#include <vector>
#include <tuple>
#include <utility>

#include "Multisampler.h"

typedef std::tuple<float,float,float,float> WeightSet;
typedef std::tuple<float,float> WeightPosition;
typedef std::vector<std::pair<int, float>> Lookup;

class NSynth : public Multisampler {
private:
    void update_lookup();
    
    std::string current_filename;
    int resolution;
    std::vector<int> pitches;
    std::vector<std::pair<int, float>> base_lookup;
    float x = 0.5;
    float y = 0.5;
    
public:
    void setup(int resolution, const std::vector<int>& pitches, int length, int sampleRate);
    bool load(std::string filename);
    void set_interpolation_position(float x, float y);
};
