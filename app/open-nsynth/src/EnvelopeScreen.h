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


// Displays the ADSR envelope.
class EnvelopeScreen : public BaseScreen{
	public:
		void draw(float elapsed) override;

		// The stage being highlighted/changed.
		// 0 for attack, 1 for decay, 2 for statain and 3 for release.
		int stage = 0;
		// The four ADSR values all normalised to 0.0 - 1.0.
		float attack = 0.5, decay = 0.5, sustain = 0.5, release = 0.5;
};
