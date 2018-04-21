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


ofApp::ofApp() : ofBaseApp(), midiThread(synthMutex, synth){
}


void ofApp::setup(){
	settings = ofLoadJson("localSettings.json");
	if(settings.is_null()){
		settings = ofLoadJson("settings.json");
	}

	setupCorners();
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

	i2c = open("/dev/i2c-1", O_RDWR);
	oledScreenDriver.setup(i2c, OLED_I2C_ADDR, OLED_BCM_RESET_PIN);

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


void ofApp::setupSound(){
	auto config = settings["audio"];
	auto devices = ofSoundStreamListDevices();
	sampleRate = settings["audio"]["sampleRate"];
	std::cout << "Sample rate: " << sampleRate << std::endl;
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
}


void ofApp::update(){
	readInputs();

	synthMutex.lock();
	synth.set_position(analogInputs[0].getNormalized());
	synth.set_attack(0.0001 + analogInputs[1].getNormalized());
	synth.set_decay(analogInputs[2].getNormalized());
	synth.set_sustain(analogInputs[3].getNormalized());
	synth.set_release(analogInputs[4].getNormalized());
	synth.set_volume(analogInputs[5].getNormalized());
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
			synthMutex.lock();
			synth.on(msg.getArgAsInt(0), msg.getArgAsFloat(1));
			synthMutex.unlock();
		}else if(addr == "/off"){
			synthMutex.lock();
			synth.off(msg.getArgAsInt(0));
			synthMutex.unlock();
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
		for(int i=0; i<6; ++i){
			analogInputs[i].init(message.potentiometers[i]);
		}
	}else{
		// Update the analog inputs.
		for(int i=0; i<6; ++i){
			updateAnalogInput(i, message.potentiometers[i], false);
		}

		// Update the instrument selections.
		for(int i=0; i<4; ++i){
			int8_t delta = message.rotaries[i] - lastInputsMessage.rotaries[i];
			updateRotary(i, delta);
		}
	}

	updateGridSelection({message.touch[0], message.touch[1]});

	inputsRead = true;
	lastInputsMessage = message;
}


void ofApp::draw(){
	double elapsed = ofGetLastFrameTime();

	particleScreen.updateParticles(
			{(float)gridSelection[0], (float)gridSelection[1]},
			elapsed);

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


void ofApp::updateAnalogInput(int idx, uint8_t value, bool force){
	bool changed = analogInputs[idx].update(value, true);
	float normalized = analogInputs[idx].getNormalized();

	if(idx == 0){
		positionScreen.position = normalized;
		if(changed || force){
			setScreen(&positionScreen);
		}
	}else if(idx == 1){
		envelopeScreen.attack = normalized;
		if(changed || force){
			envelopeScreen.stage = 0;
			setScreen(&envelopeScreen);
		}
	}else if(idx == 2){
		envelopeScreen.decay = normalized;
		if(changed || force){
			envelopeScreen.stage = 1;
			setScreen(&envelopeScreen);
		}
	}else if(idx == 3){
		envelopeScreen.sustain = normalized;
		if(changed || force){
			envelopeScreen.stage = 2;
			setScreen(&envelopeScreen);
		}
	}else if(idx == 4){
		envelopeScreen.release = normalized;
		if(changed || force){
			envelopeScreen.stage = 3;
			setScreen(&envelopeScreen);
		}
	}else if(idx == 5){
		volumeScreen.volume = normalized;
		if(changed || force){
			setScreen(&volumeScreen);
		}
	}
}


void ofApp::setScreen(BaseScreen *screen){
	currentScreen = screen;
	if(screen == &particleScreen){
		currentScreenTimeout = 0.0;
	}else{
		currentScreenTimeout = SCREEN_TIMEOUT;
	}
}


void ofApp::loadPad(){
	vector<string> parts;
	for(auto& instrumentScreen : instrumentScreens){
		parts.push_back(instrumentScreen.getCurrent().name);
	}
	string padBasename = ofJoinString(parts, "_");
	auto dataDirectory = getDefault(settings["nsynth"]["dataDirectory"], "");
	string padFilename = ofFilePath::join(dataDirectory, padBasename);
	synthMutex.lock();
	synthLoaded = synth.load(ofToDataPath(padFilename));
	synthMutex.unlock();
}


// Returns the index of a character in a string, or -1 if it is not found.
static int findChr(const char *str, int chr){
	for (int idx=0; str[idx]; ++idx) {
		if (str[idx] == chr) {
			return idx;
		}
	}
	return -1;
};


void ofApp::keyPressed(int key){
	int idx;
	if((idx = findChr("ghjkl;", key)) >= 0){
		int current = analogInputs[idx].readValue;
		updateAnalogInput(idx, std::min(current + 5, 255), true);
	}else if((idx = findChr("bnm,./", key)) >= 0){
		int current = analogInputs[idx].readValue;
		updateAnalogInput(idx, std::max(current - 5, 0), true);
	}else if((idx = findChr("asdf", key)) >= 0){
		updateRotary(idx, 1);
	}else if((idx = findChr("zxcv", key)) >= 0){
		updateRotary(idx, -1);
	}else if((idx = findChr("qwertyuiop", key)) >= 0){
		synthMutex.lock();
		synth.on(idx);
		synthMutex.unlock();
	}
}


void ofApp::keyReleased(int key){
	int idx;
	if((idx = findChr("qwertyuiop", key)) >= 0){
		synthMutex.lock();
		synth.off(idx);
		synthMutex.unlock();
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
