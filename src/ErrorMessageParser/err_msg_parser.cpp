#include "err_msg_parser.h"

#include <cstdio>
#include <string>
#include <cstring>
#include <cassert>

const int MAX_LEN = 511;
char buf[MAX_LEN + 1];
FILE *f;

bool read_error() {
    fgets(buf, MAX_LEN, f);
    if(strncmp(buf, "==================", 18) != 0) {
        return false;
    }

    int a,b;
    char *ptr;
    while(1) {
        fgets(buf, MAX_LEN, f);
        if(strncmp(buf, "==================", 18) == 0) {
            break;
        }
        if (strncmp(buf, "WARNING: ThreadSanitizer: data race", 35) == 0) {
            fgets(buf, MAX_LEN, f); // comment
            fgets(buf, MAX_LEN, f); // #0 ... #line
            assert(strstr(buf, "#0") != NULL);
            ptr = strstr(buf, ".cpp:") + 5;
            sscanf(ptr, "%d", &a);

            fgets(buf, MAX_LEN, f); // new line
            fgets(buf, MAX_LEN, f); // comment
            fgets(buf, MAX_LEN, f); // #1 ... #line
            assert(strstr(buf, "#0") != NULL);
            ptr = strstr(buf, ".cpp:") + 5;
            sscanf(ptr, "%d", &b);
            printf("(%d,%d)\n",a,b);
        }

    }
    return true;
}

void parse() {
    f = fopen("out.txt","r");
    while (read_error());
}

int main() {
    parse();
    return 0;
}