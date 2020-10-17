#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h> 
#include <sys/wait.h>

#include <ctype.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct msgBuf {
    long msgType;
    char msgText[100];
};

key_t getKey(char *buf) {
    char* pend;
    key_t key;
    if ((key = ftok("msq", strtof(buf, &pend))) == -1) {
        perror("ftok");
        exit(1);
    }
    return key;
}

int getMsgId(key_t key, int isCreate) {
    int msgid;
    int perm = IPC_EXCL;
    if (isCreate) {
        perm = IPC_CREAT;
    }
    if ((msgid = msgget(key, 0666 | perm)) == -1) {
        perror("msgget");
        exit(1);
    }
    return msgid;
}

int main(int argc, char **argv) {
    int msgid;
    struct msgBuf msg;
    char* pend;
    char cmd[2];
    key_t key;

    if (argv[1] == NULL) {
        printf("Missing command. Valid options: -c | -s | -r | -d\n");
        exit(1);
    }

    for (int i = 0; i < 2; i++) {
        cmd[i] = tolower(argv[1][i]);
    }

    if (strcmp(cmd, "-c") == 0) {
        if (argv[2] == NULL) {
            printf("Invalid create command IE: -c/C <key>\n");
            exit(1);
        }
        key = getKey(argv[2]);
        msgid = getMsgId(key, 1);
    } else if (strcmp(cmd, "-s") == 0) {
        if (argv[2] == NULL || argv[3] == NULL || argv[4] == NULL) {
            printf("Invalid send command IE: -s/S <key> <type> <text>\n");
            exit(1);
        }
        key = getKey(argv[2]);
        msgid = getMsgId(key, 0);
        msg.msgType = strtof(argv[3], &pend);
        strcpy(msg.msgText, argv[4]);
        msgsnd(msgid, &msg, sizeof(msg), 0);
        printf("Msg sent (%ld): %s\n", msg.msgType, msg.msgText);
    } else if (strcmp(cmd, "-r") == 0) { 
        if (argv[2] == NULL || argv[3] == NULL) {
            printf("Invalid receive command IE: -r/R <key> <type>\n");
            exit(1);
        }
        key = getKey(argv[2]);
        msgid = getMsgId(key, 0);
        msgrcv(msgid, &msg, sizeof(msg), strtof(argv[3], &pend), IPC_NOWAIT);
        printf("Msg received (%ld): %s\n", msg.msgType, msg.msgText);
    } else if (strcmp(cmd, "-d") == 0) {
        if (argv[2] == NULL) {
            printf("Invalid delete command IE: -d/D <key> \n");
            exit(1);
        }
        key = getKey(argv[2]);
        msgid = getMsgId(key, 0);
        msgctl(msgid, IPC_RMID, NULL);
    }

    return 0;
} 