import subprocess
import re

# generate exec file
s = subprocess.check_call('g++ -o ./gs global_main1.cpp global.cpp -lpthread', shell = True) 
if s!=0:
    print("Error in making exec file")
else:
    print("Done in making exec file")
#subprocess.call('gcc -o ./easy_test2 easytest.c -lpthread')

# run the exec flie
output = subprocess.getoutput('exec ./gs')
outputs = output.split('\n')
print(output)