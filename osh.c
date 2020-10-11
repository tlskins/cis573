#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h> 
#include <sys/wait.h>

#define MAX_LINE 80 /* The maximum length command */

void removeSpaces(char *str) { 
    int count = 0; 
    for (int i = 0; str[i]; i++) 
        if (str[i] != ' ' && str[i] != '\n') 
            str[count++] = str[i];
    str[count] = '\0'; 
} 

int main(void) {
    char *args[MAX_LINE/2 + 1]; /* command line arguments */
    int shouldRun = 1; /* flag to determine when to exit program */
    char buf[MAX_LINE]; 
    int tokenCount = 0;
    int isBg = 0;

    while (shouldRun) {
        printf("osh>");
        fflush(stdout);
        fgets(buf, MAX_LINE, stdin);

        char *token = strtok(buf, " "); 
        while (token != NULL) { 
            if (token != "" && token != " ") {
                removeSpaces(token);
                args[tokenCount] = token;
                tokenCount++;
                token = strtok(NULL, " ");
            }
        }
        if (strcmp(args[tokenCount-1], "&") == 0) {
            memset(&args[tokenCount-1], 0, sizeof(args[tokenCount-1]));
            tokenCount--;
            isBg = 1;
        }
        args[tokenCount] = NULL;

        if (strcmp(args[0], "exit") == 0) {
            printf("exiting...\n");
            shouldRun = 0;
        }
        else if (sizeof(args[0]) > 0) {
            pid_t pid;
            int  status;
            pid = fork();

            if (pid < 0) {     
                printf("*** ERROR: forking child process failed\n");
                exit(0);
            }
            else if (pid == 0) { // child process
                if (execvp(args[0], args) < 0) {     
                    printf("*** ERROR: execvp failed\n");
                    exit(0);
                }
                exit(1);
            }
            else { // parent process
                if (!isBg) {
                    wait(&status);
                }
            }
        }

        tokenCount = 0;
        isBg = 0;
        memset(&args[0], 0, sizeof(args[0]));
    }

    return 0;
} 