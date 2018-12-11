# Open NSynth Super Device provisioning

Use this guide to provision a new Open NSynth Super device from scratch. You may wish to skip this process and download one of the pre-made images: [64GB with all sample audio](https://storage.googleapis.com/open-nsynth-super/images/onss_1.2.0_full.img.bz2) or the [16GB 'lite' version](https://storage.googleapis.com/open-nsynth-super/images/onss_1.2.0_lite.img.bz2).

As part of the process of setting up a new system, you will also have to install the Open NSynth Super software application and firmware for the Open NSynth Super mainboard. There are separate documents in this repository to help with this, which are linked in this step-by-step guide. Creating a new OS image will take 1-2 hours (depending on your internet connection speed) however most of this time is spent downloading and compiling software; the 'hands-on' time required is considerably shorter.

You will need:

- A Raspberry Pi 3
- An assembled Open NSynth Super mainboard PCB
- An SD card with at least 8GB capacity
- A wireless or wired network connection
- Raspbian Stretch Lite ([version 2018-06-27](http://downloads.raspberrypi.org/raspbian_lite/images/raspbian_lite-2018-06-29/))
- A Mac, Linux or Windows PC with SD writing capability (for example using [Etcher](http://etcher.io))

### 1. Create the SD card (cross-platform)

The easiest way to create the SD card is to use the Etcher software. Download the [application](http://etcher.io) from the Etcher website and set up on your machine (the process for doing this will differ across platforms).

You will need to download the correct Raspbian Stretch Lite image from the [Raspbian archive](http://downloads.raspberrypi.org/raspbian_lite/images/raspbian_lite-2018-06-29/). Open NSynth Super may work with other releases too.

It should take about 5 minutes to flash the image, depending on the speed of your card reader and card.

### 2. Create the SD card (*NIX specific)

##### Mac OS

1. Download Raspbian Stretch Lite from the [Raspbian archive](http://downloads.raspberrypi.org/raspbian_lite/images/raspbian_lite-2018-06-29/).

2. Insert the SD card and use Disk Utility to check your system for its path:

	```
	$ diskutil list

	/dev/<DISK_NAME> (external, physical):
	#:	TYPE NAME 					SIZE 			IDENTIFIER
	0:	FDisk_partition_scheme		*127.9 GB 		disk2
	1:	Windows_FAT_32				127.9 GB 		disk2s1
	```

3. Unmount the disk:

	```
	$ diskutil unmountDisk /dev/<DISK_NAME>
	Unmount of all volumes on disk2 was successful
	```

4. Start the copying process to flash the image:

	```
	$ sudo dd if=2018-06-27-raspbian-stretch-lite.img of=<DISK_NAME> bs=64m
	```

5. Upon completion, eject the disk and insert into the Raspberry Pi:

	```
	$ sudo diskutil eject /dev/<DISK_NAME>
	Disk /dev/disk2 ejected
	```

##### Linux

1. Download Raspbian Stretch Lite from the [Raspbian archive](http://downloads.raspberrypi.org/raspbian_lite/images/raspbian_lite-2018-06-29/).

2. Insert the SD card and use check your system for its path (to ensure you choose the correct path, list the contents of `/dev/sd*` before and after inserting the card):

	```
	$ ls /dev/sd*
	/dev/sda	/dev/sdb	/dev/sdb1	/dev/sdc	/dev/sdc1
	```

3. Unmount the SD card:

	```
	$ sudo umount /dev/<DISK_NAME>
	```

4. Start the copy process to flash the image:

	```
	$ sudo dd if=2018-06-27-raspbian-stretch-lite.img of=/dev/<DISK_NAME> bs=64M
	```

5. Upon completion, unmount the disk and insert into the Raspberry Pi:

	```
	$ sudo umount /dev/<DISK_NAME>
	```

### 3. Prepare the OS

1. First, connect the Open NSynth Super mainboard to the Raspberry Pi 3, and plug in a screen and keyboard. You can now insert the SD card and power up by connecting a micro USB power supply to the port on the ONS mainboard. The device will power up, and expand the filesystem before rebooting. When the system has booted to a login prompt, enter the default username and password (`pi` / `raspberry`).

2. Connect the device to a network; either using an ethernet cable, or using the built-in wifi of the Raspberry Pi 3 (for instructions, follow [this guide](https://wiki.archlinux.org/index.php/WPA_supplicant#Connecting_with_wpa_cli), starting from 'At this poin run:' around half way down the page; you don't need to set up `wpa_supplicant` as it is preconfigured on the Raspbian image).

3. Update the package list and upgrade any dependencies (this will take up to an hour depending on your internet connection speed):

	```
	$ sudo apt-get update
	$ sudo apt-get upgrade -y
	```

4. Enable SSH, I²C, serial, and SPI on the device in the 'Interfacing Options' of the Raspberry Pi configuration tool (they are listed under 'Interfacing Options' in the main menu of the tool):

	```
	$ sudo raspi-config
	```

5. You should now copy this repository to the device. You can run the following command on your Pi to find out its IP address:

	```
	$ ifconfig | grep inet
	```

	With this information, you can now copy the `open-nsynth-super-master` directory to the device via `scp` (running from your host computer):

	```
	$ scp -r open-nsynth-super-master pi@<DEVICE IP ADDRESS>:~
	```

6. There is a script included 
in the repository which will set up all OS dependencies and establish a file structure in the home directory on the device. To run the script, enter the following commands on the Pi (this will take up to an hour, based on your internet connection speed):

	```
	$ cd /home/pi/open-nsynth-super-master/linux
	$ bash nsynth-setup.sh
	```

	You will be returned to a command prompt when this script has finished running; if there are any errors logged to the console, you should address any issues and re-run the script (for example if any packages are unavailable, or the network connection is lost).

### 4. Copy audio files

You can use the [audio pipeline](../audio) in this repository to create your own audio patches for running on your Open NSynth Super device. To get you started, there are three example audio patches available to download: the [full set (27GB)](https://storage.googleapis.com/open-nsynth-super/audio/onss_full_audio.tar.gz) or ['lite' set (1.7GB)](https://storage.googleapis.com/open-nsynth-super/audio/onss_lite_audio.tar.gz). Whichever approach you take, ensure that the application settings are updated to match the filenames of the patches you've created or chosen.

When you have a directory of audio files (from either source) saved to your computer, copy them to the home directory of the Raspberry Pi. If possible, you should do this directly (by inserting the SD card into your computer); however you can also copy via a network (this will take considerably longer - up to a few hours for the larger sets of audio, depending on the speed of your network connection).

In addition to the audio files, you will need to copy the accompanying settings file(for the [full set](https://storage.googleapis.com/open-nsynth-super/audio/onss_full_settings.json), for the ['lite' set](https://storage.googleapis.com/open-nsynth-super/audio/onss_lite_settings.json)).

```
$ scp -r pads/ pi@<DEVICE IP ADDRESS>:~
$ scp onss_full_settings.json pi@<DEVICE IP ADDRESS>:~
```

When you have installed the application software in a later step, you will need to update the `settings.json` file in the application directory; for now, you can leave it in the home directory.

### 5. Set up the firmware

Having copied all necessary files to the device, you are ready to install the MCU firmware for the Open NSynth Super mainboard. There are detailed instructions on how to approach this in the [firmware readme](../firmware) file. When you have completed these instructions, you can return to this guide and move to the next step.

### 6. Set up the application

The only remaining element to set up on the device is the Open NSynth Super application itself. There is a detailed guide on how to accomplish this in the [application readme](../app) file. Make sure that if you are using an example `settings.json` file that you now move it to the application directory, for example:

```
$ cp onss_full_settings.json /home/pi/opt/of/apps/open-nsynth/open-nsynth/bin/data/settings.json
```

When you have completed these instructions, you can return to this guide and move to the next step.

### 7. Test the configuration

Before assembling the shell and 'finishing' the instrument, it's important to check that everything is functioning correctly. First, power off the device:

```
$ sudo poweroff
```

You can now remove the keyboard and screen from the Raspberry Pi. Connect a pair of headphones or speakers, and a MIDI keyboard (or other MIDI device). You can now power up the device.

After a few seconds, you should see the grid interface appear on the OLED screen. Move your finger around the touch pad to test its responsiveness. Next, adjust the six controls at the base of the unit; the UI should update according to the control that you are adjusting. Finally, test the four instruent selection encoders, which will scroll through an instrument list on the screen.

To test the audio, ensure your MIDI device is broadcasting on the channel you have set in your application configuration (the default channel is 1 for all the sample audio sets) and send some notes to the device. You should hear audio coming from the speakers or headphones (if you don't hear anything, make sure the device audio is turned up, and that the envelope and position controls aren't cutting the audible part of the waveform).

##### Debugging audio

If you aren't able to hear audio, you should power down the device, reconnect the keyboard and screen, and boot it up again. When you see the command prompt, enter the following commands:

```
$ sudo service open-nsynth stop
$ speaker-test
```

If your audio hardware is functioning, you should hear a test sound playing through the connected headphones or speakers.

##### Debugging MIDI

If your audio hardware is working correctly, but you aren't hearing audio played by the application via MIDI, you should first check that your device is broadcasting on the correct channel (this is set in the `settings.json` file in the Open NSynth Super application directory; for more information about the settings file, see the [application readme](../app) file).

##### Controlling the service

You can start and stop the `open-nsynth` service using the following commands:

```
$ sudo service open-nsynth stop
$ sudo service open-nsynth start
$ sudo service open-nsynth restart
```
### 7. Lock the filesystem (optional)

When you have set up your device and are happy with the configuration, it is advisable to lock the file system to prevent corruption upon a hard power down (i.e. when the power cable is removed from the device). Reconnect the keyboard and screen for these steps. 

1. Open `fstab` in a text editor
	```
	$ sudo vi /etc/fstab
	```

2. Add `,ro` to the root partition to lock the filesystem at next boot:

	```
	proc 					/proc           proc    defaults  	        	0 		0
	PARTUUID=7762b82d-01 	/boot           vfat    defaults				0 		2
	PARTUUID=7762b82d-02 	/               ext4    defaults,noatime,ro 	0 		1
	```

3. Reboot, and test the filesystem by using `touch xyz` to create a new file named `xyz`. You should be denied this if the system is in read-only mode.

4. To remount as read-write (to make changes), you can remove the ```,ro``` flag from fstab. To make this change temporarily (e.g. to edit some files), you can remount the filesystem as read-write:

	```
	$ sudo mount -o remount,rw /
	```

When you restart, the filesystem will once more be read-only. Your instrument is now fully set up – you can proceed to build it into a case and start to make music with it!
