#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// Take filenmae as parameter, needs lock_list for script.py too
// 파라미터로 넘겨주셔도 좋고, 파일로 넘겨주셔도 좋습니다.
// 파일명 고정시 
int evaluate(char *filename)
{
	FILE *fp = NULL;
	char line[1024], cmd[1024];
	
	strcpy(cmd, "clang ");
	strcat(cmd, filename);
	//strcat(cmd, " -fsanitize=thread -fPIE -pie -g -o output");
	// Create file named output that prints out clang data race detection
	strcat(cmd, " -fsanitize=thread -O2 -g -o output");
	
	system(cmd);
	// Write detection output on out filee
	system("./output > out 2>&1");

	// gdb -q -x script.py 를 통해 machine instruction 개수 가져올 예정
	return 0;
	
	//clang 1.cpp -fsanitize=thread -fPIE -pie -g
}

int main()
{
	char inp[20];
	strcpy(inp, "test.cpp");
	evaluate(inp);
}
