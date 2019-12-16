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
from eval import convert_to_multi_threaded_code, use_eval

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

def write_answer(src):
	lock_candidates = None
	for filename in os.listdir(src):
		if not os.path.splitext(filename)[0].isdigit():
			continue 
		
		full_filename = os.path.join(src, filename)
		with open(os.path.join(src, 'lock_list.txt'), 'w') as f:
			subprocess.call(['../GlobalVarFinder/gm', full_filename], stdout=f)
		temp = read_script(os.path.join(src, 'lock_list.txt'))
		print(temp)

		if lock_candidates:
			assert lock_candidates == temp
		else:
			lock_candidates = temp

	pool = GenPool(16, 100, src, lock_candidates)
	best_gene, score = pool.get_best()
	
	print(best_gene)
	print(score)

	use_eval(os.path.join(src, '0.cpp'), best_gene)

	# Generate src files under ${datetime.now()} directory
	offset = datetime.now().isoformat()
	os.mkdir(offset)
	with open(os.path.join(src, '0.cpp'), 'r') as f:
		text = convert_to_multi_threaded_code(f, best_gene)
		with open(os.path.join(offset, src + '.cpp'), 'w') as f2:
			f2.write(''.join(text))


if __name__ == "__main__":
	# parser = argparse.ArgumentParser()
	# parser.add_argument("src", type=str)
	# args = parser.parse_args()

	# assert args.src

	# filelist = ['dining', 'easytest', 'global']
	filelist = ['global']
	for src in filelist:
		dirname = os.path.join('../../test', src)

		main_text = []
		function_text = ''
		header_text = ''
		for filename in os.listdir(dirname):
			full_filename = os.path.join(dirname, filename)
			ext = os.path.splitext(full_filename)[-1]
			if ext == '.h':
				with open(os.path.join(dirname, filename), 'r') as f:
					header_text = f.read()
			if ext == '.cpp':
				with open(os.path.join(dirname, filename), 'r') as f:
					text = f.read()
					headername = '#include "' + src + '.h"'
					idx = text.find(headername) + len(headername)
					if filename == src + '.cpp':
						function_text = text[idx:]
					else:
						main_text.append(text[idx:])

		if not os.path.exists(src):
			os.mkdir(src)
		main_num = len(main_text)
		for i in range(main_num):
			with open(os.path.join(src, str(i) + '.cpp'), 'w') as f:
				f.write(header_text + function_text + main_text[i])

		write_answer(src)