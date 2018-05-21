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

#include "ofThread.h"

#include "NSynth.h"


// Listens to MIDI data on a serial port.
class MidiThread : public ofThread{
	public:
		MidiThread(std::mutex &synthMutex, NSynth &synth);
		// Opens the serial device, to be called before startThread().
		bool setup(const std::string &device, int channel);
		// The thread execution method.
		void threadedFunction() override;

	private:
		// A guard for the synth instance.
		std::mutex &synthMutex;
		// The synth instance to send note events to.
		NSynth &synth;
		// The opened serial port.
		int deviceFd;
		// The MIDI channel to listen on (0-15).
		int channel;
};
