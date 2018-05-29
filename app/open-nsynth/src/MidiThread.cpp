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

// Modified 2018 by J. Ditterich for Note On messages with zero velocity
// and running status being interpreted correctly

#include "ofLog.h"

#include "MidiThread.h"

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

// States for state machine
#define WAIT_FOR_STATUS 1
#define NOTE_ON_WAIT_FOR_NOTE 2
#define NOTE_ON_WAIT_FOR_VEL 3
#define NOTE_OFF_WAIT_FOR_NOTE 4
#define NOTE_OFF_WAIT_FOR_VEL 5
#define RUNNING_NOTE_ON_WAIT_FOR_VEL 6
#define RUNNING_NOTE_OFF_WAIT_FOR_VEL 7


MidiThread::MidiThread(Poco::FastMutex &synthMutex, NSynth &synth)
	: ofThread(), synthMutex(synthMutex), synth(synth){
}


bool MidiThread::setup(const std::string &device, int channel){
	this->channel = channel;

	// The device setup is performed twice to ensure it works on device
	// startup.

	for(int i=0; i<2; ++i){
		if(i != 0){
			close(deviceFd);
		}

		deviceFd = open(device.c_str(), O_RDONLY | O_NOCTTY);
		if(deviceFd < 0){
			ofLog(OF_LOG_WARNING) << "Failed to open MIDI input " << device;
			return false;
		}

		// Configure the serial port at 38400 baud. Settings on the Raspberry Pi
		// should adapt this to the MIDI baud rate of 31250.
		struct termios tty;
		memset(&tty, 0, sizeof tty);
		if(tcgetattr(deviceFd, &tty) != 0){
			return false;
		}

		cfsetospeed(&tty, B38400);
		cfsetispeed(&tty, B38400);

		tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
		tty.c_iflag &= ~IGNBRK;
		tty.c_lflag = 0;
		tty.c_oflag = 0;
		tty.c_cc[VMIN]  = 1;
		tty.c_cc[VTIME] = 0;

		tty.c_iflag &= ~(IXON | IXOFF | IXANY);

		tty.c_cflag |= (CLOCAL | CREAD);
		tty.c_cflag &= ~(PARENB | PARODD);
		tty.c_cflag &= ~CSTOPB;
		tty.c_cflag &= ~CRTSCTS;

		if(tcsetattr(deviceFd, TCSANOW, &tty) != 0){
			return false;
		}
	}

	return true;
}


void MidiThread::threadedFunction(){
	auto read1 = [this]()->uint8_t{
		uint8_t result;
		while(read(deviceFd, &result, 1) != 1){
			if(!isThreadRunning()){
				return 0xff;
			}
		}
		return result;
	};

	uint8_t state = WAIT_FOR_STATUS;
	uint8_t running_status = 0;
	uint8_t header = 0;
	uint8_t note = 0;
	uint8_t velocity = 0;

	uint8_t midi_byte = read1();

	while(isThreadRunning()){
		switch (state) {
			case WAIT_FOR_STATUS:
				if (midi_byte < 0x80) { // unexpected data byte received
					if (running_status >= 0x90) { // We are still in Note On running status...
						note = midi_byte;
						state = RUNNING_NOTE_ON_WAIT_FOR_VEL;
					} else if (running_status) { // We are still in Note Off running status...
						note = midi_byte;
						state = RUNNING_NOTE_OFF_WAIT_FOR_VEL;
					}
				} else if ((midi_byte & 0xf0) == 0x90) { // Note On received
					header = midi_byte;
					state = NOTE_ON_WAIT_FOR_NOTE;
					running_status = header;
				} else if ((midi_byte & 0xf0) == 0x80) { // Note Off received
					header = midi_byte;
					state = NOTE_OFF_WAIT_FOR_NOTE;
					running_status = header;
				} else if ((midi_byte >= 0xa0) && (midi_byte <= 0xf7)) { // other Voice Category or System Common Category status received
					running_status = 0;
				} // RealTime Category messages are ignored and have no effect on state or running status

				break;

			case NOTE_ON_WAIT_FOR_NOTE:
				if (midi_byte < 0x80) { // data byte, as expected
					note = midi_byte;
					state = NOTE_ON_WAIT_FOR_VEL;
				} // everything else should be a RealTime Category message and has no effect on state or running status

				break;

			case NOTE_ON_WAIT_FOR_VEL:
				if (midi_byte < 0x80) { // date byte, as expected
					velocity = midi_byte;
					state = WAIT_FOR_STATUS;

					if (velocity) { // This is a real Note On message
						if ((header & 0x0f) == channel) {
							synthMutex.lock();
							synth.on(note, static_cast<float>(velocity) / 127.0f);
							synthMutex.unlock();
						}
					} else { // A velocity of zero has to be interpreted as Note Off...
						if((header & 0x0f) == channel){
							synthMutex.lock();
							synth.off(note);
							synthMutex.unlock();
						}
					}
				} // everything else should be a RealTime Category message and has no effect on state or running status

				break;

			case NOTE_OFF_WAIT_FOR_NOTE:
				if (midi_byte < 0x80) { // data byte, as expected
					note = midi_byte;
					state = NOTE_OFF_WAIT_FOR_VEL;
				} // everything else should be a RealTime Category message and has no effect on state or running status

				break;

			case NOTE_OFF_WAIT_FOR_VEL:
				if (midi_byte < 0x80) { // date byte, as expected
					state = WAIT_FOR_STATUS;

					if((header & 0x0f) == channel){
						synthMutex.lock();
						synth.off(note);
						synthMutex.unlock();
					}
				} // everything else should be a RealTime Category message and has no effect on state or running status

				break;

			case RUNNING_NOTE_ON_WAIT_FOR_VEL:
				if (midi_byte < 0x80) { // date byte, as expected
					velocity = midi_byte;
					state = WAIT_FOR_STATUS;

					if (velocity) { // This is a real Note On message
						if ((running_status & 0x0f) == channel) {
							synthMutex.lock();
							synth.on(note, static_cast<float>(velocity) / 127.0f);
							synthMutex.unlock();
						}
					} else { // A velocity of zero has to be interpreted as Note Off...
						if((running_status & 0x0f) == channel){
							synthMutex.lock();
							synth.off(note);
							synthMutex.unlock();
						}
					}
				} // everything else should be a RealTime Category message and has no effect on state or running status

				break;

			case RUNNING_NOTE_OFF_WAIT_FOR_VEL:
				if (midi_byte < 0x80) { // date byte, as expected
					state = WAIT_FOR_STATUS;

					if((running_status & 0x0f) == channel){
						synthMutex.lock();
						synth.off(note);
						synthMutex.unlock();
					}
				} // everything else should be a RealTime Category message and has no effect on state or running status

				break;
		}

		midi_byte = read1(); // read next byte
	}
}
