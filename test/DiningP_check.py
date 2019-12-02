'''
for dining Philosopher
9 tests
'''

import subprocess
import re
import os, time
from threading import Timer
from datetime import datetime

test = [False] * 9

def exit_func():
    print("Dead lock ERROR")
    test[0]=False
    print(test)
    os._exit(0)



# return deadlock error when running time is over 8 seconds.
Timer(8, exit_func).start()

# generate exec file
s = subprocess.check_call('gcc -o ./dp DiningP.c -lpthread', shell = True) 

# test 1: dead lock test
if s!=0:
    print("Error in making exec file")
    print(test)
    quit()
else:
    print("Done in making exec file")
    test[0]=True

# run the exec flie
output = subprocess.getoutput('exec ./dp')
outputs = output.split('\n')


for line in outputs:
    print(line)

p = re.compile('philosopher ([1-5]): (eating|get chopstick|put chopstick)')
p2 = re.compile('Done with chopsticks array: (\d) (\d) (\d) (\d) (\d)')
p3  =re.compile('ERROR in philosopher (\d)')


# test 2: check the output is 16 lines
if len(outputs)==16:
    test[1] = True
else:
    test[1] = False
    
# test4: check the final output  
m = p2.match(outputs[-1])
if m:
    test[2] = True
    test[3] = True
    for i in range(1,6):
        if int(m.group(i)) !=0:
            test[3]=False
            break

else:
    # test3: check the final output
    test[2] = False
    test[3] = False

# test4-9: check there is no concurrent work
ids = []
for line in outputs[:-1]:
    m2 = p3.match(line)
    if m2:
        id = int(m2.group(1))
        ids.append(id)

for id in range(1,6):
    if id not in ids:
        test[id+3] = True

print(test)
os._exit(0)
