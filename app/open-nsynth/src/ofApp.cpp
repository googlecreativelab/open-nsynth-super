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

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#include "ofApp.h"


template<typename T>
T getDefault(ofJson node, const T& defaultValue){
	if(node.is_null()){
		return defaultValue;
	}
	return node;
}


std::string getDefault(ofJson node, const char *defaultValue){
	return getDefault<std::string>(node, defaultValue);
}


template<typename T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi){
	return std::min(std::max(v, lo), hi);
}


constexpr std::array<int, 4> ofApp::PATCH_BCM_PINS;
std::vector<int> ofApp::patchKeys = {
	OF_KEY_F1, OF_KEY_F2, OF_KEY_F3, OF_KEY_F4};


ofApp::ofApp() : ofBaseApp(), midiThread(synthMutex, synth){
}


void ofApp::setup(){
	settings = ofLoadJson("localSettings.json");
	if(settings.is_null()){
		settings = ofLoadJson("settings.json");
	}

	patchScreen.onStateChange = [this](PatchScreen::State state){
		handlePatchState(state);
	};

	setupCorners();
	setupHardwareInputs();
	setupSound();
	setupSynth();
	loadPad();

	gridSelection = {5, 5};
	updateGridSelection(gridSelection);

	for(int idx=0; idx<4; ++idx){
		particleScreen.setInstrument(
				idx, instrumentScreens[idx].getCurrent().abbr);
	}
	setScreen(&particleScreen);

	patches.filename = getDefault(
			settings["patchFile"], "bin/data/patches.json");
	if(!patches.load()){
		ofLog(OF_LOG_WARNING) << "Failed to patches from " << patches.filename;
	}

	i2c = open("/dev/i2c-1", O_RDWR);
	gpio.reset(OLED_BCM_RESET_PIN, 1000, 1000);
	oledScreenDriver.setup(i2c, OLED_I2C_ADDR);

	inputsRead = false;
	readInputs();

	auto midiDevice = getDefault(settings["midi"]["device"], "/dev/ttyAMA0");
	// Convert from MIDI channel 1-16 to the wire representation
	// 0-15.
	int midiChannel = getDefault(settings["midi"]["channel"], 7) - 1;
	if(midiThread.setup(midiDevice, midiChannel)){
		midiThread.startThread();
	}

	oscIn.setup(getDefault(settings["osc"]["in"]["port"], 8000));

	fbo.allocate(BaseScreen::SCREEN_WIDTH, BaseScreen::SCREEN_HEIGHT, GL_RGBA);
	ofSetFrameRate(30);
}


void ofApp::setupCorners(){
	int idx = 0;
	for(auto corner : settings["corners"]){
		for(auto instrument: corner["instruments"]){
			instrumentScreens[idx].instruments.push_back({
					instrument["name"],
					instrument["display"],
					instrument["abbr"]});
		}
		if(++idx >= 4){
			break;
		}
	}
}


void ofApp::setupHardwareInputs(){
	if(gpio){
		for(auto pin : PATCH_BCM_PINS){
			gpio.setInput(pin, Gpio::PULL_UP);
		}
	}
}


void ofApp::setupSound(){
	auto config = settings["audio"];
	auto devices = ofSoundStreamListDevices();
	sampleRate = settings["audio"]["sampleRate"];
#if OF_VERSION_MINOR < 10
	if(config.find("deviceId") != config.end()){
		soundStream.setDevice(devices[config["deviceId"]]);
	}
	soundStream.setup(this, 2, 0, sampleRate, config["bufferSize"], 0);
#else
	ofSoundStreamSettings sss;
	sss.sampleRate = sampleRate;
	sss.numInputChannels = 0;
	sss.numOutputChannels = 2;
	sss.bufferSize = config["bufferSize"];
	sss.numBuffers = 0;
	sss.setOutListener(this);
	if(config.find("deviceId") != config.end()){
		sss.setOutDevice(devices[config["deviceId"]]);
	}
	soundStream.setup(sss);
#endif
}


void ofApp::setupSynth(){
	auto config = settings["nsynth"];
	synth.setup(
			getDefault(config["resolution"], 9),
			getDefault(config["pitches"], std::vector<int>{
				36, 40, 44, 48, 52, 56, 60, 64, 68, 72, 76, 80, 84}),
			getDefault(config["length"], 60000),
			getDefault(config["sampleRate"], 16000));
	synth.looping = getDefault(config["looping"], synth.looping);
	synth.loopStart = getDefault(config["loopStart"], synth.loopStart);
	synth.loopEnd = getDefault(config["loopEnd"], synth.loopEnd);
}


