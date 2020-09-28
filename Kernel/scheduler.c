#include <scheduler.h>

typedef struct {
    char * name;
    void * rsp;
    void * stackPos;
    int state;
    int priority;
    int foreground;
}process;

typedef int processMat[PRIORITIES][PROCESSES]; //const
processMat processLists[2];
process allProcesses[PROCESSES];

int currentPid = 0;
int totalProcess = 0;
int shellCreated = 0;
int startflag = 0;

int currentPri = 0;
int currentPos = 0;
int currentList = 0;
int timeCycle = PRIORITIES - DEFAULTPRI - 1; //const

void * schedule(void * rsp) {
    if ( startflag == 0)
        return rsp;
    if ( shellCreated == 0 )
        shellCreated = 1;
    else
        allProcesses[currentPid].rsp = rsp;
    currentPid = chooseProcess();
    return allProcesses[currentPid].rsp;
}

int chooseProcess() {
    if (timeCycle <= 0) {
        currentPos++;
        while (processLists[currentList][currentPri][currentPos] == 0 ||
               allProcesses[processLists[currentList][currentPri][currentPos] - 1].state != READY) { //falta ++ en pos o timer
            if ( allProcesses[processLists[currentList][currentPri][currentPos] - 1].state == BLOCKED) {
                insertProcess(processLists[currentList][currentPri][currentPos] - 1, currentPri);
                processLists[currentList][currentPri][currentPos] = 0;
            }
            if (currentPos == PROCESSES - 1) {
                currentPos = 0; //chequear tumba
                currentPri++;
                if (currentPri == PRIORITIES) { //const
                    swapList();
                    currentPri = 0;
                    return chooseProcess();
                }
            } else
                currentPos++;
        }
        insertProcess(processLists[currentList][currentPri][currentPos]-1, currentPri);
        processLists[currentList][currentPri][currentPos] = 0;
        timeCycle = PRIORITIES - currentPri - 1; //constante

        return processLists[1-currentList][currentPri][currentPos] - 1;
    }
    timeCycle--;
    return currentPid;
}

void swapList() {
    currentList = 1 - currentList;
}

void insertProcess(int pid, int pri) {
    int listaux = currentList;
    if (pri <= currentPri)
        listaux = 1 - currentList;
    int pos = 0;
    while (processLists[listaux][pri][pos] != 0 && pos < PROCESSES)
        pos++;
    processLists[listaux][pri][pos] = pid + 1;
}

int searchPos() {
    int pos = 0;
    while (allProcesses[pos].state != KILLED && pos < PROCESSES)
        pos++;
    if (pos == PROCESSES)
        return - 1;
    return pos;
}

int createProcess(int argc, char * argv[]) { //rip, name, foreground
    if (totalProcess == PROCESSES)
        return;
    uint64_t finalpos;
    uint64_t* stack = malloc(STACKSIZE);
    //if ( stack == -1); //checkear error
    finalpos = STACKSIZE;
    int pos = searchPos();
    //if (pos == -1); //chequear  error despues
    int i = 1;
    stack[finalpos -i] = 0x0;
    i++;
    stack[finalpos -i] = stack + finalpos;
    i++;
    stack[finalpos -i] = 0x202;
    i++;
    stack[finalpos -i] = 0x8;
    i++;
    stack[finalpos -i] = (void *) argv[0];
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

    allProcesses[pos].state = READY;
    allProcesses[pos].stackPos = stack;
    allProcesses[pos].rsp = stack + STACKSIZE -i;
    allProcesses[pos].priority = DEFAULTPRI; //parametro
    allProcesses[pos].foreground = (int) argv[2]; //parametro
    allProcesses[pos].name = argv[1];

    insertProcess(pos, DEFAULTPRI); //cambiar con parametro

    totalProcess++;
    if (startflag == 0) {
        startflag = 1;
        _hlt();
    }
    return pos;
}

void liberateResourcesPid(int pid) {
    free(allProcesses[pid].stackPos);
    totalProcess--;
}

int changeStatePid(int pid, int state) {
    if ( allProcesses[pid].state == KILLED){
        return -1;
    }
    if (state == BLOCKED) {
        if (allProcesses[pid].state == BLOCKED)
            allProcesses[pid].state = READY;
        else
            allProcesses[pid].state = BLOCKED;
    }
    else
        allProcesses[pid].state = state;
    if(state == KILLED)
        liberateResourcesPid(pid);
    return 0;
}


void liberateResources() {
    free(allProcesses[currentPid].stackPos);
    totalProcess--;
}

void changeState(int state) {
    allProcesses[currentPid].state = state;
    if(state == KILLED)
        liberateResources();
}


int getPid() {
    return currentPid;
}

void printRandom(void* pos) {
    printString("Prueba: ");
    printHex(pos);
}

void printRandomString() {
    printString("Llego");
}

void blockProcess(int pid) {
    allProcesses[pid].state = BLOCKED;
}

void unblockProcess(int pid) {
    allProcesses[pid].state = READY;
}

void removeProcess(int pid,int pri) {
    int aux=0;
    while ( processLists[currentList][pri][aux] != (pid + 1) ) {
        aux++;
    }
    processLists[currentList][pri][aux] = 0;
}

void nice(int pid,int pri) {
    if (allProcesses[pid].state != KILLED) {
        if ( pri >= allProcesses[pid].priority && pri >= currentPri)
            removeProcess(pid,allProcesses[pid].priority);
        insertProcess(pid,pri);
        allProcesses[pid].priority = pri;
    }
}

char * listProcesses() {
    int j=0;
    char ret[PROCESSES*100] = {0}, auxID[10], auxPri[10], auxRSP[10], auxRBP[10];
    for (int pid=0; pid < PROCESSES && j < totalProcess;pid++) {
        if (allProcesses[pid].state != KILLED) {
            strcpy(auxID, dectostr(pid));
            strcpy(auxPri, dectostr(allProcesses[pid].priority));
            strcpy(auxRSP, hextostr(allProcesses[pid].rsp));
            strcpy(auxRBP, hextostr(allProcesses[pid].stackPos + STACKSIZE));
            char *aux[] = {"Nombre: ", allProcesses[pid].name, "\t\t",
                           "ID: ", auxID, "\t\t",
                           "State: ", (allProcesses[pid].state == 1) ? "BLOCKED" : "READY", "\t\t",
                           "Priority: ", auxPri, "\t\t",
                           "Foreground: ", (allProcesses[pid].foreground == 1) ? "Foreground" : "Background", "\t\t",
                           "RSP: ", "0x", auxRSP, "\t\t",
                           "RBP: ", "0x", auxRBP, "\n"};
            strcat(ret, aux, 23);
            j++;
        }
    }
    return ret;
}