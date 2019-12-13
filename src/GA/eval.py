import os
from datetime import datetime

LOCK_START = "pthread_mutex_lock(&my_lock);\n"
LOCK_END = "pthread_mutex_unlock(&my_lock);\n"
cnt = 0

def convert_to_multi_threaded_code(src, gene):
	new_src = src.readlines()
	for i, tup in enumerate(gene):
		begin, end = tup
		global LOCK_END
		global LOCK_START
		new_src.insert(2*i+begin, LOCK_START)
		new_src.insert(2*i+end+1, LOCK_END)
	return ''.join(new_src)

""" 
	gen is list of tuple which contains start point of lock and end point of lock. 
	ex) [(3,4), (5,7), (8,11)]
"""
def get_score(src_file, offset, lock_gene):
	# TODO
	src = open(src_file, 'r')
	new_src = convert_to_multi_threaded_code(src, lock_gene)
	src.close()

	global cnt
	with open(os.path.join(offset, '1'+'.txt'), 'w') as f:
		f.write(new_src)

	return len(lock_gene)