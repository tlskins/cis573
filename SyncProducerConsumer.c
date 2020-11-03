#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define BUFFER_SIZE 5
#define MAX_LINE 5

pthread_t tid[2];
pthread_mutex_t lock;
sem_t full;
sem_t empty;

int n;
int in = 0;
int out = 0;
int buffer[BUFFER_SIZE];

void *produce(void *vargp) {
    int item, filled;
    for (int i=0;i<n;i++) {
        sem_wait(&empty);
        pthread_mutex_lock(&lock);

        item = rand() % 100;
        buffer[in] = item;
        in = (in+1) % BUFFER_SIZE;

        filled = (in-out) == 0 ? BUFFER_SIZE : in - out;
        if (filled < 0) filled += BUFFER_SIZE;
        printf("produced %d, %d/%d filled\n", item, filled, BUFFER_SIZE);

        pthread_mutex_unlock(&lock);
        sem_post(&full);
    }
    printf("done producing\n");

    pthread_exit(NULL);
}

void *consume(void *vargp) {
    int item, filled;
    for (int i=0;i<n;i++) {
        sem_wait(&full);
        pthread_mutex_lock(&lock);

        item = buffer[out];
        out = (out+1) % BUFFER_SIZE;

        filled = (in-out) == 0 ? 0 : in - out;
        if (filled < 0) filled += BUFFER_SIZE;
        printf("consumed %d, %d/%d filled\n", item, filled, BUFFER_SIZE);

        pthread_mutex_unlock(&lock);
        sem_post(&empty);
    }
    printf("done consuming\n");

    pthread_exit(NULL);
}

int main(void) {
    int err;
    int p = 1;
    int c = 2;
    char cmdLine[MAX_LINE];

    printf("# To Produce -> ");
    fflush(stdout);
    fgets(cmdLine, MAX_LINE, stdin);
    n = atoi(cmdLine);

    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("\n mutex init failed\n");
        return 1;
    }

    sem_init(&full, 0, 0);
    sem_init(&empty, 0, BUFFER_SIZE);

    if (pthread_create(&(tid[0]), NULL, &produce, (void *)&p) != 0)
        printf("\nproducer can't create thread :[%s]", strerror(err));
    if (pthread_create(&(tid[1]), NULL, &consume, (void *)&c) != 0)
        printf("\ncconsumer an't create thread :[%s]", strerror(err));

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_mutex_destroy(&lock);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}