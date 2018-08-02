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

#include "Gpio.h"

#include <chrono>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <thread>
#include <unistd.h>


constexpr int SET = 7;
constexpr int CLEAR = 10;
constexpr int GET = 13;
constexpr int MAP_SIZE = 0xb4;
constexpr int GPPUD = 37;
constexpr int GPPUDCLK = 38;


Gpio::Gpio(){
	int fd = open("/dev/gpiomem", O_RDWR | O_SYNC);
	if(fd >= 0){
		gpioReg = (uint32_t *)mmap(
				NULL, MAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

		if(gpioReg == MAP_FAILED){
			gpioReg = nullptr;
		}

		close(fd);
	}
}


Gpio::~Gpio(){
	if(gpioReg){
		munmap(const_cast<uint32_t *>(gpioReg), MAP_SIZE);
	}
}


Gpio::operator bool() const{
	return gpioReg != nullptr;
}


bool Gpio::setInput(int pin, Pull pull){
	if(!gpioReg || pin < 2 || pin > 27){
		return false;
	}

	int reg = pin / 10;
	int shift = (pin % 10) * 3;

	gpioReg[reg] = (gpioReg[reg] & ~(7<<shift));

	gpioReg[GPPUD] = pull & 3;
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
	gpioReg[GPPUDCLK] = 1 << pin;
	std::this_thread::sleep_for(std::chrono::milliseconds(1));

	gpioReg[GPPUD] = 0;
	gpioReg[GPPUDCLK] = 0;

	return true;
}


bool Gpio::setOutput(int pin){
	if(!gpioReg || pin < 2 || pin > 27){
		return false;
	}

	int reg = pin / 10;
	int shift = (pin % 10) * 3;

	gpioReg[reg] = (gpioReg[reg] & ~(7<<shift)) | (1<<shift);

	return true;
}


bool Gpio::write(int pin, bool high){
	if(!gpioReg || pin < 2 || pin > 27){
		return false;
	}

	gpioReg[high ? SET : CLEAR] = 1 << (pin & 0x1f);
	return true;
}


bool Gpio::read(int pin){
	if(!gpioReg || pin < 2 || pin > 27){
		return false;
	}

	return (gpioReg[GET] & (1 << (pin & 0x1f))) != 0;
}


void Gpio::reset(int pin, size_t uSecHold, size_t uSecDelay){
	// Set the pin to an output and drive it low.
	setOutput(pin);
	write(pin, false);
	std::this_thread::sleep_for(std::chrono::microseconds(uSecHold));

	// Restore the pin
	write(pin, 1);
	setInput(pin, PULL_UP);
	std::this_thread::sleep_for(std::chrono::microseconds(uSecDelay));
}
