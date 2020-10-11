#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <ctype.h>
#include <string.h>
#define SEGSIZE 100

void writeshm(int shmid, char *localPTR, char *text) {
    strcpy(localPTR, text);
    printf("Done...\n");
}

void readshm(int shmid, char *localPTR) {
    printf("localPtr: %s\n", localPTR);
}

void removeshm(int shmid) {
    shmctl(shmid, IPC_RMID, 0);
    printf("Shared memory segment marked for deletion\n");
}

void changemode(int shmid, char *mode) {
    struct shmid_ds myshmds;
    shmctl(shmid, IPC_STAT, &myshmds);
    printf("Old permissions were: %o\n", myshmds.shm_perm.mode);
    sscan(mode, "%o", &myshmds.shm_perm.mode);
    shmctl(shmid, IPC_SET, &myshmds);
    printf("New permissions are : %o\n", myshmds.shm_perm.mode);
}

void usage() {
    fprintf(stderr, "shmtool");
    exit(1);
}

int main(int argc, char *argv[]) {
    key_t key;
    int shmid, cntr;
    char *localPTR;

    if (argc == 1) usage();
    key = ftok(".", 'S');
    if ((shmid = shmget(key, SEGSIZE, IPC_CREAT|IPC_EXCL|0666)) == -1) {
        printf("Shared memory segment exists - opening as client\n");
        if ((shmid = shmget(key, SEGSIZE, 0)) == -1) {
            perror("shmget");
            exit(1);
        }
    } else {
        printf("Creating new shared memory segment\n");
    }

    if ((localPTR = (char *)shmat(shmid, 0, 0)) == (char *)-1) {
        perror("shmat");
        exit(1);
    }

    switch (tolower(argv[1][0])) {
        case 'w': writeshm(shmid, localPTR, argv[2]);
            break;
        case 'r': readshm(shmid, localPTR);
            break;
        case 'm': changemode(shmid, argv[2]);
            break;
        default: usage();
    }
}