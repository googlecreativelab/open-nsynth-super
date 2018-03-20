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

#include "EnvelopeScreen.h"


void EnvelopeScreen::draw(float elapsed) {
	constexpr int TOP = SCREEN_CENTER_Y - 16;
	constexpr int BOTTOM = SCREEN_CENTER_Y + 16;
	constexpr int LEFT = 16;
	constexpr int RIGHT = SCREEN_WIDTH - LEFT;
	constexpr int SEG_WIDTH = 23;
	constexpr int WIDE_LINE = 3;
	constexpr int THIN_LINE = 1;
	constexpr int CAP_RADIUS = 1;

	ofSetColor(255, 255, 255);

	int x = LEFT;

	int attackWidth = static_cast<int>(attack * SEG_WIDTH) + 1;
	int decayWidth = static_cast<int>(decay * SEG_WIDTH) + 1;
	int sustainY = BOTTOM - static_cast<int>((BOTTOM - TOP) * sustain);
	int releaseWidth = static_cast<int>(release * SEG_WIDTH) + 1;

	// Draws a line, making it thicker if it is the current stage.
	auto drawLine = [&](int s, int x0, int y0, int x1, int y1){
		if(s == stage){
			ofSetLineWidth(WIDE_LINE);
			ofDrawCircle(x0, y0, CAP_RADIUS);
			ofDrawCircle(x1, y1, CAP_RADIUS);
		}else{
			ofSetLineWidth(THIN_LINE);
		}
		ofDrawLine(x0, y0, x1, y1);
	};

	drawLine(0, x, BOTTOM, x+attackWidth, TOP);
	x += attackWidth;

	drawLine(1, x, TOP, x+decayWidth, sustainY);
	x += decayWidth;

	drawLine(2, x, sustainY, RIGHT-releaseWidth, sustainY);
	x = RIGHT-releaseWidth;

	drawLine(3, x, sustainY, x+releaseWidth, BOTTOM);

	ofSetLineWidth(1);
}
