#include <pipes.h>

pipe_t pipes[MAXPIPES];

void blockProcessPipe(int * p, int pid) {
    int i;
    for(i = 0; i < PIPEPROCESSES && p[i] != 0; i++);
    if (i == PIPEPROCESSES)
        return;
    p[i] = pid;
    changeStatePid(pid, BLOCKED);
}

void releaseProcessesPipe(int * p) {
    for (int i=0; i < PIPEPROCESSES && p[i] != 0;  i++) {
        changeStatePid(p[i], READY);
        p[i] = 0;
    }
}

int pipewrite(pipe_t *p, char *addr, int n) {
    int i;
    for (i = 0; i < n && addr[i] != 0; i++) {
        while (p->nwrite >= p->nread + PIPESIZE)
            blockProcessPipe(p->wProcesses, getPid());
        p->data[p->nwrite++ % PIPESIZE] = addr[i];
    }
    p->data[p->nwrite % PIPESIZE] = -1;
    releaseProcessesPipe(p->rProcesses);
    return i;
}

int piperead(pipe_t *p, char *addr, int n) {
    int i;
    for (i = 0; i < n && p->data[p->nread % PIPESIZE] != -1; i++) { //ver compa
        while (p->nread >= p->nwrite) {
            blockProcessPipe(p->rProcesses, getPid());
        }
        addr[i] = p->data[p->nread++ % PIPESIZE];
    }
    addr[i] = 0;
    if (p->data[p->nread % PIPESIZE] == -1)
        p->data[p->nread % PIPESIZE] = 0;
    releaseProcessesPipe(p->rProcesses);
    return i;
}

int nextPipe() {
    int aux = 0;
    while (pipes[aux].created != 0 && aux < MAXPIPES)
        aux++;
    return (aux == MAXPIPES) ? -1 : aux;
}

uint64_t pipeOpen(int p[2]) {
    int aux = nextPipe();
    if (aux == -1)
        return -1;
    p[0] = (aux+1)*10;
    p[1] = p[0] + 1;
    pipes[aux].created = 1;
    return aux;
}

void pipeClose(int index) {
    for (int i=0; i < PIPEPROCESSES;  i++) {
        pipes[index].rProcesses[i] = 0;
        pipes[index].wProcesses[i] = 0;
    }
    pipes[index].data[0] = 0;
    pipes[index].nread = 0;
    pipes[index].nwrite = 0;
    pipes[index].created = 0;
}

pipe_t * getPipe(int p) {
    int aux = p/10 - 1;
    return &pipes[aux];
}

void listBlockedProcesses(int * p, char * buf) {
    char * aux[PIPEPROCESSES*2];
    int cant = 0;
    for (int i = 0; i < PIPEPROCESSES; i++) {
        if (p[i] != 0) {
            aux[cant++] = dectostr(p[i]);
            aux[cant++] = ", ";
        }
    }
    strcat(buf, aux, cant-1);
}
char retpipes[MAXPIPES*100];
char * listPipes() {
    int cant = 0, i = 0;
    while (retpipes[i] != 0){
        retpipes[i++] = 0;
    }
    char numPipe[10], state[10], brp[50] = {0}, bwp[50] = {0};
    for (int p=0; p < MAXPIPES; p++) {
        if (pipes[p].created != 0) {
            cant++;
            strcpy(numPipe, dectostr(p));
            strcpy(state, dectostr(pipes[p].nwrite - pipes[p].nread));
            listBlockedProcesses(pipes[p].rProcesses, brp);
            listBlockedProcesses(pipes[p].wProcesses, bwp);
            char *aux[] = {"Pipe: ", numPipe, "\n",
                           "State: ", state, " bytes to read\n",
                           "Blocked Read Processes: ", brp, "\n",
                           "Blocked Write Processes: ", bwp, "\n\n"};
            strcat(retpipes, aux, 12);
        }
    }
    if (cant == 0)
        strcpy(retpipes, "No pipes available\n");
    return retpipes;
}