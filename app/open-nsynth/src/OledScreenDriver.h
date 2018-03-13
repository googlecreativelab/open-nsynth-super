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

#include "ofMain.h"


// Drives the SSD1306 based screen over I2C.
class OledScreenDriver{
	public:
		// Sends the initial configuration to the screen.
		bool setup(int i2cFd, uint8_t address, int resetPin=-1);
		// Draws a 128x64 frame buffer on the screen.
		void draw(ofFbo &fbo);

	private:
		// Resets the screen by driving a GPIO reset pin low.
		void reset(int resetPin);
		// The file descriptor of the I2C device.
		int i2cFd;
		// The I2C address of the screen.
		uint8_t address;
};
