#include <scheduler.h>

typedef struct {
    void * rsp;
    int state;
}process;

process processList[PROCESSES];
int curProcess = 0;
int totalProcess = 0;

void * schedule(void * rsp) {
    processList[curProcess++].rsp = rsp;
    //aca va algoritmo de prioridades
    while (processList[curProcess++].state != READY); 
    if (curProcess == totalProcess)
        curProcess = 0;
    return processList[curProcess].rsp;
}

int searchPos() {
    int pos = 0;
    while (processList[pos].state != KILLED && pos < PROCESSES)
        pos++;
    if (pos == PROCESSES)
        return -1;
    return pos;
}

void createProcess(void * rip) {
    int finalpos;
    void * stack = malloc(STACKSIZE);
    finalpos = stack + STACKSIZE;
    int pos = searchPos();
    //if (pos == -1)
        //chequear  error despues
    processList[pos].state = READY;
    curProcess = pos;
    createProcess_asm(finalpos,rip);
} 
