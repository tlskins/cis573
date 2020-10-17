#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/fcntl.h>
#define MSG_SIZE 32

const char *pkt[] = {
    "Msg_1: Hello CIS370\n",
    "Msg_2: Hello CIS370\n",
    "Msg_3: Hello CIS370\n"
};

int main() {
    int bytesWritten, bytesRead, totalBytesWritten = 0, index;
    int pipeFD[2];
    char Buffer[MSG_SIZE];

    if (pipe(pipeFD) == -1) {
        perror("[-] pipe() failed!\n");
        exit(1);
    }
    bytesWritten = write(pipeFD[1], pkt[0], MSG_SIZE);
    printf("bytes written: %d\n", bytesWritten);
    bytesWritten = write(pipeFD[1], pkt[1], MSG_SIZE);
    printf("bytes written: %d\n", bytesWritten);
    bytesWritten = write(pipeFD[1], pkt[2], MSG_SIZE);
    printf("bytes written: %d\n", bytesWritten);

    for (index = 0; index < 3; index++) {
        bytesRead = read(pipeFD[0], Buffer, MSG_SIZE);
        // bytesRead = read(pipeFD[0], Buffer, 50);
        printf("%s (chars read: %d)\n", Buffer, bytesRead);
        printf("1234567890123456789012345678901234567890\n");
    }
    close(pipeFD[0]);
    return 1;
}