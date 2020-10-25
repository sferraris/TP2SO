#include <scheduler.h>

typedef struct {
    int waitingForChar;
    char * name;
    void * rsp;
    void * stackPos;
    int state;
    int priority;
    int foreground;
    int fd[4];
    int waiting;
}process;

typedef int processMat[PRIORITIES][PROCESSES]; //const
processMat processLists[2];
process allProcesses[PROCESSES];

int currentPid = 1;
int totalProcess = 0;
int shellCreated = 0;
int startflag = 0;
int auxProcessCreated = 0;
int currentPri = 0;
int currentPos = 0;
int currentList = 0;
int timeCycle = PRIORITIES - DEFAULTPRI - 1; //const
int foregroundFlag = 0;
int foregroundProcesses[FOREGROUNDPROCESSES];
int foregroundIndex = -1;

void * schedule(void * rsp) {
    if ( startflag == 0)
        return rsp;
    if ( shellCreated == 0 )
        shellCreated = 1;
    else
        allProcesses[currentPid].rsp = rsp;
    if (foregroundFlag == 0)
        currentPid = chooseProcess();
    else {
        changeStatePid(foregroundProcesses[foregroundIndex], READY);
        currentPid = foregroundProcesses[foregroundIndex];
        insertProcess(processLists[currentList][allProcesses[currentPid].priority][currentPid], allProcesses[currentPid].priority);
        processLists[currentList][allProcesses[currentPid].priority][currentPid] = 0;
        foregroundFlag = 0;
    }
    return allProcesses[currentPid].rsp;
}

void waitForChar(int pid) {
    allProcesses[pid].waitingForChar = 1;
    changeStatePid(pid, BLOCKED);
}

void detectChar() {
    if (allProcesses[foregroundProcesses[foregroundIndex]].waitingForChar == 1) {
        foregroundFlag = 1;
        allProcesses[foregroundProcesses[foregroundIndex]].waitingForChar = 0;
    }
}

void auxProcess() {
    while(1) {
        _hlt();
    }
}

void createAuxProcess() {
    uint64_t finalpos;
    uint64_t* stack = malloc(STACKSIZE);
    if (stack == (void*) 0)
        return -1;
    finalpos = STACKSIZE;
    int pos = 0;
    int i = 1;
    stack[finalpos -i] = 0x0;
    i++;
    stack[finalpos -i] = stack + finalpos;
    i++;
    stack[finalpos -i] = 0x202;
    i++;
    stack[finalpos -i] = 0x8;
    i++;
    stack[finalpos -i] = auxProcess;
    i++;
    stack[finalpos -i] = 0;
    i++;
    stack[finalpos -i] = 0;
    i++;
    stack[finalpos -i] = 0; //rcx
    i++;
    stack[finalpos -i] = 0; //rdx
    i++;
    stack[finalpos -i] = 0;
    i++;
    stack[finalpos -i] = 0; //rsi
    i++;
    stack[finalpos -i] = 0; //rdi
    i++;
    stack[finalpos -i] = 0; //r8
    i++;
    stack[finalpos -i] = 0; //r9
    i++;
    stack[finalpos -i] = 0; //10
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
    allProcesses[pos].state = KILLED;
    allProcesses[pos].stackPos = stack;
    allProcesses[pos].rsp = stack + STACKSIZE -i;
    auxProcessCreated = 1;
}

int chooseProcess() {
    int flag = 0;
    if (timeCycle <= 0) {
        currentPos++;
        while (processLists[currentList][currentPri][currentPos] == 0 ||
               allProcesses[processLists[currentList][currentPri][currentPos]].state != READY) {
            if ( allProcesses[processLists[currentList][currentPri][currentPos]].state == BLOCKED) {
                insertProcess(processLists[currentList][currentPri][currentPos], currentPri);
                processLists[currentList][currentPri][currentPos] = 0;
            }
            if (currentPos == PROCESSES - 1) {
                currentPos = 0; //chequear tumba
                currentPri++;
                if (currentPri == PRIORITIES) { //const
                    flag++;
                    swapList();
                    currentPri = 0;
                    currentPos = 0;
                    if (flag == 2) {
                        if (auxProcessCreated == 0)
                            createAuxProcess();
                        return 0;
                    }
                }
            } else
                currentPos++;
        }
        insertProcess(processLists[currentList][currentPri][currentPos], currentPri);
        processLists[currentList][currentPri][currentPos] = 0;
        timeCycle = PRIORITIES - currentPri - 1; //constante

        return processLists[1-currentList][currentPri][currentPos];
    }
    timeCycle--;

    return currentPid;
}

void swapList() {
    currentList = 1 - currentList;
}

void insertProcess(int pid, int pri) {
    processLists[1 - currentList][pri][pid] = pid;
}

int searchPos() {
    int pos = 1;
    while (allProcesses[pos].state != KILLED && pos < PROCESSES)
        pos++;
    if (pos == PROCESSES)
        return - 1;
    return pos;
}

