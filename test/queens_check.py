import subprocess
import re

# generate exec file
s = subprocess.check_call('gcc -o ./q queens.c -lpthread', shell = True) 
if s!=0:
    print("Error in making exec file")
else:
    print("Done in making exec file")

# run the exec flie
output = subprocess.getoutput('exec ./q')
outputs = output.split('\n')
print(output)