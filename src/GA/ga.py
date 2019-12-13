import os
import argparse
import math
import random
import numpy as np
import itertools
from copy import deepcopy
from datetime import datetime
import re
import gen_pool

def read_script(path):
	lock_candidates = []
	with open(args.script, 'r') as script:
		for line in script:
			if re.search('[a-zA-Z]', line):
				# Ignore meta information
				continue
			else:
				lock_candidates.append(tuple(map(lambda x: int(x), line.split())))
	return lock_candidates



if __name__ == "__main__":
	parser = argparse.ArgumentParser()
	parser.add_argument("script", type=str)
	parser.add_argument("src", type=str)
	args = parser.parse_args()

	assert args.script

	lock_candidates = read_script(args.script)
	offset = datetime.now().isoformat()
	# Generate src files under ${datetime.now()} directory
	os.mkdir(offset)

	pool = gen_pool.GenPool(16, 100, args.src, offset, lock_candidates)
	lock_gene = pool.get_best()
	
	print(lock_gene)
