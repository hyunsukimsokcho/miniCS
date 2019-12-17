#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<string.h>
#include<list>

const int MAX_LEN = 511;
char buf[MAX_LEN + 1];
FILE *f, *out = fopen("race_set.txt", "a");

// false if file ends, else true
bool read_error()
{
    fgets(buf, MAX_LEN, f);
    if(strncmp(buf, "==================", 18) != 0) return false;

    int a, b;
    char *ptr;
    
    while(true)
	{
        fgets(buf, MAX_LEN, f);
        if(strncmp(buf, "==================", 18) == 0) break;
        
        if (strncmp(buf, "WARNING: ThreadSanitizer: data race", 35) == 0)
		{
			std::list<int> set1, set2;
    		std::list<int>::iterator it1, it2;
    		
            fgets(buf, MAX_LEN, f); // comment
            fgets(buf, MAX_LEN, f); // #0 ... #line
            
            while(buf[4] == '#' && strlen(buf) > 4)
            {
            	ptr = strstr(buf, "test.cpp:") + 9;
            	sscanf(ptr, "%d", &a);
            	set1.push_back(a);
            	fgets(buf, MAX_LEN, f);
			}
            
            fgets(buf, MAX_LEN, f); // comment
            fgets(buf, MAX_LEN, f); // #1 ... #line
            
            while(buf[4] == '#' && strlen(buf) > 4)
            {
            	ptr = strstr(buf, "test.cpp:") + 9;
            	sscanf(ptr, "%d", &b);
            	set2.push_back(b);
            	fgets(buf, MAX_LEN, f);
			}
			
            // Write all racing sets on "race_set.txt"
			for(it1 = set1.begin(); it1 != set1.end(); it1++)
            	for(it2 = set2.begin(); it2 != set2.end(); it2++)
            		fprintf(out, "%d %d\n", *it1, *it2);
        }

    }
    
    return true;
}

// Take filenmae as parameter, needs lock_list for script.py too
void data_race()
{	
	// Create race detection executable for test.cpp
	system("clang test.cpp -fsanitize=thread -O2 -g -o race_detect");
	
	// Write detection output on race.txt file
	system("./race_detect > race_detection.txt 2>&1");

	// Detects every race sets and append on "race.txt" file
	f = fopen("race_detection.txt","r");
	while (read_error());
	fclose(f);
	
	return;
}

// Count number of instructions between locks
int num_ins()
{
	// Compile test_join.cpp file
	// Static to avoid dynamic library link
	system("gcc -pthread -static -g test_join.cpp -o test_join_exe");
	system("gdb -q -batch -x gdb_script.py test_join_exe > /dev/null 2>&1");

	return 0;
}

int main()
{
	data_race();
	num_ins();
	
	return 0;
}
