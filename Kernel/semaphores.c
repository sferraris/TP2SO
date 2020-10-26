// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <semaphores.h>

typedef struct {
    char * name;
    int status;
    int processesList[TOTALPROCESSES]; //procesos bloqueados
    int processes;
    int cantProcesses;
    int nextProcess;
    int openProcesses;
}semaphore_t;

semaphore_t semaphores[TOTALSEM];
int cantSem = 0;
uint64_t bigLock = 0;

int getSem(char * name) {
    for (int i=0; i < TOTALSEM; i++) {
        if (strcmp(semaphores[i].name, name))
            return i;
    }
    return -1;
}

int nextSem(){
    for (int i = 0; i < TOTALSEM; i++){
        if (semaphores[i].name == 0)
            return i;
    }
    return -1;
}

int createSem(char * name, int status) {
    int index = nextSem();
    if ( index == -1)
        return 0;
    semaphore_t sem;
    sem.name = name;
    sem.status = status;
    for (int i=0; i<TOTALPROCESSES;i++)
        sem.processesList[i] = 0;
    sem.processes = 0;
    sem.cantProcesses = 0;
    sem.nextProcess = 0;
    sem.openProcesses = 1;
    semaphores[index] = sem;
    cantSem++;
    return 1;
}

int semPresent(char * name) {
    int aux = -1;
    for (int i=0; i < TOTALSEM && aux == -1; i++) {
        if (strcmp(semaphores[i].name, name))
            aux = i;
    }
    return aux;
}

int findProcessIndex(int sem) {
    if (semaphores[sem].cantProcesses == TOTALPROCESSES)
        return -1;
    int index = semaphores[sem].processes;
    semaphores[sem].processes++;
    if (semaphores[sem].processes == TOTALPROCESSES)
        semaphores[sem].processes = 0;
    return index;
}

int addProcessSem(int pid,int sem) {
    int index = findProcessIndex(sem);
    if (index == -1)
        return -1;
    semaphores[sem].processesList[index] = pid;
    semaphores[sem].cantProcesses++;
    return 1;
}

uint64_t sem_open(char * name, int status) {
    int sem = semPresent(name);
    if (sem == -1)
        return createSem(name, status);
    semaphores[sem].openProcesses++;
    return 1;
}

void cleanSem(int index) {
    semaphores[index].name = (char *) 0;
    semaphores[index].processes = 0;
    semaphores[index].nextProcess = 0;
    semaphores[index].cantProcesses = 0;
    for (int i=0; i < TOTALPROCESSES; i++)
        semaphores[index].processesList[i] = 0;
}

uint64_t sem_close(char * name){
    int index = getSem(name);
    if (index != -1){
        semaphores[index].openProcesses--;
        if (semaphores[index].openProcesses == 0) {
            cleanSem(index);
            cantSem--;
            return 1;
        }
        return semaphores[index].openProcesses;
    }
    return 0;
}

void sleep(int sem) {
    int pid = getPid();
    int aux = addProcessSem(pid, sem);
    if (aux == 1) {
        changeStatePid(pid, BLOCKED);
    }
}

int findNextProcess(int sem){
    if (semaphores[sem].cantProcesses == 0)
        return -1;
    while (semaphores[sem].processesList[semaphores[sem].nextProcess] == -1) {
        semaphores[sem].nextProcess++;
        if (semaphores[sem].nextProcess == TOTALPROCESSES)
            semaphores[sem].nextProcess = 0;
    }
    int i = semaphores[sem].nextProcess;
    semaphores[sem].nextProcess++;
    if (semaphores[sem].nextProcess == TOTALPROCESSES)
        semaphores[sem].nextProcess = 0;
    return i;
}

void acquire() {
    while (_xchg(&bigLock,1) == 1);
}

void release() {
    _xchg(&bigLock,0);
}

uint64_t sem_wait(char * name) {
    int index = getSem(name);
    if (index != -1) {
        acquire();
        if (semaphores[index].status > 0)
            semaphores[index].status--;
        else {
            release();
            sleep(index);
            acquire();
            semaphores[index].status--;
        }
        release();
    }
    return 1;
}

void wakeup(int sem) {
    int index = findNextProcess(sem);
    if (index != -1) {
        int pid = semaphores[sem].processesList[index];
        semaphores[sem].processesList[index] = 0;
        semaphores[sem].cantProcesses--;
        changeStatePid(pid, READY); //Desbloquear el proceso
    }
}

uint64_t sem_post(char * name) {
    int index = getSem(name);
    if (index != -1) {
        acquire();
        semaphores[index].status++;
        wakeup(index);
        release();
    }
    return 1;
}

void listBProcesses(int * s, char * buf, int cProcesses) {
    char * aux[TOTALPROCESSES*2];
    int cant = 0;
    for (int i = 0; i < TOTALPROCESSES && cant < cProcesses; i++) {
        if (s[i] > 0) {
            aux[cant++] = dectostr(s[i]);
            aux[cant++] = ", ";
        }
    }
    strcat(buf, aux, cant-1);
}

char ret[TOTALSEM*100];
char * printSemaphores() {
    int cant = 0, i = 0;
    while (ret[i] != 0){
        ret[i++] = 0;
    }
    char status[10], bp[50] = {0};
    for (int s=0; s < TOTALSEM; s++) {
        if (semaphores[s].name != 0) {
            cant++;
            strcpy(status, dectostr(semaphores[s].status));
            listBProcesses(semaphores[s].processesList, bp, semaphores[s].cantProcesses);
            char *aux[] = {"Sem: ", semaphores[s].name, "\n",
                           "Status: ", status, "\n",
                           "Blocked Processes: ", bp, "\n\n"};
            strcat(ret, aux, 9);
        }
    }
    if (cant == 0)
        strcpy(ret, "No semaphores available\n");
    return ret;
}

void eraseProcessSem(int pid) {
    for(int i=0; i < TOTALSEM; i++) {
        if (semaphores[i].name != 0) {
            for (int j=0; j < TOTALPROCESSES; j++) {
                if (semaphores[i].processesList[j] == pid) {
                    semaphores[i].processesList[j] = -1;
                    semaphores[i].cantProcesses--;
                }
            }
        }
    }
}