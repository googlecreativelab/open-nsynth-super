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

#include <functional>

#include "ofMain.h"
#include "BaseScreen.h"


// Displays patch saving and loading.
class PatchScreen : public BaseScreen{
	public:
		enum State{
			IDLE,
			INTERACTION_STARTED,
			PATCH_LOADED,
			PATCH_SAVE_CANCELLED,
			PATCH_SAVED
		};

		static constexpr float LOAD_PATCH_SCREEN_TIMEOUT = 0.750f;
		static constexpr float SAVE_PATCH_SCREEN_TIMEOUT = 0.250f;

		static constexpr float LOAD_TIME = 0.250f;
		static constexpr float SAVE_COMPLETE_TIME = 0.750f;
		static constexpr float SAVE_FLASH_TIME = 0.100f;

		PatchScreen();

		// Handles button presses.
		void handleInput(int idx, bool pressed);
		// Cancels any interactions.
		void cancelInput();
		// Updates the state, should be called before draw().
		void update(float elapsed);
		// Gets the patch being loaded/saved.
		int getPatchIndex(){ return displayIdx; };

		void draw(float elapsed) override;

		std::function<void(State)> onStateChange = 0;

	private:
		// Changes the state and calls onStateChange.
		void setState(State newState);

		State state = IDLE;
		int currentIdx = -1;
		int displayIdx = -1;
		float totalElapsed = -1.0f;

		// The font to use for displaying names.
		ofTrueTypeFont font;
		// A frame buffer object used during drawing.
		ofFbo fbo;
};
