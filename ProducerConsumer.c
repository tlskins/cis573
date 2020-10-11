#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <ctype.h>
#include <time.h>
#include <string.h>
#define MAX_SIZE 8
#define MAX_LINE 15 /* The maximum length command */

typedef struct bufferStruct {
    int in;
    int out;
    int content[MAX_SIZE];// will hold items produced (single int)
} bufferStruct; 

int bufferNextIn(bufferStruct *buf) {
    if (buf->in + 1 == MAX_SIZE) {
        return 0;
    }
    return buf->in + 1;
}

int isEmpty(bufferStruct *buf) {
    if (buf->in == buf->out) {
        return 1;
    }
    return 0;
}

int isFull(bufferStruct *buf) {
    if (bufferNextIn(buf) == buf->out) {
        return 1;
    }
    return 0;
}

void produce(bufferStruct *localPTR) {
    if (isFull(localPTR)) {
        printf("Buffer is full\n");
        return;
    }
    localPTR->content[localPTR->in] = rand() % 100;
    localPTR->in = localPTR->in + 1;
    if (localPTR->in == MAX_SIZE) localPTR->in = 0;
    printf("Produced...\n");
}

void consume(bufferStruct *localPTR) {
    if (isEmpty(localPTR)) {
        printf("Buffer is empty\n");
        return;
    }
    localPTR->out = localPTR->out + 1;
    if (localPTR->out == MAX_SIZE) localPTR->out = 0;
    printf("Consumed...\n");
}

void readshm(bufferStruct *localPTR) {
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

void usage() {
    fprintf(stderr, "p -> produce | c -> consume | r -> read | d -> delete | q -> quit \n");
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

    key = ftok(".", 'S');
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
        printf("entered: %s\n", cmdBuf); 

        switch (cmdBuf[0]) {
            case 'p':
                produce(localPTR);
                readshm(localPTR);
                break;
            case 'c':
                consume(localPTR);
                readshm(localPTR);
                break;
            case 'r':
                readshm(localPTR);
                break;
            case 'd':
                removeshm(shmid);
                break;
            case 'q':
                shouldRun = 0;
                break;
            default: usage();
        }
    }

    return 0;
}