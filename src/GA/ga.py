import os
import argparse
import math
import random
import numpy as np
import itertools
from copy import deepcopy
import re
from datetime import datetime


LOCK_START = "pthread_mutex_lock(&my_lock);\n"
LOCK_END = "pthread_mutex_unlock(&my_lock);\n"

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

def get_fitness_calculator(dist, cycle=True):
	# Unimplemented!
	return 0

def get_twopoint_gauss(length, sigma=10, rand_prob=0.1):
	start = random.randrange(0, length)
	end = start
	while end == start:
		if sigma and random.random() > rand_prob:
			end = int(random.gauss(start, sigma)) % length
		else:
			end = random.randrange(0, length) % length
	return start, end

def get_enclosing_sequence(ind, index, step=(-1, 1)):
	return [ ind[i%len(ind)] for i in range(index+step[0], index+step[1]+1) ]

def mutate_reverse(ind, dist_calculator, sigma=10):
	start, end = 0, -1
	while (start - 1)%len(ind) == end:
		start, end = get_twopoint_gauss(len(ind), sigma)
	start_l = (start - 1)%len(ind)
	end_r = (end + 1)%len(ind)
	original_distance = dist_calculator([ind[start_l], ind[start]]) + dist_calculator([ind[end], ind[end_r]])
	distance = dist_calculator([ind[start_l], ind[end]]) + dist_calculator([ind[start], ind[end_r]])
	if distance < original_distance:
		if start < end:
			sequence = ind[start:end+1]
		else:
			sequence = ind[start:] + ind[:end+1]
		sequence = list(reversed(sequence))
		if start < end:
			mutated = ind[:start] + sequence + ind[end+1:]
		else:
			mutated = ind[end+1:start] + sequence
		assert len(mutated) == len(ind)
		return mutated, original_distance - distance
	return None, 0

def mutate_unroll(ind, dist_calculator, delta=[4,5,6,7,8,9]):
	 start = random.randrange(len(ind))
	 end = (start + random.choice(delta))%len(ind)
	 if start < end:
		 sequence = ind[start+1:end]
	 else:
		 sequence = ind[start+1:] + ind[:end]
	 original_distance = dist_calculator([ind[start]] + sequence + [ind[end]])
	 min_distance = original_distance
	 shortest_sequence = None
	 for perm in itertools.permutations(sequence):
		 distance = dist_calculator([ind[start]] + list(perm) + [ind[end]])
		 if distance < min_distance:
			 min_distance = distance
			 shortest_sequence = list(perm)
	 if shortest_sequence:
		 if start < end:
			 mutated = ind[:start+1] + shortest_sequence + ind[end:]
		 else:
			 mutated = ind[end:start+1] + shortest_sequence
		 return mutated, original_distance - min_distance
	 return None, 0

def mutate_swap(ind, dist_calculator, sigma=10):
	start, end = get_twopoint_gauss(len(ind), sigma)
	original_distance = dist_calculator(get_enclosing_sequence(ind, start)) + dist_calculator(get_enclosing_sequence(ind, end))
	mutated = ind[:]
	mutated[start], mutated[end] = ind[end], ind[start]
	distance = dist_calculator(get_enclosing_sequence(mutated, start)) + dist_calculator(get_enclosing_sequence(mutated, end))
	if distance < original_distance:
		return mutated, original_distance - distance
	return None, 0

def random_gen(arr):
	prob = 0.35

	arr_len = len(arr)
	prob_arr = np.random.rand(arr_len)
	random.shuffle(arr)

	ret = []

	for i in range(arr_len):
		if prob_arr[i] < prob:
			is_ok = True
			for j in ret:
				if not ((j[1] <= arr[i][0]) or (arr[i][1] <= j[0])):
					is_ok = False
					break
			if is_ok:
				ret.append(arr[i])
	ret.sort()
	return ret

def convert_to_multi_threaded_code(src, gene):
	new_src = src.readlines()
	for i, tup in enumerate(gene):
		begin, end = tup
		global LOCK_END
		global LOCK_START
		new_src.insert(2*i+begin, LOCK_START)
		new_src.insert(2*i+end+1, LOCK_END)
	return ''.join(new_src)

if __name__ == "__main__":
	parser = argparse.ArgumentParser()
	parser.add_argument("script", type=str)
	parser.add_argument("src", type=str)
	args = parser.parse_args()

	assert args.script

	lock_candidates = read_script(args.script)
	offset = datetime.now().isoformat()
	os.mkdir(offset)
	lock_gene = random_gen(lock_candidates)
	src = open(args.src, 'r')
	print('lock_gene', lock_gene)
	new_src = convert_to_multi_threaded_code(src, lock_gene)
	with open(os.path.join(offset, '1'+'.txt'), 'w') as f:
		f.write(new_src)
	# Generate src files under ${datetime.now()} directory
	




