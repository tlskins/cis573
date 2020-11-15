#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#define MAX_LINE 50

void printHelp() {
    printf("reverse -f file1 file2\n");
    printf("reverse file1\n");
}
  
int main(int argc, char **argv) 
{ 
    char option[MAX_LINE];
    char file1[MAX_LINE];
    char file2[MAX_LINE];
    char c;
    int cnt = 0;
    int i;
    FILE *fp1;
    FILE *fp2;

    if (argc != 4 && argc != 2) {
        printHelp();
        exit(0); 
    }

    if (argc == 4) {
        strcpy(file1, argv[2]);
    } else {
        strcpy(file1, argv[1]);
    }

    fp1 = fopen(file1, "r"); 
    if (fp1 == NULL) { 
        puts("Could not open source file"); 
        exit(0); 
    }

    if (argc == 4) {
        strcpy(option, argv[1]);
        if (strcmp(option, "-f") != 0) {
            printHelp();
            exit(0);
        } else {
            strcpy(file2, argv[3]);
            fp2 = fopen(file2, "w");             
            if (fp2 == NULL) { 
                puts("Could not open destination files"); 
                exit(0); 
            }
        }
    }

    //moves the file pointer to the end.
    fseek(fp1,0,SEEK_END);
    //get the position of file pointer.
    cnt = ftell(fp1);
    
    while( i < cnt ) {
        i++;
        fseek(fp1,-i,SEEK_END);
        c = fgetc(fp1);

        if (argc == 4) {
            fputc(c, fp2);
        } else {
            printf("%c",c);
        }
    }

    fclose(fp1); 
    if (argc == 4) {
        fclose(fp2); 
    } else {
        printf("\n");
    }

    return 0; 
} 