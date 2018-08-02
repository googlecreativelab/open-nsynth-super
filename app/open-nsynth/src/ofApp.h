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

#include <stdint.h>

#if OF_VERSION_MINOR < 10
#include "ofJson.h"
#endif

#include "ofMain.h"
#include "ofxOsc.h"
#include "AnalogInput.h"
#include "EnvelopeScreen.h"
#include "Gpio.h"
#include "InstrumentScreen.h"
#include "MidiThread.h"
#include "OledScreenDriver.h"
#include "ParticleScreen.h"
#include "Patch.h"
#include "PatchScreen.h"
#include "PositionScreen.h"
#include "VolumeScreen.h"
#include "NSynth.h"


class ofApp : public ofBaseApp{
	public:
		ofApp();

		void setup() override;
		// Sets up the instrument selection in each corner.
		void setupCorners();
		// Sets up hardware inputs.
		void setupHardwareInputs();
		// Sets up ALSA sound output.
		void setupSound();
		// Sets up the NSynth instance.
		void setupSynth();
		void update() override;
		// Reads hardware inputs from the MCU.
		void readInputs();
		void draw() override;
		// Called to change the interpolation position.
		void updateGridSelection(std::array<int, 2> selection);
		// Called to update one of the instrument selections.
		void updateRotary(int idx, int amount);
		// Called to update one of the potentiometers.
		void updateAnalogInput(int idx, uint8_t value, bool software);
		// Called to update a patch button.
		void updatePatchInput(int idx, bool pressed);
		// Called to handle the patch state.
		void handlePatchState(PatchScreen::State state);
		// Changes the current screen.
		void setScreen(BaseScreen *screen);
		// Loads the pad according to the 4 selected instruments.
		void loadPad();

		// Loads one of the saved patches.
		bool loadPatch(size_t patchIdx);
		// Saves the current settings to a patch.
		bool savePatch(size_t patchIdx);

		void keyPressed(int key) override;
		void keyReleased(int key) override;
		void mouseMoved(int x, int y ) override;
		void mouseDragged(int x, int y, int button) override;
		void mousePressed(int x, int y, int button) override;
		void mouseReleased(int x, int y, int button) override;
		void mouseEntered(int x, int y) override;
		void mouseExited(int x, int y) override;
		void windowResized(int w, int h) override;
		void dragEvent(ofDragInfo dragInfo) override;
		void gotMessage(ofMessage msg) override;
		void audioOut(ofSoundBuffer& buffer) override;

	private:
		static constexpr int OLED_I2C_ADDR = 0x3d;
		static constexpr int MCU_I2C_ADDR = 0x47;
		static constexpr int OLED_BCM_RESET_PIN = 4;
		static constexpr int GRID_SIZE = 11;
		static constexpr float SCREEN_TIMEOUT = 1.0;
		static const constexpr std::array<int, 4> PATCH_BCM_PINS = {{5, 6, 13, 26}};

		// The interpolation position.
		std::array<int, 2> gridSelection;
		// The 6 potentiometer values.
		enum {
			POSITION,
			ATTACK,
			DECAY,
			SUSTAIN,
			RELEASE,
			VOLUME,
			ANALOG_INPUT_MAX
		};
		AnalogInput analogInputs[ANALOG_INPUT_MAX];

		// The home screen.
		ParticleScreen particleScreen;
		// The instrument selection screens.
		InstrumentScreen instrumentScreens[4];
		// The position adjustment screen.
		PositionScreen positionScreen;
		// The sound envelope adjustment screen.
		EnvelopeScreen envelopeScreen;
		// The volume adjustment screen.
		VolumeScreen volumeScreen;
		// The volume adjustment screen.
		PatchScreen patchScreen;

		// The active screen.
		BaseScreen *currentScreen;
		// The number of seconds before the home screen is shown.
		float currentScreenTimeout;

		// The I2C device file descriptor.
		int i2c;
		// The offscreen buffer used for rendering.
		ofFbo fbo;
		// The driver used to send the fbo to the OLED screen.
		OledScreenDriver oledScreenDriver;

		// Describes the format of data read from the MCU.
		struct InputsMessage{
			uint8_t touch[2];
			int8_t rotaries[4];
			uint8_t potentiometers[6];
			uint32_t chk;
		};

		// The last InputsMessage read from the MCU, used for comparison.
		InputsMessage lastInputsMessage;
		// True if the inputs from the MCU have been successfully read before.
		bool inputsRead;

		// A thread to handle MIDI data.
		MidiThread midiThread;

		// Settings read from localSettings.json, or from settings.json.
		ofJson settings;
		// The configured sample rate for audio.
		int sampleRate;
		// A buffer used for transferring audio from NSynth to the sound card.
		vector<float> monoBuffer;
		// The configured sound output.
		ofSoundStream soundStream;
		// A lock to guard multithreaded use of synth.
		std::mutex synthMutex;
		// The NSyth instance.
		NSynth synth;
		// True if NSynth has a valid pad loaded.
		bool synthLoaded = false;

		// Keyboard handling.
		static constexpr const char *keyNotes = "qwertyuiop";
		bool keyDown[10] = {false};

		// Patch handling.
		static std::vector<int> patchKeys;
		Patches patches;

		// A handler for incoming Open Sound Control messages.
		ofxOscReceiver oscIn;

		// Used to access the GPIO pins.
		Gpio gpio;
};
