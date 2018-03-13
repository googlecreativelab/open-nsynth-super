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

#include "PositionScreen.h"


void PositionScreen::draw(float elapsed){
	constexpr int CENTER = SCREEN_CENTER_Y;
	constexpr int LEFT = 16;
	constexpr int RIGHT = SCREEN_WIDTH - LEFT;
	constexpr int WIDE_LINE = 3;
	constexpr int THIN_LINE = 1;

	ofPolyline line;

	// Get the y (pixel) value along the sine wave for a given x (pixel) value.
	auto getY = [&](int x)->int{
		constexpr float offset = M_PI * 7.0 / 8.0;
		constexpr float scale = 0.246;
		return CENTER - static_cast<int>(sin((x+offset) * scale) * 16);
	};

	// Calculate the length of the sine wave.
	float totalLenSq = 0.0;
	float x0 = LEFT;
	float y0 = getY(0);
	for(int i=1; i<RIGHT-LEFT+1; ++i){
		int x1 = LEFT + i;
		int y1 = getY(i);

		totalLenSq += pow((y1 - y0), 2) + pow((x1 - x0), 2);

		x0 = x1;
		y0 = y1;
	}

	// Calculate the length to draw the line thin as a fraction of the total
	// length.
	float thinLen = position * totalLenSq;

	// Draw the sine wave.
	float lenSq = 0.0;
	bool thin = true;
	x0 = LEFT;
	y0 = getY(0);
	ofSetLineWidth(THIN_LINE);
	line.addVertex(x0, y0);
	for(int i=1; i<RIGHT-LEFT+1; ++i){
		int x1 = LEFT + i;
		int y1 = getY(i);

		lenSq += pow((y1 - y0), 2) + pow((x1 - x0), 2);
		if((lenSq > thinLen) && thin){
			line.draw();
			ofSetLineWidth(WIDE_LINE);
			line.clear();
			line.addVertex(x0, y0);
			thin = false;
		}
		line.addVertex(x1, y1);

		x0 = x1;
		y0 = y1;
	}
	line.draw();
	ofSetLineWidth(1);
}