void ofApp::update(){
	readInputs();

	synthMutex.lock();
	synth.set_position(analogInputs[POSITION].getNormalized());
	synth.set_attack(0.0001 + analogInputs[ATTACK].getNormalized());
	synth.set_decay(analogInputs[DECAY].getNormalized());
	synth.set_sustain(analogInputs[SUSTAIN].getNormalized());
	synth.set_release(analogInputs[RELEASE].getNormalized());
	synth.set_volume(analogInputs[VOLUME].getNormalized());
	synth.set_interpolation_position(
			static_cast<float>(gridSelection[0]) / (GRID_SIZE - 1),
			static_cast<float>(gridSelection[1]) / (GRID_SIZE - 1));
	synthMutex.unlock();

	// The on and off handling is placeholder code. The OSC thread should
	// drive synth directly to avoid the large latency of the rendering loop.
	ofxOscMessage msg;
	while(oscIn.getNextMessage(msg)){
        string addr = msg.getAddress();
		if(addr == "/on"){
			synth.addNoteOn(msg.getArgAsInt(0), msg.getArgAsFloat(1));
		}else if(addr == "/off"){
			synth.addNoteOff(msg.getArgAsInt(0));
		}else if(addr == "/volume"){
			int val = static_cast<int>(msg.getArgAsFloat(0)*255.0f);
			updateAnalogInput(VOLUME, clamp(val, 0, 255), true);
		}else if(addr == "/patch/load"){
			loadPatch(msg.getArgAsInt(0));
		}else if(addr == "/patch/save"){
			savePatch(msg.getArgAsInt(0));
		}
	}
}


void ofApp::readInputs(){
	if(i2c < 0){
		return;
	}

	// Switch to the MCU address.
	if(ioctl(i2c, I2C_SLAVE, MCU_I2C_ADDR) < 0){
		return;
	}

	// Issue an SMBus read from address 0.
	uint8_t buf[1] = {0};
	if(write(i2c, buf, 1) != 1){
		return;
	}

	// Read the whole inputs message from the MCU.
	InputsMessage message;
	if(read(i2c, &message, sizeof(message)) != sizeof(message)){
		return;
	}

	// Check the checksum.
	uint32_t *src = reinterpret_cast<uint32_t *>(&message);
	uint32_t chk = 0xaa55aa55;
	chk += src[0];
	chk += src[1];
	chk += src[2];
	if(chk != message.chk){
		return;
	}

	if(!inputsRead){
		// Set the initial values of the analog inputs.
		for(int i=0; i<ANALOG_INPUT_MAX; ++i){
			analogInputs[i].init(message.potentiometers[i]);
		}
	}else{
		// Update the analog inputs.
		for(int i=0; i<ANALOG_INPUT_MAX; ++i){
			updateAnalogInput(i, message.potentiometers[i], false);
		}

		// Update the instrument selections.
		for(int i=0; i<4; ++i){
			int8_t delta = message.rotaries[i] - lastInputsMessage.rotaries[i];
			updateRotary(i, delta);
		}
	}

	updateGridSelection({message.touch[0], message.touch[1]});

	if(gpio){
		int idx = 0;
		for(auto pin : PATCH_BCM_PINS){
			updatePatchInput(idx++, !gpio.read(pin));
		}
	}

	inputsRead = true;
	lastInputsMessage = message;
}


void ofApp::draw(){
	double elapsed = ofGetLastFrameTime();

	particleScreen.updateParticles(
			{(float)gridSelection[0], (float)gridSelection[1]},
			elapsed);

	patchScreen.update(elapsed);

	// Render into an fbo.
	fbo.begin();
	ofClear(0, 0, 0, 0);
	ofSetColor(255, 255, 255);

	// Switch back to the particle screen if the timeout is reached.
	if(currentScreenTimeout > 0.0){
		currentScreenTimeout -= elapsed;
		if(currentScreenTimeout <= 0.0){
			currentScreen = &particleScreen;
		}
	}
	currentScreen->draw(elapsed);

	fbo.end();

	// Show the FBO for development.
	ofClear(0, 0, 0, 0);
	ofSetColor(255, 255, 255);
	fbo.draw(0, 0);

	// Send the FBO to the OLED screen.
	oledScreenDriver.draw(fbo);
}


void ofApp::updateGridSelection(std::array<int, 2> selection){
	int x = selection[0];
	int y = selection[1];
	if(x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE){
		gridSelection = selection;
		setScreen(&particleScreen);
	}
}


void ofApp::updateRotary(int idx, int amount){
	if(amount){
		instrumentScreens[idx].update(amount);
		loadPad();

		particleScreen.setInstrument(
				idx, instrumentScreens[idx].getCurrent().abbr);
		setScreen(&instrumentScreens[idx]);
	}
}


