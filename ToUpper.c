#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h> 
#include <sys/wait.h>
#include <ctype.h>

#define MAX_LINE 80 /* The maximum length command */

void toUpperString(char *str) { 
    int count = 0; 
    for (int i = 0; str[i]; i++)
        str[i] = toupper(str[i]);
}

int main(void) {
    char *args[MAX_LINE/2 + 1]; /* command line arguments */
    int shouldRun = 1; /* flag to determine when to exit program */
    char buf[MAX_LINE]; 

    while (shouldRun) {
        printf(">");
        fflush(stdout);
        fgets(buf, MAX_LINE, stdin);

        if (strcmp(buf, "exit") == 0) {
            printf("exiting...\n");
            shouldRun = 0;
        }

        toUpperString(buf);

        printf("%s\n", buf);
    }

    return 0;
}