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

int getSem(char * name) {
	int aux = -1;
	for (int i=0; i < TOTALSEM && aux == -1; i++) {
		if ( strcmp(semaphores[i].name, name) )
			return i;
	}
	return aux;
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
    semaphores[index] = sem;
    semaphores[index].openProcesses++;
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

int findProcessIndex(int sem){
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
	if ( index == -1)
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

uint64_t sem_close(char * name){
	int index = getSem(name);
	if (index != -1){
        semaphores[index].openProcesses--;
		if (semaphores[index].openProcesses == 0 ) {
			semaphores[index].name = (char *) 0;
            semaphores[index].processes = 0;
            semaphores[index].nextProcess = 0;
            semaphores[index].cantProcesses = 0;
			cantSem--;
			return 1;
		}
        return semaphores[index].openProcesses;
	}
	return 0;
}

void sleep(int sem) {
	if (getLock(sem) > 0)
		decreaseSignal(sem);
	else {
		int pid = getPid();
		int process = addProcessSem(pid, sem);
		if ( process == 1){
			blockProcess(pid);
		}
	}
}

int findNextProcess(int sem){
	if (semaphores[sem].cantProcesses == 0)				// processes -> indice para agregar el prox
		return -1;								//cantProcesses -> cant = 0
	int i = semaphores[sem].nextProcess;					//nextprocess -> proximo a elegir
	semaphores[sem].nextProcess++;
	if (semaphores[sem].nextProcess == TOTALPROCESSES)    // t	0	0	0	0	0	n	m	p	q
		semaphores[sem].nextProcess = 0;					// ^						^processes && next
	return i;
	
}

uint64_t sem_wait(char * name) {
	int index = getSem(name);
	if ( index != -1){
		while ( _xadd(-1, &(semaphores[index].status)) <= 0 ) {
			_xadd(1, &(semaphores[index].status));
			sleep(index); //ojo signal lost
		}
	}
	return 1;
}

uint64_t sem_post(char * name) {
	int index = getSem(name);
	if ( index != -1){
		if ( semaphores[index].cantProcesses > 0) 
			wakeup(index);
		_xadd(1,&(semaphores[index].status));
	}
	return 1;
}

void wakeup(int sem) {
	int index = findNextProcess(sem);
	if ( index != -1){
		int pid = semaphores[sem].processesList[index];
		semaphores[sem].processesList[index] = 0;
		semaphores[sem].cantProcesses--;
    	blockProcess(pid); //Desbloquear el proceso
	}
	else {
		increaseSignal(sem);
	}
}

void printSemaphores() {
	if (cantSem == 0) {
	    printf("No semaphores available\n");
	    return;
	}
    for (int i = 0; i < TOTALSEM; i++) {
		if (semaphores[i].name != 0) {
		    printf("Sem: ");
		    printf(semaphores[i].name);
		    printf("\t\t");
		    printf("Status: ");
		    putDec(semaphores[i].status);
		    printf("\t\t");
		    printf("Blocked Processes: ");
		    for (int j = 0; j < TOTALPROCESSES; j++) {
			    if ( semaphores[i].processesList[j] != 0) {
				    putDec(semaphores[i].processesList[j]);
				    printf(" ");
		        }
		    }
		}
	}
}