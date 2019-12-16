import gdb
import sys


# Get what line will be executed next
# If next line is not in test_join.cpp, return -1
def find_line(str):
	pos = str.find('at test_join.cpp:')
	if pos != -1:
		str = str[pos + 17:]
		pos = str.find('\n')
		pos = int(str[:pos])

	return pos
		

f = open("lock_range.txt", "r")
lock_list = {}

while True:
	line = f.readline()
	if not line: break
	pos = line.find(' ')
	
	start = int(line[:pos])
	end = int(line[pos + 1:])
	lock_list[start] = end

gdb.execute('set pagination off')

find_str = 'at test_join.cpp:'
count_num = 0

# Insert break point for every lock starting point
break_stmt = 'break {}'
for start in lock_list.keys():
	gdb.execute(break_stmt.format(start))

gdb.execute('run')

while True:
	response = gdb.execute('frame', to_string = True)
	start = find_line(response)
	end = lock_list[start]
	unlocking = False
		
	while True:
		count_num += 1
		gdb.execute('stepi')
		response = gdb.execute('frame', to_string = True)
		next = find_line(response)
			
		if unlocking and next != end and next != -1:
			with open("num_ins.txt", "w") as f:
				f.write(str(count_num))
			break
		elif not unlocking and next == end:
			unlocking = True

	gdb.execute('continue')            
