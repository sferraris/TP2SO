#include <scheduler.h>
#include <video_driver.h>//sacar
#include <time.h>
typedef struct {
    //char * name;
    //int priority;
    void * rsp;
    void * stackPos;
    int state;
    //int foreground;
}process;

process processList[PROCESSES];
int currentPid = 0;
int totalProcess = 0;
int shellCreated = 0;
int startflag = 0;
void * schedule(void * rsp) {
    if ( startflag == 0)
        return rsp;
    if ( shellCreated == 0 ){
        shellCreated = 1;
    }
    else{
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
    stack[finalpos -i] = 0;
    i++;
    stack[finalpos -i] = 0;
    i++;
    stack[finalpos -i] = 0;
    i++;
    stack[finalpos -i] = 0;
    i++;
    stack[finalpos -i] = 0;
    i++;
    stack[finalpos -i] = 0;
    i++;
    stack[finalpos -i] = 0;
    i++;
    stack[finalpos -i] = 0;
    i++;
    stack[finalpos -i] = 0;
    i++;
    stack[finalpos -i] = 0;
    i++;
    stack[finalpos -i] = 0;
    i++;
    stack[finalpos -i] = 0;
    i++;
    stack[finalpos -i] = 0;
    i++;
    stack[finalpos -i] = 0;
    i++;
    stack[finalpos -i] = 0;
    

    processList[pos].state = READY;
    processList[pos].stackPos = stack;
    processList[pos].rsp = stack + STACKSIZE -i;

    totalProcess++;
    startflag = 1;
    _hlt();
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

void blockProcess(int pid) {
    processList[pid].state = BLOCKED;
}

void unblockProcess(int pid) {
    processList[pid].state = READY;
}


void liberateResources(){
    free(processList[currentPid].stackPos);
}

int getPid(){
    return currentPid;
}

void printRandom(void* pos){
    printString("Prueba: ");
    printHex(pos);
}

void printRandomString(){
    printString("Llego");
}

void listProcesses() {
    int j=0;
    for (int pid=0; pid < PROCESSES && j < totalProcess;pid++) {
        if (processList[pid].state != KILLED) {
            j++;
            printString("Nombre = "); //printString(processList[pid].name); //Agregar nombre
            printChar('\n');
            printString("ID = "); printDec(pid);
            printChar('\n');
            printString("Priority = "); //printDec(processList[pid].priority);//Agregar priority
            printChar('\n');
            printString("RSP = "); printHex(processList[pid].rsp);
            printChar('\n');
            printString("RBP = "); printHex(processList[pid].stackPos + STACKSIZE);
            printChar('\n');
           // printString("Foreground = "); printDec(processList[pid].foreground);
            //printChar('\n');
        }
    }
}
