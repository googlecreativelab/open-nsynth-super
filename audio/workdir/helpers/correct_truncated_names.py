#	Use this script to correct any truncated filenames (this can occur on occasion after computing input embeddings)

import os, sys

missing_part = sys.argv[1]
corrected_name = sys.argv[2]

for original_name in os.listdir('.'):
	if missing_part in original_name:
		new_name = original_name.replace(missing_part, corrected_name)
		os.rename(original_name, new_name)
