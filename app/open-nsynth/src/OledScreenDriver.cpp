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

#include "OledScreenDriver.h"

#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>


bool OledScreenDriver::setup(int i2cFd_, uint8_t address_){
	i2cFd = i2cFd_;
	address = address_;

	if(i2cFd < 0){
		return false;
	}
	if(ioctl(i2cFd, I2C_SLAVE, address) < 0){
		return false;
	}

	// Send the screen setup commands.
	static uint8_t setup0[] = {
		0, 174, 213, 128, 168, 63, 211, 0, 64, 141, 20, 32,
		0, 161, 200, 218, 18, 217, 241, 219, 64, 164, 166
	};
	static uint8_t setup1[] = {
		0, 129, 207
	};
	static uint8_t setup2[] = {
		0, 33, 0, 127, 34, 0, 7
	};

	if(write(i2cFd, setup0, sizeof(setup0)) != sizeof(setup0)){
		return false;
	}
	if(write(i2cFd, setup1, sizeof(setup1)) != sizeof(setup1)){
		return false;
	}
	if(write(i2cFd, setup2, sizeof(setup2)) != sizeof(setup2)){
		return false;
	}

	return true;
}


void OledScreenDriver::draw(ofFbo &fbo){
	if(i2cFd < 0){
		return;
	}
	if(ioctl(i2cFd, I2C_SLAVE, address) < 0){
		return;
	}

	ofPixels pixels;
	fbo.readToPixels(pixels);

	uint8_t buf[129];

	// Write data command.
	buf[0] = 64;

	int pixelIdx = 0;
	for(int seg=0; seg<8; ++seg){
		// The screen is drawn in 8 horizontal segments.
		// Each segment is 128x8 pixels.
		// Each byte represents one column.

		// Clear the buffer first.
		for(unsigned int x=1; x<sizeof(buf); ++x){
			buf[x] = 0;
		}

		// Traverse the 8 rows.
		for(int y=0; y<8; ++y){
			for(int x=0; x<128; ++x){
				int set = pixels[pixelIdx] >= 127;
				buf[x+1] |= set << y;
				pixelIdx += 4;
			}
		}

		if(write(i2cFd, buf, sizeof(buf)) != sizeof(buf)){
			break;
		}
	}

	// Flush the new data to the screen.
	static uint8_t flush[] = {
		0, 175
	};

	if(write(i2cFd, flush, sizeof(flush)) != sizeof(flush)){
		return;
	}
}