void ofApp::updateAnalogInput(int idx, uint8_t value, bool software){
	bool changed = analogInputs[idx].update(value, software, true);
	float normalized = analogInputs[idx].getNormalized();

	if(idx == 0){
		positionScreen.position = normalized;
		if(changed){
			setScreen(&positionScreen);
		}
	}else if(idx == 1){
		envelopeScreen.attack = normalized;
		if(changed){
			envelopeScreen.stage = 0;
			setScreen(&envelopeScreen);
		}
	}else if(idx == 2){
		envelopeScreen.decay = normalized;
		if(changed){
			envelopeScreen.stage = 1;
			setScreen(&envelopeScreen);
		}
	}else if(idx == 3){
		envelopeScreen.sustain = normalized;
		if(changed){
			envelopeScreen.stage = 2;
			setScreen(&envelopeScreen);
		}
	}else if(idx == 4){
		envelopeScreen.release = normalized;
		if(changed){
			envelopeScreen.stage = 3;
			setScreen(&envelopeScreen);
		}
	}else if(idx == 5){
		volumeScreen.volume = normalized;
		if(changed){
			setScreen(&volumeScreen);
		}
	}
}


void ofApp::updatePatchInput(int idx, bool pressed){
	if(idx >= 0 && idx < 4){
		patchScreen.handleInput(idx, pressed);
	}
}


void ofApp::handlePatchState(PatchScreen::State state){
	switch(state){
		case PatchScreen::State::IDLE:
			setScreen(&particleScreen);
			break;
		case PatchScreen::State::INTERACTION_STARTED:
			setScreen(&patchScreen);
			break;
		case PatchScreen::State::PATCH_LOADED:
			loadPatch(patchScreen.getPatchIndex());
			break;
		case PatchScreen::State::PATCH_SAVE_CANCELLED:
			setScreen(&particleScreen);
			break;
		case PatchScreen::State::PATCH_SAVED:
			savePatch(patchScreen.getPatchIndex());
			break;
		default:
			break;
	}
}


void ofApp::setScreen(BaseScreen *screen){
	currentScreen = screen;

	if(screen != &patchScreen){
		patchScreen.cancelInput();
	}

	if(screen == &particleScreen || screen == &patchScreen){
		currentScreenTimeout = 0.0;
	}else{
		currentScreenTimeout = SCREEN_TIMEOUT;
	}
}


void ofApp::loadPad(){
	std::vector<std::string> parts;
	for(auto& instrumentScreen : instrumentScreens){
		parts.push_back(instrumentScreen.getCurrent().name);
	}
	std::string padBasename = ofJoinString(parts, "_");
	auto dataDirectory = getDefault(settings["nsynth"]["dataDirectory"], "");
	std::string padFilename = ofFilePath::join(dataDirectory, padBasename);
	std::string fullpath = ofToDataPath(padFilename);
	synthMutex.lock();
	synthLoaded = synth.load(fullpath);
	synthMutex.unlock();
}


bool ofApp::loadPatch(size_t patchIdx){
	if(patchIdx >= patches.size()){
		return false;
	}

	auto setAnalog = [this](size_t idx, float normValue){
		uint8_t value = static_cast<uint8_t>(round(normValue * 0xff));
		analogInputs[idx].update(value, true);
	};

	auto getInstrumentIndex = [](
			std::vector<InstrumentScreen::Instrument> &insts,
			std::string name)->int{
		int idx = 0;
		for(auto& inst : insts){
			if(inst.name == name){
				return idx;
			}
			++idx;
		}
		return -1;
	};

	auto& patch = patches[patchIdx];

	// Find the instruments first, if this fails change nothing.
	int cornerIndexes[4];
	for(size_t corner=0; corner<4; ++corner){
		int idx = getInstrumentIndex(
				instrumentScreens[corner].instruments,
				patch.instruments[corner]);
		if(idx==-1){
			ofLog(OF_LOG_WARNING) << "Failed to load patch " << (patchIdx + 1)
				<< " as corner " << corner << " instrument '"
				<< patch.instruments[corner] << "' not found";
			return false;
		}
		cornerIndexes[corner] = idx;
	}

	setAnalog(POSITION, patch.position);
	setAnalog(ATTACK, patch.attack);
	setAnalog(DECAY, patch.decay);
	setAnalog(SUSTAIN, patch.sustain);
	setAnalog(RELEASE, patch.release);

	bool changed = false;
	for(size_t corner=0; corner<4; ++corner){
		int idx = cornerIndexes[corner];
		if(idx!=instrumentScreens[corner].idx){
			changed = true;
			instrumentScreens[corner].idx = idx;
			particleScreen.setInstrument(
					corner, instrumentScreens[corner].getCurrent().abbr);
		}
	}
	if(changed){
		loadPad();
	}

	for(size_t idx=0; idx<2; ++idx){
		gridSelection[idx] = clamp(
				static_cast<int>(patch.grid[idx] * (GRID_SIZE-1)),
				0, GRID_SIZE-1);
	}

	return true;
}


