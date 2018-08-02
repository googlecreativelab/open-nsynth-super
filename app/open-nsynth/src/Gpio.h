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

#include <cstddef>
#include <stdint.h>


// Provides access to the GPIO pins on a Raspberry Pi
class Gpio{
	public:
		enum Pull{
			PULL_NONE=0,
			PULL_DOWN,
			PULL_UP
		};

		Gpio();
		~Gpio();

		operator bool() const;

		bool setInput(int pin, Pull pull=PULL_NONE);
		bool setOutput(int pin);

		bool write(int pin, bool high);
		bool read(int pin);

		// Sends a reset signal by pulsing a pin low.
		void reset(int pin, size_t uSecHold, size_t uSecDelay);

	protected:
		volatile uint32_t *gpioReg = nullptr;
};
