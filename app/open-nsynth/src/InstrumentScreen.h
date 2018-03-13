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


// Displays an intrument selection screen.
class InstrumentScreen : public BaseScreen{
	public:
		// Keeps information on a single instrument.
		struct Instrument{
			std::string name;
			std::string display;
			std::string abbr;
		};

		InstrumentScreen();

		// Changes the current selection by the given amount.
		// The instrument list is circular - wraps in both directions.
		void update(int amt);
		void draw(float elapsed) override;
		const Instrument &getCurrent() const;

		// The collection of instrument options.
		std::vector<Instrument> instruments;
		// The font to use for displaying names.
		ofTrueTypeFont font;
		// A frame buffer object used during drawing.
		ofFbo fbo;
		// The index of the current selection.
		int idx = 0;
		// The direction to scroll in, determined by the update amount.
		int direction = 1;
		// The current y position.
		float y = 0;
		// The amount y is being changed, used for smooth scrolling.
		float velocity = 0;
};
