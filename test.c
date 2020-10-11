#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/shm.h> 
#include <sys/stat.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>

int printList(char **someList, char *ptr)
{
    int index = 0;
    while (someList[index] != NULL)
    {
        printf("%s\n", someList[index]);
        sprintf(ptr, "%s", someList[index]);
        ptr += strlen(someList[index]);
        index++;
    }
    return index;
}

int main(int argc, char **argv, char **envp)
{
    const char *name = "SharedFile";
    const int SIZE = 4096;
    pid_t ppid;
    int fd;
    char *ptr;

    fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, SIZE);
    ptr = (char *) mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    ppid = fork();
    if (ppid > 0) {
        wait(NULL);
    }
    printList(argv, ptr);
    printList(envp, ptr);
    if (ppid > 0) {
        shm_unlink(name);
        printf("[p] this is process %d and my parent is %d\n", getpid(), getppid());
    }
    else {
        printf("[c] this is process %d and my parent is %d\n", getpid(), getppid());
    }

    return EXIT_SUCCESS;
}