int updateForegroundList(){
    if (foregroundIndex == -1)
        return -1;
    int i, j=0;
    for ( i = 0; i < FOREGROUNDPROCESSES; i++){
        if ( foregroundProcesses[i] != 0 )
            foregroundProcesses[j++] = foregroundProcesses[i];
    }
    i = j;
    while ( i < FOREGROUNDPROCESSES){
        foregroundProcesses[i++] = 0;
    }
    return j - 1;
}

int createProcess(int argc, char * argv[]) { //rip, name, foreground, input, output mas argumentos
    if (totalProcess == PROCESSES)
        return -1;
    if ( argv[2] == 1 && foregroundIndex + 1 == FOREGROUNDPROCESSES)
        return -1;
    uint64_t finalpos;
    uint64_t* stack = malloc(STACKSIZE);
    if ( stack == (void*) 0)
        return -1;
    finalpos = STACKSIZE;
    int pos = searchPos();
    if (pos == -1){
        free(stack);
        return -1;
    }
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
    stack[finalpos -i] = ( (argc > 8)? argv[8] : 0); //rcx
    i++;
    stack[finalpos -i] = ( (argc > 7)? argv[7] : 0); //rdx
    i++;
    stack[finalpos -i] = 0;
    i++;
    stack[finalpos -i] = ( (argc > 5)? argv[5] : 0); //rsi
    i++;
    stack[finalpos -i] = ( (argc > 6)? argv[6] : 0); //rdi
    i++;
    stack[finalpos -i] = ( (argc > 9)? argv[9] : 0); //r8
    i++;
    stack[finalpos -i] = ( (argc > 10)? argv[10] : 0); //r9
    i++;
    stack[finalpos -i] = 0; //10
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
    allProcesses[pos].fd[0] = 0;
    allProcesses[pos].fd[1] = 1;
    allProcesses[pos].fd[2] = argv[3];
    allProcesses[pos].fd[3] = argv[4];
    allProcesses[pos].state = READY;
    allProcesses[pos].waiting = 0;
    allProcesses[pos].waitingForChar = 0;
    insertProcess(pos, DEFAULTPRI); //cambiar con parametro
    totalProcess++;
    //setear procesos foreground a la lista de foregrounds
    if (allProcesses[pos].foreground == 1){
        foregroundIndex = updateForegroundList();
        foregroundFlag = 0;
        foregroundIndex++;
        foregroundProcesses[foregroundIndex] = pos;
        if (foregroundIndex > 0) {
            //printDec(foregroundProcesses[foregroundIndex]);
            //printDec(allProcesses[foregroundProcesses[foregroundIndex]].state);
            changeStatePid(foregroundProcesses[foregroundIndex-1], BLOCKED);
        }
    }
    if (startflag == 0) {
        startflag = 1;
        _hlt();
    }
    return pos;
}

int foregroundPos(int pid){
    for ( int i = 0; i < FOREGROUNDPROCESSES; i++){
        if (foregroundProcesses[i] == pid){
            return i;
        }
    }
    return -1;
}

void liberateResourcesPid(int pid) {
    if (allProcesses[pid].foreground == 1 ){
        int pos = foregroundPos(pid);
        foregroundProcesses[pos] = 0;
        foregroundIndex = updateForegroundList();
        allProcesses[foregroundProcesses[foregroundIndex]].state = READY;
    }
    if (allProcesses[pid].waiting != 0) {
        changeStatePid(allProcesses[pid].waiting, READY);
        allProcesses[pid].waiting = 0;
    }
    eraseProcessSem(pid);
    free(allProcesses[pid].stackPos);
    totalProcess--;
    removeProcess(pid,allProcesses[pid].priority);
}

int changeStateFromShell(int pid, int state) {
    if (state == BLOCKED && allProcesses[pid].state == BLOCKED)
        return changeStatePid(pid, READY);
    return changeStatePid(pid, state);
}

int changeStatePid(int pid, int state) {
    if ( allProcesses[pid].state == KILLED)
        return -1;
    allProcesses[pid].state = state;
    if(state == KILLED)
        liberateResourcesPid(pid);
    if (pid == currentPid)
        yield();
    return 0;
}

int getPid() {
    return currentPid;
}

void removeProcess(int pid,int pri) {
    processLists[currentList][pri][pid] = 0;
    processLists[1-currentList][pri][pid] = 0;
}

void nice(int pid,int pri) {
    if (allProcesses[pid].state != KILLED) {
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

int isBack() {
    return !allProcesses[currentPid].foreground;
}

void yield(){
    timeCycle = 0;
   _timertick();
}

int getFD(int f) {
    return allProcesses[currentPid].fd[f];
}

void wait(int pid) {
    if (allProcesses[pid].state!= KILLED) {
        allProcesses[pid].waiting = currentPid;
        changeStatePid(currentPid, BLOCKED);
    }
}