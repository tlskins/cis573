#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h> 
#include <sys/wait.h>

#define MAX_LINE 128
#define READ_END 0
#define WRITE_END 1

void removeSpaces(char *str) { 
    int count = 0; 
    for (int i = 0; str[i]; i++) 
        if (str[i] != ' ' && str[i] != '\n') 
            str[count++] = str[i];
    str[count] = '\0'; 
} 

int main(int argc, char **argv) {
    char cmdLine[MAX_LINE];
    char fname[MAX_LINE];
    char outBuffer[MAX_LINE];
    int index = 0;
    int sd, fd;

    strcpy(fname, argv[1]);
    sd = dup(1);
    fd = open(fname, O_CREAT | O_WRONLY, 0666);

    if (fd < 0) {
        fprintf(stderr, "Err opening file\n");
        return 1;
    }

    while (1) {
        sprintf(outBuffer, "Index is %d\n", ++index);
        fflush(stdout);
        fgets(cmdLine, MAX_LINE, stdin);
        removeSpaces(cmdLine);

        if (strcmp(cmdLine, "f") == 0) {
            dup2(fd, 1);
        } else if (strcmp(cmdLine, "s") == 0) {
            dup2(sd, 1);
        } else if (strcmp(cmdLine, "0") == 0) {
            return 0;
        }

        write(1, outBuffer, strlen(outBuffer));
    }
} 