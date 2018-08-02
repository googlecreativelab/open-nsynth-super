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

#include <stdint.h>


// Keeps track of the input from a potentiometer.
class AnalogInput{
	public:
		// Sets the initial value.
		void init(uint8_t value);

		// Updates the value.
		// Returns true if the value has changed more than a threshold.
		bool update(uint8_t value, bool software, bool currentScreen=false);

		// Returns the displayedValue as a float in the range 0.0 - 1.0.
		float getNormalized() const;

		static constexpr int CURRENT_SCREEN_DELTA = 2;
		static constexpr int HIDDEN_SCREEN_DELTA = 5;

		// The value read from the hardware.
		uint8_t readValue = 0x7f;
		// The (smoothed) value to use for display purposes.
		uint8_t displayedValue = 0x7f;
};
