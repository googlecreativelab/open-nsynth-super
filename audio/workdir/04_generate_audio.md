#	Generating audio

The GPU setting in `settings.json` will determine the number of batches to split the input audio into (the target embeddings will be placed in `embeddings_output/batch*`, with one folder for each GPU).

To generate audio across multiple GPUs, the `nsynth_generate` command should be run manually, in separate `screen` sessions or using `tmux` to allow the process to continue in the event that the terminal disconnects.

1. Open one terminal for each GPU, for example using `screen`:
 
```
$ screen -S gpuX
```

2. If you want to monitor the status of your GPUs and CPUs, you can open additional terminals and use the following commands:

```
$ watch -n0.1 nvidia-smi`
$ top
```


2. In each `screen` session, start an instance of `nsynth_generate`. Note the changing `gpu_number` which should increase with the batch folder for both input and output. `batch_size` is dependent on GPU specification - a batch size of 512 across 4 GPUs is a stable configuration for a machine with 6x CPU / 4x GPU / 96GB RAM. This needs to be tweaked on a per-machine basis.
 
 ```
 $ nsynth_generate --checkpoint_path=/home/user/magenta/magenta/models/nsynth/wavenet-ckpt/model.ckpt-200000 --source_path=/home/user/nsynth-pipeline/workdir/embeddings_batched/batch0 --save_path=/home/user/nsynth-pipeline/workdir/audio_output/batch0 --batch_size=512 --alsologtostderr --gpu_number=0
 ```


3. When all processes are complete, each `batch` folder in `workdir/audio_output/` will be filled with .wav audio files. Before compiling pad files for deployment, move these files into the `audio_output` directory:

```
	$ mv audio_output/batch*/* audio_output/
```