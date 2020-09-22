#include <scheduler.h>
#include <video_driver.h>//sacar
#include <time.h>
typedef struct {
    void * rsp;
    void * stackPos;
    int state;
}process;

process processList[PROCESSES];
int currentPid = 0;
int totalProcess = 0;
int shellCreated = 0;

void * schedule(void * rsp) {
         printHex(rsp);
        if ( shellCreated == 0 )
            shellCreated = 1;
        else{
            printString("holis");
            processList[currentPid].rsp = rsp;
            currentPid++;
        }
        //aca va algoritmo de prioridades
        while (processList[currentPid].state != READY ){//checkear que no se pase del rango
            currentPid++;
            if (currentPid >= PROCESSES){
                currentPid = 0;
            }
        }
        printHex(processList[currentPid].rsp);
        return processList[currentPid].rsp;
}

int searchPos() {
    int pos = 0;
    while ( processList[pos].state != KILLED && pos < PROCESSES)
        pos++;
    if (pos == PROCESSES)
        return -1;
    return pos;
}

void createProcess(void * rip) {
    uint64_t finalpos;
    uint64_t* stack = malloc(STACKSIZE);
    //if ( stack == -1); //checkear error
    finalpos = STACKSIZE;
    int pos = searchPos();
    //if (pos == -1)
        //chequear  error despues
    //createProcess_asm(finalpos,rip);
    int i = 1;
    stack[finalpos -i] = 0x0;
    i++;
    stack[finalpos -i] = stack + finalpos;
    i++;
    stack[finalpos -i] = 0x202;
    i++;
    stack[finalpos -i] = 0x8;
    i++;
    stack[finalpos -i] = rip;
    i++;
    stack[finalpos -i] = 1;
    i++;
    stack[finalpos -i] = 2;
    i++;
    stack[finalpos -i] = 3;
    i++;
    stack[finalpos -i] = 4;
    i++;
    stack[finalpos -i] = 5;
    i++;
    stack[finalpos -i] = 6;
    i++;
    stack[finalpos -i] = 7;
    i++;
    stack[finalpos -i] = 8;
    i++;
    stack[finalpos -i] = 9;
    i++;
    stack[finalpos -i] = 10;
    i++;
    stack[finalpos -i] = 11;
    i++;
    stack[finalpos -i] = 12;
    i++;
    stack[finalpos -i] = 13;
    i++;
    stack[finalpos -i] = 14;
    i++;
    stack[finalpos -i] = 15;
    

    processList[pos].state = READY;
    processList[pos].stackPos = stack;
    processList[pos].rsp = stack + STACKSIZE -i;

    totalProcess++;
}


int changeStatePid(int pid, int state){
    if ( processList[pid].state == KILLED)
        return -1;
    processList[pid].state = state;
    return 0;
}
void liberateResourcesPid(int pid){
    free(processList[pid].stackPos);
}
void changeState(int state){
    processList[currentPid].state = state;
}

void liberateResources(){
    free(processList[currentPid].stackPos);
}

int getPid(){
    return currentPid;
}

void printRandom(void* pos){
    printString("POSICION DE MEMORIA DEL INICIO DE SHELL DESDE KERNEL");
    printHex(pos);
}

void printRandomString(){
    printString("Llego");
}