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

#include "PatchScreen.h"


PatchScreen::PatchScreen(){
	font.load("NSSMedium.ttf", 12, false, true, false, 0.3, 96);
	fbo.allocate(SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGBA);
}


void PatchScreen::handleInput(int idx, bool pressed){
	if(pressed){
		if(idx != currentIdx){
			currentIdx = idx;
			displayIdx = idx;
			totalElapsed = 0.0f;
			state = IDLE;
			setState(INTERACTION_STARTED);
		}
	}else if(idx == currentIdx){
		currentIdx = -1;
		if(state == INTERACTION_STARTED){
			if(totalElapsed <= LOAD_TIME){
				totalElapsed = 0.0f;
				setState(PATCH_LOADED);
			}else{
				setState(PATCH_SAVE_CANCELLED);
			}
		}else if(state == PATCH_SAVED){
			setState(IDLE);
		}
	}
}


void PatchScreen::cancelInput(){
	state = IDLE;
}


void PatchScreen::update(float elapsed){
	if(state != IDLE){
		totalElapsed += elapsed;
	}

	if(state == INTERACTION_STARTED){
		if(totalElapsed>=SAVE_COMPLETE_TIME){
			totalElapsed = 0.0f;
			setState(PATCH_SAVED);
		}
	}else if(state == PATCH_LOADED){
		if(totalElapsed >= LOAD_TIME){
			setState(IDLE);
		}
	}
}


void PatchScreen::draw(float elapsed){
	constexpr int CENTER = SCREEN_CENTER_Y;
	constexpr int LINE_HEIGHT = 21;
	constexpr int CENTER_LINE = CENTER + 5;

	std::string text = "PATCH " + to_string(displayIdx + 1);

	fbo.begin();
	ofClear(0, 0, 0, 0);
	int w = static_cast<int>(font.stringWidth(text));
	font.drawString(text, (SCREEN_WIDTH-w)/2, CENTER_LINE);
	fbo.end();

	// Invert the center section.
	ofPixels pixels;
	fbo.readToPixels(pixels);

	int fillEnd = 0;

	if(state==INTERACTION_STARTED && totalElapsed>=LOAD_TIME){
		fillEnd = static_cast<int>(ofMap(
					totalElapsed, LOAD_TIME, SAVE_COMPLETE_TIME,
					0, SCREEN_WIDTH, true));
	}else if(state==PATCH_SAVED){
		// Blink
		fillEnd = totalElapsed > SAVE_FLASH_TIME ? SCREEN_WIDTH : 0;
	}

	for(int py=CENTER - LINE_HEIGHT/2; py<CENTER+LINE_HEIGHT/2; ++py){
		int pidx = py * SCREEN_WIDTH * 4;
		for(int px=0; px<fillEnd; ++px){
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


void PatchScreen::setState(PatchScreen::State newState){
	if(newState!=state){
		state = newState;
		if(onStateChange){
			onStateChange(state);
		}
	}
}
