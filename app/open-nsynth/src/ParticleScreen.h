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
#include "BaseScreen.h"


// Displays the four instrument selections along side the interpolation
// position as an animated grid of dots.
class ParticleScreen : public BaseScreen{
	public:
		ParticleScreen();

		// Converts touch (0-10) co-ordinates to screen co-ordinates.
		ofVec2f modelToScreen(float x, float y) const;
		// Converts screen co-ordinates to touch (0-10) co-ordinates.
		std::array<int, 2> screenToModel(float x, float y) const;

		// Sets the instrument name to be displayed in one of the 4 corners.
		void setInstrument(int idx, const std::string &name);
		// Animates the particles according to the current touch selection.
		void updateParticles(ofVec2f selection, float elapsed);
		void draw(float elapsed) override;

	private:
		static constexpr int NUM_PARTICLES = 11 * 11;
		// The starting/resting position of the particles.
		ofVec2f origin[NUM_PARTICLES];
		// The current position of the particles.
		ofVec2f position[NUM_PARTICLES];
		// The current velocity of the particles.
		ofVec2f velocity[NUM_PARTICLES];

		static constexpr int CURSOR_SIZE = 5;
		// The x+y offsets for particles under the cursor.
		ofVec2f cursorShape[CURSOR_SIZE][CURSOR_SIZE];

		static constexpr int NUM_INSTRUMENTS = 4;
		// The four instrument names.
		std::string instruments[NUM_INSTRUMENTS];
		// The font to use for displaying names.
		ofTrueTypeFont font;
};
