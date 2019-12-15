import os
import subprocess
import argparse
import math
import random
import numpy as np
import itertools
from copy import deepcopy
from datetime import datetime
import re
from GenPool import GenPool
from eval import convert_to_multi_threaded_code

def read_script(filename):
	lock_candidates = []
	with open(filename, 'r') as script:
		for line in script:
			if re.search('[a-zA-Z]', line):
				# Ignore meta information
				continue
			else:
				lock_candidates.append(tuple(map(lambda x: int(x), line.split())))
	return lock_candidates


if __name__ == "__main__":
	parser = argparse.ArgumentParser()
	parser.add_argument("src", type=str)
	args = parser.parse_args()

	assert args.src

	with open('a.txt', 'w') as f:
		subprocess.call(['../GlobalVarFinder/gm', args.src], stdout=f)
	
	lock_candidates = read_script('a.txt')

	pool = GenPool(16, 100, args.src, lock_candidates)
	best_gene = pool.get_best()
	
	print(best_gene)

	# Generate src files under ${datetime.now()} directory
	offset = datetime.now().isoformat()
	os.mkdir(offset)
	with open(args.src, 'r') as f:
		text = convert_to_multi_threaded_code(f, best_gene)
		with open(offset + '/1.cpp', 'w') as f2:
			f2.write(text)
