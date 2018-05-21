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

#include "ofLog.h"

#include "MidiThread.h"

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>


MidiThread::MidiThread(std::mutex &synthMutex, NSynth &synth)
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

	uint8_t header = read1();
	while(isThreadRunning()){
		if((header & 0xf0) == 0x90){
			// Note on - this is a 3 byte message.
			uint8_t note = read1();
			if(!isThreadRunning()){
				break;
			}
			if(note & 0x80){
				// The note is badly formed, discard the current header
				// and continue;
				header = note;
				break;
			}

			uint8_t velocity = read1();
			if(!isThreadRunning()){
				break;
			}
			if(velocity & 0x80){
				// The note is badly formed, discard the current header
				// and continue;
				header = note;
				break;
			}

			if((header & 0x0f) == channel){
				synthMutex.lock();
				synth.on(note, static_cast<float>(velocity) / 127.0f);
				synthMutex.unlock();
			}

			// Start on the next message.
			header = read1();
		}else if((header & 0xf0) == 0x80){
			// Note off - this is a 3 byte message.
			uint8_t note = read1();
			if(!isThreadRunning()){
				break;
			}
			if(note & 0x80){
				// The note is badly formed, discard the current header
				// and continue;
				header = note;
				break;
			}

			uint8_t velocity = read1();
			if(!isThreadRunning()){
				break;
			}
			if(velocity & 0x80){
				// The note is badly formed, discard the current header
				// and continue;
				header = note;
				break;
			}

			if((header & 0x0f) == channel){
				synthMutex.lock();
				synth.off(note);
				synthMutex.unlock();
			}

			// Start on the next message.
			header = read1();
		}else{
			// Discard the header as it is not understood.
			header = read1();
		}
	}
}
