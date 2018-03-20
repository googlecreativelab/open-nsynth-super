# Open NSynth Super firmware

This firmware is responsible for reading hardware inputs and making that state available to the Raspberry Pi over I²C.

The inputs are:

 - 4 rotary encoders used for instrument selection
 - 6 potentiometers used for altering the sound envelope and volume
 - 2 touch controller ICs used for the touch sensitive grid

The output is an I²C interface using SMBus transactions to send the current state of the inputs.

# Installation guide

These instructions will guide you through the steps needed to build and install the firmware that runs on the Open NSynth Super mainboard microcontroller. For more information about setting up the rest of the system, you should follow first either the [main quick-start guide](https://github.com/googlecreativelab/open-nsynth-super) or the [full Linux setup](../linux) documentation. You should have the Open NSynth Super mainboard connected to the Raspberry Pi, and a keyboard and screen plugged in before starting.

1. If you haven't already copied this repository to the device, you will need to do so. You can run the following command on your device to find out its IP address:
	```
	$ ifconfig | grep inet
	inet addr: <DEVICE IP ADDRESS> Bcast: 255.255.255.0 Mask 255.255.255.0
	```

	You can copy the repository directory to the device from your host computer via SCP:

	```
	$ scp -r open-nsynth-super-master pi@<DEVICE IP ADDRESS>:~
	```

2. In addition to the files in the firmware directory of the repository, you will need some supporting libraries from STMicroelectronics. You can download the files from the bottom of [this page on their website](http://www.st.com/en/embedded-software/stm32cubef0.html). Note that you will have to register with STM in order to download the libraries. When you have downloaded the zip archive, you need to copy it to the device:

	```
	$ scp en.stm32cubef0.zip pi@192.168.0.10:~/open-nsynth-super-master/firmware
	```

	Next, unzip the archive, and rename the unzipped directory so that it can be found by the firmware compile script. (The renamed `cube` directory should be in the `firmware` directory, alongside the `src` directory.)

	```
	$ cd /home/pi/open-nsynth-super-master/firmware/
	$ unzip en.stm32cubef0.zip 
	$ mv STM32Cube_FW_F0_V1.9.0 cube
	```

3. With the files in place, you can now compile and install the firmware:
	```
	$ cd /home/pi/open-nsynth-super-master/firmware/src
	$ make
	$ make install
	```

	You will receive a message similar to the listing below if the installation was successful:

	```
	** Programming Started **
	auto erase enabled
	Info : device id = 0x10006444
	Info : flash size = 32kbytes
	wrote 12288 bytes from file ../bin/main.elf in 0.607889s (19.740 KiB/s)
	** Programming Finished **
	** Verify Started **
	verified 11788 bytes in 0.050784s (226.680 KiB/s)
	** Verified OK **
	** Resetting Target **
	adapter speed: 1001 kHz
	shutdown command invoked
	```

	In order to verify that the firmware is functioning correctly, you will need to install the Open NSynth Super application software; a guide for this is [available here](../app). You can now return to the provisioning guide if you have been following it.

# About the firmware

### Inputs

##### Rotary encoders

The rotary encoders are two channel devices, each one is connected to two digital input pins on the MCU. The state of these pins is monitored at fixed intervals to help with de-bouncing. The input state is represented as an 8 bit integer. When a clockwise turn is detected the input state is incremented by one, and when an anticlockwise turn is detected the input state is decremented by one. The input state is not clamped, it wraps around on overflow.

An application reading these values can detect changes by comparing the current value to the previous value.

##### Potentiometers

The potentiometers are connected to analog input pins and read via the MCUs ADC unit. The input state is an unsigned 8 bit integer.

Because ADC readings are subject to noise the firmware employs two filtering steps. The first as to average many readings together to reduce high frequency noise. The second is to not report sub-bit changes to reduce jitter when the input is close to a bit boundary.

##### Touch controllers

The touch controllers are responsible for reading 11 lines each. Both ICs communicate over I²C but unfortunately have a single fixed address. They are therefore unable to operate on the same I²C bus, so the MCU defines two additional I²C buses - one for each touch IC. The MCU of the Open NSynth Super mainboard has a single hardware I²C unit and this is used to communicate with the Raspberry Pi. The additional buses are defined in firmware by bit banging the pins.

The input state for each axis is an 8 bit integer. The value is 0-10 (inclusive) if a touch has been detected, or 255 (-1) if no touch has been detected. 

### Output

The hardware I²C unit is configured to communicate with the Raspberry Pi. The interface supports separate reads and writes as well as combined write-then-read transactions commonly used for SMBus transactions.

Due to timing constraints all read transactions read from an internal buffer. To update this buffer with the current input state a write transaction must be performed. Thus a typical read cycle consists of a write followed by a read, which again is commonly used for SMBus transactions.

The MCU sends 16 bytes of data for the inputs, followed by a 4 byte checksum. For efficiency the checksum is calculated as a 32 bit value. It is the sum of 0xAA55AA55, the first 32 bits of the inputs, the second 32 bits of the inputs and the third 32 bits of the inputs.

### Hardware used

 - The hardware I²C unit is used for communications with the host. It generates interrupts.
 - A timer (TIM16) is used to generate interrupts for reading the rotary encoders. 
 - The ADC unit is used for reading the potentiometers. It generates interrupts.
 - A timer (TIM17) is used to time the bit banged I²C interfaces.
 
### Priorities

 1. The hardware I²C interrupts are given the highest priority to ensure that communications are reliable.
 2. The rotary encoders are given the next highest priority so that they are read at regular intervals.
 3. The ADC reads are given the lowest interrupt priority.
 4. Reading from the touch ICs is performed as the base task in the main loop.
 
### Compatible devices

This firmware is designed for STM320F030K6Tx devices, and is also drop in compatible with STM320F031K6Tx.
