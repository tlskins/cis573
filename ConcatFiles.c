#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#define MAX_LINE 50
  
int main(int argc, char **argv) 
{ 
    char option[MAX_LINE];
    char file1[MAX_LINE];
    char file2[MAX_LINE];
    char file3[MAX_LINE];

    if (argc < 5) {
        printf("Error! Usage: MyCat <s,e> file1 file2 file3\n");
        exit(0); 
    }

    strcpy(option, argv[1]);
    strcpy(file1, argv[2]);
    strcpy(file2, argv[3]);
    strcpy(file3, argv[4]);

    printf("%s %s %s %s\n", option, file1, file2, file3);

    FILE *fp1 = fopen(file1, "r"); 
    FILE *fp2 = fopen(file2, "r"); 
    FILE *fp3 = fopen(file3, "w"); 
    char c; 
    
    if (fp1 == NULL || fp2 == NULL || fp3 == NULL) { 
            puts("Could not open files"); 
            exit(0); 
    }

    FILE *start, *end;

    int isEnd = 0;
    if (strcmp(option, "-s") == 0) {
        isEnd = -1;
        start = fp1;
        end = fp2;
    }
    if (strcmp(option, "-e") == 0) {
        isEnd = 1;
        start = fp2;
        end = fp1;
    }
    if (isEnd == 0) {
        printf("Error! Usage: you should use either '-s' or '-e' then the file names");
        exit(0); 
    }
    
    while ((c = fgetc(start)) != EOF) 
        fputc(c, fp3); 
    
    while ((c = fgetc(end)) != EOF) 
        fputc(c, fp3); 
    
    fclose(fp1); 
    fclose(fp2); 
    fclose(fp3);

    return 0; 
} 