# Open NSynth Super application

Open NSynth Super is run by an openFrameworks application that is installed on the Raspberry Pi. It communicates with the OLED display, receives input from the encoders and potentiometers via the microcontroller, processes incoming MIDI data, and outputs audio. The app is built using openFrameworks 0.9.8 and the following addons:

- [SoundParts](https://github.com/kylemcdonald/SoundParts) by Kyle McDonald
- [ofJson](https://github.com/openframeworks/openFrameworks/blob/master/libs/openFrameworks/utils/ofJson.h) from openFrameworks (development version)

This document includes a guide for setting up the application, and additional details for configuring the application (e.g. MIDI channel, file paths, audio settings).

# Installation guide

1. If you haven't already copied this repository to the device, you will need to do so. You can run the following command on your device to find out its IP address:
	```
	$ ifconfig | grep inet
	inet addr: <DEVICE IP ADDRESS> Bcast: 255.255.255.0 Mask 255.255.255.0
	```

	You can copy the repository directory to the device from your host computer via SCP:

	```
	$ scp -r open-nsynth-super-master pi@<DEVICE IP ADDRESS>:~
	```

2. These instructions assume that you have run the setup script from the [provisioning readme](../linux), which establishes the file structure used in this guide.

	On the Pi, navigate to the openFrameworks apps directory, and create a new directory for the ```open-nsynth``` application.

	```
	$ cd /home/pi/opt/of/apps/
	$ mkdir open-nsynth
	```

3. Copy the contents of the app directory in this repository into the openFrameworks app directory you just created on the device.

	```
	$ cp -r /home/pi/open-nsynth-super-master/app/open-nsynth /home/pi/opt/of/apps/open-nsynth
	```

4. When the directory has copied, navigate to it, and build the application. This will take up to 2 hours.

	```
	$ cd /home/pi/opt/of/apps/open-nsynth/open-nsynth
	$ make -j4
	```

	The openFrameworks build script will inform you if the application has installed successfully. When you see this message, the application is set up. If you are using an example settings file, you should copy it now to the application folder:

	```
	$ cp /home/pi/settings.json /home/pi/opt/of/apps/open-nsynth/open-nsynth/bin/data/settings.json
	```

	If you are following the provisioning instructions, you can return to that guide now.

# Changing application settings

The application is configured via a JSON file (`settings.json`) which is located in the `/home/opt/of/apps/open-nsynth/open-nsynth/bin/data/` directory. The most important property is instrument assignment (which source sounds are placed in which corner), which should match exactly the names of the audio patches that you are using.

### Audio data path
Set the property `nsynth: dataDirectory` to the absolute path of the audio data directory (either the [1.7GB lite](https://storage.googleapis.com/open-nsynth-super/audio/onss_lite_audio.tar.gz) or [26GB full](https://storage.googleapis.com/open-nsynth-super/audio/onss_full_audio.tar.gz) sample audio, or created using the [audio pipeline](../audio)).

### Pitches
The `nsynth: pitches` array should contain all of the input note values that you have trained the dataset on. Setting this incorrectly will cause pitches and voicings to be mismatched on playback.

### Instrument assignment
Use `corners: instruments` to assign instruments to the corners of the interface. The order of these arrays are: top left, top right, bottom left, bottom right. The `name` property should match the filename; `display` will set the text displayed in the instrument selection interface on the device (with a limit of 8 characters); `abbr` is the two character abbreviation used in the main display.

### MIDI channel
The default MIDI device should be unchanged, while the channel can be safely altered with the `channel` property.

### Loop points
There is a default audio looping point defined in the settings file. By tweaking the values of `loopStart` and `loopEnd` you can change the point in the sample where the device will loop notes. Looping can be enabled and disabled with the `looping` boolean.

### Patch Saving
The patch saving feature in version 1.2.0 enables storing and recalling settings. The path for this file can be set with the `patchFile` property.

# Deploying new sounds to the device
Any sounds that you've created using the [audio pipeline](../audio) should be placed in a directory on the device. As this can be many gigabytes of audio data, it's recommended that you remove the SD card from the device and copy locally using a Linux computer. (You may need to wait for `fsck` to run when you reinsert the card to the device and boot up for the first time after copying.)

The `dataDirectory` setting needs to be updated to match the new file and instrument names that have been copied. It's quite a common error to input the instrument names incorrectly, or in the wrong order, which prevents the software from locating the audio files. If you experience any issues after updating audio, this is the most likely issue.