bool ofApp::savePatch(size_t patchIdx){
	if(patchIdx >= patches.size()){
		return false;
	}

	auto& patch = patches[patchIdx];
	patch.position = analogInputs[POSITION].getNormalized();
	patch.attack = analogInputs[ATTACK].getNormalized();
	patch.decay = analogInputs[DECAY].getNormalized();
	patch.sustain = analogInputs[SUSTAIN].getNormalized();
	patch.release = analogInputs[RELEASE].getNormalized();
	for(size_t idx=0; idx<4; ++idx){
		patch.instruments[idx] = instrumentScreens[idx].getCurrent().name;
	}
	patch.grid[0] = float(gridSelection[0]) / float(GRID_SIZE-1);
	patch.grid[1] = float(gridSelection[1]) / float(GRID_SIZE-1);

	return patches.save();
}


// Returns the index of a character in a string, or -1 if it is not found.
static int findChr(const char *str, int chr){
	for(int idx=0; str[idx]; ++idx){
		if(str[idx] == chr){
			return idx;
		}
	}
	return -1;
};


// Returns the index of a key in a list of keys, or -1 if it is not found.
static int findKey(const std::vector<int> &keys, int key){
	int idx = 0;
	for(auto needle : keys){
		if(needle == key){
			return idx;
		}
		++idx;
	}
	return -1;
};


void ofApp::keyPressed(int key){
	int idx;
	if((idx = findChr("ghjkl;", key)) >= 0){
		int current = analogInputs[idx].displayedValue;
		updateAnalogInput(idx, std::min(current + 5, 255), true);
	}else if((idx = findChr("bnm,./", key)) >= 0){
		int current = analogInputs[idx].displayedValue;
		updateAnalogInput(idx, std::max(current - 5, 0), true);
	}else if((idx = findChr("asdf", key)) >= 0){
		updateRotary(idx, 1);
	}else if((idx = findChr("zxcv", key)) >= 0){
		updateRotary(idx, -1);
	}else if((idx = findChr(keyNotes, key)) >= 0){
		if (!keyDown[idx]) {
			keyDown[idx] = true;
			synth.addNoteOn(idx, 1.0);
		}
	}else if(findChr("[]'#", key) >= 0){
		if(key == '['){
			synth.loopStart = std::max(0.f, synth.loopStart - 0.01f);
		}else if(key == ']'){
			synth.loopStart = std::min(1.f, synth.loopStart + 0.01f);
		}else if(key == '\''){
			synth.loopEnd = std::max(0.f, synth.loopEnd - 0.01f);
		}else if(key == '#'){
			synth.loopEnd = std::min(1.f, synth.loopEnd + 0.01f);
		}

		if (synth.loopStart > synth.loopEnd) {
			std::swap(synth.loopStart, synth.loopEnd);
		}
		printf("start: %.03f    end: %.03f\n", synth.loopStart, synth.loopEnd);
	}else if((idx = findKey(patchKeys, key)) >= 0){
		updatePatchInput(idx, true);
	}
}


void ofApp::keyReleased(int key){
	int idx;
	if((idx = findChr(keyNotes, key)) >= 0){
		keyDown[idx] = false;
		synth.addNoteOff(idx);
	}else if((idx = findKey(patchKeys, key)) >= 0){
		updatePatchInput(idx, false);
	}
}


void ofApp::mouseMoved(int x, int y){
	std::array<int, 2> p = particleScreen.screenToModel(
			static_cast<float>(x), static_cast<float>(y));
	updateGridSelection(p);
}


void ofApp::mouseDragged(int x, int y, int button){
}


void ofApp::mousePressed(int x, int y, int button){
}


void ofApp::mouseReleased(int x, int y, int button){
}


void ofApp::mouseEntered(int x, int y){
}


void ofApp::mouseExited(int x, int y){
}


void ofApp::windowResized(int w, int h){
}


void ofApp::gotMessage(ofMessage msg){
}


void ofApp::dragEvent(ofDragInfo dragInfo){
}


void ofApp::audioOut(ofSoundBuffer& buffer){
	int n = buffer.size() / 2;
	monoBuffer.resize(n);
	// we need to use the sampleRate from settings instead of
	// buffer.getSampleRate()
	// https://github.com/openframeworks/openFrameworks/issues/5588
	if(synthLoaded){
		synthMutex.lock();
		synth.audio_loop(monoBuffer, sampleRate);
		synthMutex.unlock();
	}else{
		monoBuffer.assign(n, 0.0f);
	}
	int j = 0;
	for(int i = 0; i < n; i++){
		buffer[j++] = monoBuffer[i];
		buffer[j++] = monoBuffer[i];
		if(isnan(monoBuffer[i])){
			printf("ERROR: sGOT A NaN!!!\n");
		}
	}
}
