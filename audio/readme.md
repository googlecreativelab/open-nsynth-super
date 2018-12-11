# Open NSynth Super audio pipeline

By following this guide, you will be able to prepare audio samples in WAV format, process and interpolate them using NSynth, and deploy them to an Open NSynth Super device.

A fast computer is required to produce audio in a reasonable amount of time; however it is possible to speed up the process by simplifying the task. The variables are:

- Batch size (per process)
- Number of interpolation combinations
- Number of example pitches
- Grid resolution

As an example, a complex interpolation task (e.g. to produce the sounds in the [large example OS image](https://storage.googleapis.com/open-nsynth-super/images/onss_1.2.0_full.img.bz2)) included 4 different sounds in each corner of the instrument for a total of 256 different permutations, a 9x9 grid resolution, and 15 example pitches. (The source audio for generating these patches is available [here](https://storage.googleapis.com/open-nsynth-super/audio/onss_source_audio.tar.gz).)

For reference, a machine with 8 NVidia K80 GPUs, 128GB RAM and 8 CPU cores can generate the nearly 120,000 audio samples produced by this example in approximately 36 hours with a conservative batch size of 512.

By reducing the number of instruments in each corner to 2, the time required is reduced significantly; with a far more modest server (2x GPU, 2x CPU 16GB RAM) it would be possible to produce a patch of this size in a couple of days.

This directory includes a sequence of scripts wrapping core NSynth functions, and for processing and converting audio for deployment on the device.
You will need to a Linux PC or server with at least one CUDA-compatible GPU.

# Key files

`/settings.json` is the settings file which governs each stage of the pipeline. If running on multiple machines, ensure that this is identical across instances.

All work should be performed in the `/workdir` directory. It contains the pipeline scripts, numbered 1 - 6; these stages are:

1. Compute input file embeddings
2. Compute embeddings for the output sounds to be generated
3. Batch the output embeddings for parallel processing
4. Generate audio (this is markdown reference file rather than a script; commands should be run manually)
5. Clean the generated audio files
6. Build pads for deployment on the device

# Setting up your environment
These steps assume you have installed and configured NVIDIA CUDA correctly on your machine, which is outside the scope of this document (if you’ve been able to run Tensorflow GPU on CUDA previously, then these instructions will apply correctly). You may wish to make use of a virtual environment for some steps; this is not documented. Note that you may need additional permissions on your machine (via `sudo`) for some steps if you are installing in global scope.

### Install Bazel:

(More detailed instructions on this step are available from the [Bazel website](https://docs.bazel.build/versions/master/install-ubuntu.html).)
```
$ sudo apt-get install openjdk-8-jdk
$ echo "deb [arch=amd64] http://storage.googleapis.com/bazel-apt stable jdk1.8" | sudo tee /etc/apt/sources.list.d/bazel.list
curl https://bazel.build/bazel-release.pub.gpg | sudo apt-key add -
$ sudo apt-get update
$ sudo apt-get install bazel
$ sudo apt-get upgrade bazel
```

### Python

Check Python version is 2.7 and install / upgrade `tqdm` and `pip`:

```
$ python --version
$ pip install tqdm
$ pip install --upgrade pip
```

### Magenta
Fetch and build Magenta’s multigpu branch

```
$ git clone https://github.com/jesseengel/magenta.git
$ cd magenta
$ git checkout origin/synth-multigpu
$ bazel build //magenta/tools/pip:build_pip_package
$ bazel-bin/magenta/tools/pip/build_pip_package ~/magenta-pip-pkg --gpu
$ cd ~/magenta-pip-pkg
$ pip install -U {filename}
```

Download and install the WaveNet checkpoint file from Magenta on GitHub:

```
$ cd magenta/magenta/models/nsynth
$ wget http://download.magenta.tensorflow.org/models/nsynth/wavenet-ckpt.tar
$ tar xf wavenet-ckpt.tar
```

# Preparing to run the pipeline

### 1. Preparing audio
Open NSynth Super requires one-note samples for each of the desired source sounds across a range of pitches. In the default configuration, these should be 4000ms long. Optionally, you can release the note after 3000ms to leave a decay, however this is subjective, and something to experiment with.

According to the default settings, these files should be created at the following MIDI notes:

- 24 (C2)
- 28 (E2)
- 32 (G#2)
- 36 (C3)
- 40 (E3)
- 44 (G#3)
- 48 (C4)
- 52 (E4)
- 56 (G#4)
- 60 (C5)
- 64 (E5)
- 68 (G#5)
- 72 (C6)
- 76 (E6)
- 80 (G#6)
- 84 (C7)


For instruments that are out of range at any of these pitches, you can pitch down or up using software, or repeat the nearest ‘available’ pitch to fill in the gaps. Alternatively, for unpitched sounds like drums you can maintain a single pitch across all notes, or pitch each one manually.

Changing these conditioning values can result in interesting different combinations and is one of the most interesting and powerful ways to alter the behaviour of the instrument.

Samples should be named using the following convention:

```
[soundname]_[pitch].wav
```

For example, the sound ‘guitar’ would be stored as:

```
guitar_24.wav
guitar_28.wav
...
```

Audio should be saved as 16-bit integer wave files at 16000kHz. See the following soxi output for a valid input file:

```
$ soxi guitar_24.wav

Input File     : 'grungebass_24.wav'
Channels       : 1
Sample Rate    : 16000
Precision      : 16-bit
Duration       : 00:00:04.00 = 64000 samples ~ 300 CDDA sectors
File Size      : 128k
Bit Rate       : 256k
Sample Encoding: 16-bit Signed Integer PCM
```

When you have selected the input audio files you wish to interpolate, they should be placed in the `audio_input` folder in the pipeline working directory. There is a helper script for converting AIF audio files to the correct format in `workdir/helpers`.


### 2. Update the settings file

The entire pipeline process is governed by the settings.json file, which will need to be reflected in the application. For example:
```
{
   "pads":  {
      "NW":       ["brass","caridle", "crotales", "electroclav"],
      "NE":       ["fatpoly", "flute", "softpad", "kick"],
      "SW":       ["marimba", "electricpiano", "snare", "steeldrum"],
      "SE":       ["synthbrass", "cleanbass", "uprightbass", "windfx"]
   },

   "magenta_dir": "~/magenta",
   "pitches":     [24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, 68, 72, 76, 80, 84],
   "resolution":  9,
   "final_length": 60000,
   "gpus":        16
}
```

##### pads
The pads property is a map of instrument layout. This object places the source audio files in different corners of the NSynth instrument; its properties are the compass locations of each corner. The lists should reflect the instrument filenames, i.e. ‘guitar’ will point the pipeline to all the correctly named ‘guitar_24.wav’, ‘guitar_28.wav’ files.

##### magenta_dir
This should point to the root directory of your Magenta installation. This is used to locate the WaveNet checkpoint file so should be set to the Magenta installation containing that file if you have multiple installations.

##### pitches
This is a list of the different input pitches that should be interpolated by the pipeline. This (as with all settings) should be reflected in the application settings file to ensure proper indexing of the audio files.

##### resolution
This setting controls how many inter-instrumental interpolations are created. Higher resolutions will create closer and smoother interpolations at a cost of disk space and processing time.

##### final_length
The length in samples of the output wave files.

##### gpus
Set this number to match the number of GPUs your system is equipped with. This will control the number of batches the processes are split into (enabling much faster processing times).

# Running the pipeline


### 1. Compute inpute embeddings

```
$ python 01_compute_input_embeddings.py
```

On occasion, this script can chop the leading character of some filenames, resulting in 'missing file' errors later on in the pipeline. You can use the `correct_truncated_names.py` helper script (in `workdir/helpers`) to correct these filenames in batches.

### 2. Compute interpolated embeddings

```
$ python 02_compute_new_embeddings.py
```

This script interpolates between the input embeddings according to the different permutations of input sounds and resolution. The target embeddings are placed in the `embeddings_output` folder.


### 3. Batch embeddings for processing
```
$ python 03_batch_embeddings.py
```

This script divides the audio embeddings into a folder per-GPU according to the settings file, in the `embeddings_batched` folder.

### 4. Generate audio
This command should be run manually, using the Magenta `nsynth_generate` command. You will need to run this command once for each batch / GPU. It is recommended that you run these commands in a series of detachable terminal sessions (for example using [`screen`](https://linux.die.net/man/1/screen) or [`tmux`](https://linux.die.net/man/1/tmux)) so that you can leave the process running for extended periods of time without having to maintain a session on the machine (even on a multi-GPU server, this process will take tens of hours depending on configuration and process settings).

The command should be invoked as follows:

```
$ nsynth_generate --checkpoint_path=/home/user/magenta/magenta/models/nsynth/wavenet-ckpt/model.ckpt-200000 --source_path=/home/user/nsynth-pipeline/workdir/embeddings_batched/batch0 --save_path=/home/user/nsynth-pipeline/workdir/audio_output/batch0 --batch_size=512 --alsologtostderr --gpu_number=0
```

You should run this command for each batch / GPU, updating the `batchN` file paths and `gpu=N` argument. This process will take a considerable amount of time depending on machine spec and arguments (e.g. batch size; number of input instruments).

### 5. Clean files

New NSynth audio files will be placed in `batchN` folders under the `output_audio` directory. Inside this directory, create a `raw_wav` folder and move all `batchN` folders into it:

```
$ mkdir audio_output/raw_wav
$ mv audio_output/batch* audio_output/raw_wav
```

The cleaning script will convert all input wave files to MP3, normalise them, and export as wave files for packing onto the device. This process is also batched; you can run it once for each batch (put the batch number after the script invocation).

```
$ python 05_clean_files.py 0
$ python 05_clean_files.py 1
...
```

### 6. Build pads
```
$ python 06_build_pads.py
```

This script will pack the wave files into efficient binary files for fast access on the device. It will output a single binary file for each instrument combination to the `pads_output` folder.

### 7. Deploy to the device
The the `pads_output` folder will now contain a binary file for each conmbination of input instruments, ready to load on to the device.

Once you have copied the contents of the `pads_output` folder to the device, you will need to update the `settings.json` file according to the new instrument names and pipeline settings. For more information on this, refer to the [application settings](../app) guide.