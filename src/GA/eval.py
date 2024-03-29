import os
import subprocess

LOCK_START = "pthread_mutex_lock(&mutex_lock);\n"
LOCK_END = "pthread_mutex_unlock(&mutex_lock);\n"

def convert_to_multi_threaded_code(f, gene):
	"""
		Return code lines after inserting lock commands.
		`code lines` is a list of strings.

		Keyword arguments:
		f -- file object for source code
		gene -- list of locks
	"""
	global LOCK_END
	global LOCK_START
	lines = f.readlines()
	line_len = len(lines)
	ret = []
	prev = 0
	for i, tup in enumerate(gene):
		begin, end = tup
		while prev < begin:
			ret.append(lines[prev])
			prev += 1
		ret.append(LOCK_START)

		while prev < end:
			ret.append(lines[prev])
			prev += 1
		ret.append(LOCK_END)
	while prev < line_len:
		ret.append(lines[prev])
		prev += 1
	return ret

def convert_to_join_threaded_code(lines):
	"""
		Return code lines after inserting `pthread_join` right after `pthread_create`.
		`code lines` is a list of strings.

		Keyword arguments:
		lines -- `code lines` which is a returned value from `convert_to_multi_thread_code(f, gene)`
	"""
	ret = []
	keystr = 'pthread_create(&'
	keystr2 = 'pthread_join('
	keylen = len(keystr)

	for line in lines:
		if line.find(keystr2) >= 0:
			continue
		ret.append(line)
		idx = line.find(keystr)
		if idx >= 0:
			idx += keylen
			idx2 = line[idx:].find(',')
			thread_name = line[idx:idx+idx2]
			ret.append('pthread_join(' + thread_name + ', NULL);\n')
	return ret


	for i, tup in enumerate(gene):
		begin, end = tup
		global LOCK_END
		global LOCK_START
		lines.insert(2*i+begin, LOCK_START)
		lines.insert(2*i+end+1, LOCK_END)
	return ''.join(ret)

def use_eval(filename, lock_gene):
	"""
		Write input files necessary for `../Evaluate/evaluate.exe` to calculate data race # and instruction #.
		It writes `../Evaluate/test.cpp`, `../Evaluate/test_join.cpp` and `../Evaluate/lock_range.cpp`.
		Finally, it executes `../Evaluate/evaluate.exe` file.
	"""
	code_w_lock = []
	code_w_join = []
	with open(filename, 'r') as f:
		code_w_lock = convert_to_multi_threaded_code(f, lock_gene)
	code_w_join = convert_to_join_threaded_code(code_w_lock)
	with open('../Evaluate/test.cpp', 'w') as f:
		f.write(''.join(code_w_lock))
	with open('../Evaluate/test_join.cpp', 'w') as f:
		f.write(''.join(code_w_join))
	with open('../Evaluate/lock_range.txt', 'w') as f:
		offset = 1
		for lock in lock_gene:
			f.write("{} {}\n".format(lock[0] + offset, lock[1] + offset + 1))
			offset += 2
	
	subprocess.call(['../Evaluate/evaluate'], cwd='../Evaluate/')

def num_inst():
	"""
		Return the number of instructions
		Use `../Evaluate/num_ins.txt` file.
	"""
	with open('../Evaluate/num_ins.txt', 'r') as f:
		num = f.readline()
		return int(num)


def num_race_set():
	"""
		Return the number of data race.
		Use `../Evaluate/race_set.txt` file.
	"""
	race_set = set()

	with open('../Evaluate/race_set.txt', 'r') as f:
		lines = f.readlines()

		for line in lines:
			pos = line.find(' ')
			line1 = int(line[:pos])
			line2 = int(line[pos + 1:])

			race_set.add((min(line1, line2), max(line1, line2)))

	return len(race_set)

def get_score(src, lock_gene):
	"""
		Return score of given lock_gene.
		score is tuple with size 2 -- (Data race #, Instruction #)

		Keyword Arguments:
		lock_gene -- list of tuple which contains start point of lock and end point of lock. 
					ex) [(3,4), (5,7), (8,11)]
	"""
	inst = 0
	race_set = 0
	
	with open('../Evaluate/race_set.txt', 'w') as f:
		pass

	for filename in os.listdir(src):
		if not os.path.splitext(filename)[0].isdigit():
			continue 
		full_filename = os.path.join(src, filename)
		use_eval(full_filename, lock_gene)
		inst += num_inst()
	
	race_set = num_race_set()

	return (race_set, inst)
