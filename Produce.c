#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <ctype.h>
#include <time.h>
#include <string.h>
#include "buffer.h"
#define MAX_LINE 15 /* The maximum length command */

void produce(bufferStruct *localPTR) {
    if (isFull(localPTR)) {
        printf("<Buffer full>\n");
        return;
    }
    localPTR->content[localPTR->in] = rand() % 100;
    localPTR->in = localPTR->in + 1;
    if (localPTR->in == MAX_SIZE) localPTR->in = 0;
}

void printBuffer(bufferStruct *localPTR) {
    printf("[");
    if (!isEmpty(localPTR)) {
        int idx = localPTR->out;
        while (idx != localPTR->in) {
            printf("%d ", localPTR->content[idx]);
            if (++idx == MAX_SIZE) {
                idx = 0;
            }
        }
    }
    printf("]\n");
}

void removeshm(int shmid) {
    shmctl(shmid, IPC_RMID, 0);
    printf("Shared memory segment marked for deletion\n");
}

int main() {
    key_t key;
    int shmid, cntr;
    int shouldRun = 1;
    bufferStruct buf;
    bufferStruct *localPTR;
    char cmdBuf[MAX_LINE]; 
    time_t t;
   
    /* Intializes random number generator */
    srand((unsigned) time(&t));

    key = ftok(PATH_NAME, PROJ_ID);
    if ((shmid = shmget(key, sizeof(&buf), IPC_CREAT|IPC_EXCL|0666)) == -1) {
        printf("Shared memory segment exists - opening as client\n");
        if ((shmid = shmget(key, sizeof(&buf), 0)) == -1) {
            perror("shmget");
            exit(1);
        }
    } else {
        printf("Creating new shared memory segment\n");
    }

    if ((localPTR = (bufferStruct *)shmat(shmid, 0, 0)) == (bufferStruct *)-1) {
        perror("shmat");
        exit(1);
    }

    while (shouldRun) {
        printf(">");
        fflush(stdout);
        fgets(cmdBuf, MAX_LINE, stdin); 

        switch (cmdBuf[0]) {
            case '0':
                shouldRun = 0;
                break;
            default:
                produce(localPTR);
                printBuffer(localPTR);
        }
    }

    return 0;
}