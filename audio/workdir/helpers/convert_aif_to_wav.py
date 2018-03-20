#	Converts AIF input files to WAV

import os, subprocess

for fname in os.listdir('.'):
	nfn = fname.replace('aif', 'wav')
	subprocess.call(["sox", fname, "-b", "16", "-r", "16000", "-c", "1", nfn])
	os.remove(fname)
