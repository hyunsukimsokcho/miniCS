import subprocess
import re

# generate exec file
s = subprocess.check_call('g++ -o ./easy_test easytest_main1.cpp easytest.cpp -lpthread', shell = True) 
if s!=0:
    print("Error in making exec file")
else:
    print("Done in making exec file")
#subprocess.call('gcc -o ./easy_test2 easytest.c -lpthread')

# run the exec flie
output = subprocess.getoutput('exec ./easy_test')
print(output)
outputs = output.split('\n')

p = re.compile('thread([1-2]): ([0-9])')
test = [-1] * 4
checkarr = []
# test 1: check the output is 10 lines
if len(outputs)==10:
    test[0] = True
else:
    test[0] = False
    test = [False] * 4
    print(test)
    quit()

# test2: check the output is digits 
for line in outputs:
    m = p.match(line)
    if m:
        checkarr.append((int(m.group(1)), int(m.group(2))))
    else:
        test = [True, False, False, False]
        print(test)
        quit()
test[1]= True

# test3: check there is no concurrent work
check = []
for th, n in checkarr:
    if n not in check:
        check.append(n)
    else:
        test[2]=False

if len(check)==10:
    test[2]= True
    
# test4: check the mutex is in right place by checking continuous
start_thr= checkarr[0][0]

for idx, p in enumerate(checkarr[1:]):
    if idx >= 4:
        if p[0] != 3- start_thr or checkarr[idx][1] != p[1] - 1:
            test[3]= False
            break
    else:
        if p[0]!= start_thr or checkarr[idx][1] != p[1] - 1:
            test[3]= False
            break

if test[3]== -1:
    test[3]=True

print(test)
