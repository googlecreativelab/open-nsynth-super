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

#include <stdlib.h>

#include "AnalogInput.h"


void AnalogInput::init(uint8_t value){
	readValue = value;
	displayedValue = value;
}


bool AnalogInput::update(uint8_t value, bool software, bool currentScreen){
	bool changed;

	if(software){
		changed = value != displayedValue;
	}else{
		int delta = abs(int(value) - int(readValue));
		if(currentScreen){
			changed = delta > CURRENT_SCREEN_DELTA;
		}else{
			changed = delta > HIDDEN_SCREEN_DELTA;
		}
		readValue = value;
	}

	if(changed){
		displayedValue = value;
	}

	return changed;
}


float AnalogInput::getNormalized() const{
	return static_cast<float>(displayedValue) / 255.0;
}
