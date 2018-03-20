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

#include "InstrumentScreen.h"


InstrumentScreen::InstrumentScreen(){
	font.load("NSSMedium.ttf", 12, false, true, false, 0.3, 96);
	fbo.allocate(128, 64, GL_RGBA);
}


void InstrumentScreen::update(int amt){
	idx = (idx + amt) % instruments.size();
	direction = amt > 0 ? 1 : -1;
}


void InstrumentScreen::draw(float elapsed){
	constexpr int CENTER = SCREEN_CENTER_Y;
	constexpr int LINE_HEIGHT = 21;
	constexpr int CENTER_LINE = CENTER + 5;

	// Work out the target y position.
	int target = idx * LINE_HEIGHT;
	int listHeight = instruments.size() * LINE_HEIGHT;
	if (direction == 1 && target < y){
		target += listHeight;
	}else if (direction == -1 && target > y){
		target -= listHeight;
	}

	// Update velocity to travel towards the target.
	velocity *= 0.45;
	velocity += (target - y) * 5.0;
	// Update the y position by the velocity, but do not overshoot the target.
	if(abs(velocity) < 1.0){
		velocity = 0.0;
		y = target;
	}else if(direction == 1){
		y = std::min(y + velocity*elapsed, float(target));
	}else{
		y = std::max(y + velocity*elapsed, float(target));
	}

	// Keep the position between 0 and listHeight.
	y = fmod(y, listHeight);
	if(y < 0){
		y += listHeight;
	}

	// Draw the lines of text to an fbo.
	fbo.begin();
	ofClear(0, 0, 0, 0);
	for(int i=1; i<5; ++i){
		float pos = y - CENTER_LINE + i*LINE_HEIGHT;
		int loopIdx = static_cast<int>(floor(pos / LINE_HEIGHT));
		int off = static_cast<int>(pos - (loopIdx * LINE_HEIGHT));
		Instrument& instrument = instruments[loopIdx % instruments.size()];

		int w = static_cast<int>(font.stringWidth(instrument.display));
		font.drawString(instrument.display, (128 - w) / 2, i*LINE_HEIGHT - off);
	}
	fbo.end();

	// Invert the center section.
	ofPixels pixels;
	fbo.readToPixels(pixels);

	for(int py=CENTER - LINE_HEIGHT/2; py<CENTER+LINE_HEIGHT/2; ++py){
		int pidx = py * 128 * 4;
		for(int px=0; px<128; ++px){
			pixels[pidx] = 255 - pixels[pidx];
			++pidx;
			pixels[pidx] = 255 - pixels[pidx];
			++pidx;
			pixels[pidx] = 255 - pixels[pidx];
			++pidx;
			pixels[pidx] = 255;
			++pidx;
		}
	}

	ofImage image;
	image.setFromPixels(pixels);

	image.draw(0, 0);
}


const InstrumentScreen::Instrument &InstrumentScreen::getCurrent() const{
	return instruments[idx];
}
