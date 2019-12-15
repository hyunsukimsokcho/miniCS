import os
import subprocess

LOCK_START = "pthread_mutex_lock(&mutex_lock);\n"
LOCK_END = "pthread_mutex_unlock(&mutex_lock);\n"

def convert_to_multi_threaded_code(f, gene):
	global LOCK_END
	global LOCK_START
	lines = f.readlines()
	line_len = len(lines)
	ret = []
	prev = 0
	for i, tup in enumerate(gene):
		begin, end = tup
		while prev <= begin:
			ret.append(lines[prev])
			prev += 1
		ret.append(LOCK_START)

		while prev <= end:
			ret.append(lines[prev])
			prev += 1
		ret.append(LOCK_END)
	while prev < line_len:
		ret.append(lines[prev])
		prev += 1
	return ret

def convert_to_join_threaded_code(lines):
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
	with open(filename, 'r') as input_f:
		code_w_lock = convert_to_multi_threaded_code(input_f, lock_gene)
		code_w_join = convert_to_join_threaded_code(code_w_lock)
		with open('../Evaluate/test.cpp', 'w') as f:
		# with open(os.path.join(src, 'text.cpp'), 'w') as f:
			f.write(''.join(code_w_lock))
		with open('../Evaluate/test_join.cpp', 'w') as f:
		# with open(os.path.join(src, 'text_join.cpp'), 'w') as f:
			f.write(''.join(code_w_join))
		with open('../Evaluate/lock_range.txt', 'w') as f:
			for lock in lock_gene:
				f.write("{} {}\n".format(lock[0], lock[1]))
		with open('../Evaluate/race_set.txt', 'w') as f:
			pass
	
	proc = subprocess.Popen(['../Evaluate/evaluate'], cwd='../Evaluate/' ,stdout=subprocess.PIPE)
	out, err = proc.communicate()
	print("out : ")
	print(out.decode('utf-8'))

""" 
	gen is list of tuple which contains start point of lock and end point of lock. 
	ex) [(3,4), (5,7), (8,11)]
"""
def get_score(src, lock_gene):
	return len(lock_gene)
	
	# TODO
	for filename in os.listdir(src):
		if not os.path.splitext(filename)[0].isdigit():
			continue 
		full_filename = os.path.join(src, filename)
		use_eval(full_filename, lock_gene)

		