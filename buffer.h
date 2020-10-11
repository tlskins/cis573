#define MAX_SIZE 8
#define PATH_NAME "."
#define PROJ_ID 'S'

typedef struct bufferStruct {
    int in;
    int out;
    int content[MAX_SIZE]; // will hold ASCII code of characters
}bufferStruct;

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