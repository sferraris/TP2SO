#include <scheduler.h>

typedef struct {
    void * rsp;
    int state;
}process;

process processList[100];
int curProcess = 0;
int totalProcess = 0;

void * schedule(void * rsp) {
    processList[curProcess++].rsp = rsp;
    if (curProcess == totalProcess)
        curProcess = 0;
    return processList[curProcess].rsp;
}

int searchPos() {
    int pos = 0;
    while (processList[pos].state == -1 && pos < 100)
        pos++;
    if (pos == 100)
        return -1;
    return pos;
}