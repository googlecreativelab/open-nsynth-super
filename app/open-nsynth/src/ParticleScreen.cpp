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

#include "ParticleScreen.h"


ParticleScreen::ParticleScreen(){
	// Set up the initial particle positions.
	int x, y;
	int idx = 0;

	for(y=0; y<11; ++y){
		for(x=0; x<11; ++x){
			origin[idx] = modelToScreen(x, y);
			position[idx] = origin[idx];
			velocity[idx] = {0, 0};
			++idx;
		}
	}

	// Create the x+y cursor from this x offset pattern.
	static float xOffset[CURSOR_SIZE][CURSOR_SIZE] = {
		0, 2, 0, -2, 0,
		3, 3, 0, -3, -3,
		2, 2, 0, -2, -2,
		3, 3, 0, -3, -3,
		0, 2, 0, -2, 0
	};

	for(y=0; y<CURSOR_SIZE; ++y){
		for(x=0; x<CURSOR_SIZE; ++x){
			cursorShape[x][y] = {xOffset[y][x], xOffset[x][y]};
		}
	}

	font.load("NSSThin.ttf", 12, false, true, false, 0.3, 96);
}


ofVec2f ParticleScreen::modelToScreen(float x, float y) const{
	float sx = 32;
	float sy = 0;
	float ox = sx + 7;
	float oy = sy + 7;
	return {ox + x*5, oy + y*5};
}


std::array<int, 2> ParticleScreen::screenToModel(float x, float y) const{
	float sx = 32;
	float sy = 0;
	float ox = sx + 7;
	float oy = sy + 7;
	return {
		static_cast<int>(round(((x - ox)/5.0))),
			static_cast<int>(round((y - oy)/5.0))
	};
}


void ParticleScreen::setInstrument(int idx, const std::string &name){
	if(idx >= 0 && idx < NUM_INSTRUMENTS){
		instruments[idx] = name;
	}
}


void ParticleScreen::updateParticles(ofVec2f selection, float elapsed){
	int idx;
	ofVec2f mousePos = modelToScreen(selection.x, selection.y);


	for(idx=0; idx<NUM_PARTICLES; ++idx){
		// Calculate where this particle is according to the cursor.
		ofVec2f cursorIdx = (origin[idx] - mousePos) / CURSOR_SIZE;
		int cx = int(cursorIdx.x) + 2;
		int cy = int(cursorIdx.y) + 2;

		// Calculate the target position for this particle.
		ofVec2f target = origin[idx];
		if(cx >= 0 && cx < CURSOR_SIZE && cy >= 0 && cy < CURSOR_SIZE){
			// This particle is affected by the cursor.
			target += cursorShape[cx][cy];
		}

		// Update the velocity to move the particle towards the target.
		velocity[idx] *= 0.65;
		velocity[idx] += (target - position[idx]) * 10.0;

		// Move the particle according to the velocity.
		position[idx] += velocity[idx] * elapsed;
	}
}


void ParticleScreen::draw(float elapsed){
	ofSetColor(255, 255, 255);

	ofDrawLine(32, 0, 32, SCREEN_HEIGHT);
	ofDrawLine(SCREEN_WIDTH-31, 0, SCREEN_WIDTH-31, SCREEN_HEIGHT);

	int fontHeight = font.getSize();

	font.drawString(instruments[0], 0, fontHeight);
	int w = static_cast<int>(font.stringWidth(instruments[1]));
	font.drawString(instruments[1], SCREEN_WIDTH-w, fontHeight);
	font.drawString(instruments[2], 0, SCREEN_HEIGHT);
	w = static_cast<int>(font.stringWidth(instruments[1]));
	font.drawString(instruments[3], SCREEN_WIDTH-w, SCREEN_HEIGHT);

	for(int idx=0; idx<NUM_PARTICLES; ++idx){
		ofDrawRectangle(position[idx], 1, 1);
	}
}
