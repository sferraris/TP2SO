#include <semaphores.h>
/*
typedef struct {
	char * name;
	int * status;
	int * processesList; //procesos bloqueados
	int processes;
}semaphore_t;

semaphore_t semaphores[TOTALSEM];
int cantSem = 0;


int searchAvailSem() {
    for (int i=0; i < TOTALSEM; i++) {
       if ( semaphores[i].name == 0) 
        return i;
    }
    return -1;
}

int addSem(char * name,int * mem, int pid) {
	semaphore_t sem;
	sem.name = name;
	sem.status = mem; 
	sem.processesList[0] = pid;
	sem.processes = 1;
    int pos = searchAvailSem();
    if (pos == -1)
        return pos;
    semaphores[pos] = sem;
    return 0;
}

int semPresent(char * name) {
	int aux = -1;
	for (int i=0; i < TOTALSEM && aux == -1; i++) {
		if (strcmp(semaphores[i].name, name))
			aux = i;
	}
	return aux;
}

void addProcessSem(int pid,semaphore_t * sem) {
	sem->processesList[(sem->processes)++] = pid;
}



int semEmpty(int sem) {
    if ( semaphores[sem].processes == 0)
        return 1;
    return 0;
}

int getSem(char * name) {
	int aux = -1;
	for (int i=0; i < TOTALSEM && aux == -1; i++) {
		if ( strcmp(semaphores[i].name, name) )
			return i;
	}
	return aux;
}

void desbloquearProcesoSem(semaphore_t * sem) {
    if ( sem->processes > 0) {
        blockProcess(sem->processesList[sem->processes-1]);
        (sem->processes)--;
    }
}

int sem_open(char * name) {
	int sem = semPresent(name);
    int create;
	if (sem == -1) { 
	int * aux = malloc( sizeof(int) );
    create = addSem(name,aux,getPid());
    if (create == -1)
        return -1;
	}
	else {
		addProcessSem(getPid(),&semaphores[sem]);
	}
	return 0;
}

int sem_close(char * name) {
	int index = getSem(name);
	if( index == -1){
		printf("Semaphore doesn't exist\n");
		return -1;
	}
	semaphores[index].name = 0;
	return 0;
}

int sem_wait(char * name) {
	int index = getSem(name);
	if( index == -1){
		printf("Semaphore doesn't exist\n");
		return -1;
	}
	semaphore_t sem = semaphores[index];
	if (_xchg(sem.status,1)) { 
	_dec(sem.status);
	}
	else {
	int pid = getPid();
	blockProcess(pid); //(registrando el motivo) y tal vez conviene hacer todo en una funcion nueva para evitar doble syscall (SYSCALL)
	}
	return 0;
}

int sem_post(char * name) {
	int index = getSem(name);
	if( index == -1){
		printf("Semaphore doesn't exist\n");
		return -1;
	}
    semaphore_t sem = semaphores[index];
    if ( !semEmpty(index) )
        desbloquearProcesoSem(&sem);
    _inc(sem.status);
	return 0;
}
*/
/////////////////////////////////////////////////////////////////////
typedef struct {
    char * name;
    int * status;
    int processesList[TOTALPROCESSES]; //procesos bloqueados
    int processes;
	int nextProcess;
    int * signals;
}semaphore_t;

semaphore_t semaphores[TOTALSEM];
int cantSem = 0;

int nextSem(){
	for ( int i = 0; i < TOTALSEM; i++){
		if ( semaphores[i].name == 0)
			return i;
	}
	return -1;
}
int addSem(char * name,void * mem, int status) {
	int index = nextSem();
	if ( index == -1)
		return 0;
    semaphore_t sem;
    sem.name = name;
    sem.status = mem;
	*(sem.status) = status;
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
int findProcessIndex(semaphore_t * sem){
	int contador = 0;
	for (int i = sem->processes; i < TOTALPROCESSES && contador < TOTALPROCESSES; i++, contador++){
		if ( i == TOTALPROCESSES - 1)
			i = -1;
		if ( sem->processesList[i] == 0 )
			return i;
	}
	return -1;
}

int addProcessSem(int pid,semaphore_t * sem) {
	int index = findProcessIndex(sem);
	if ( index == -1){
		return -1;
	}
	else{
		sem->processesList[index] = pid;
		sem->processes++;
	}
	return 0;
	
}


uint64_t sem_open(char * name, int status) {
    int sem = semPresent(name);
    if ( sem == -1) {
    void * aux = malloc(sizeof(uint64_t));//reservar espacio para int y retornar puntero 
    if (aux == (void *)0 )
        return 0;
    return addSem(name,aux, status);
    }
    return 0;
}

uint64_t sem_close(char * name){
	int index = getSem(name);
	if ( index != -1){
		if ( semaphores[index].processes == 0 ){
			semaphores[index].name = (char *) 0;
			cantSem--;
			return 1;
		}
	}
	return 0;
}


void sleep(semaphore_t * s) {
    while (s->signals > 0 && s->processes > 0) {
        wakeup(s);
        (*(s->signals))--;
    }
	int pid = getPid();
	if ( addProcessSem(pid,s) == 0)
    	blockProcess(pid);
}

int findNextProcess(semaphore_t * sem){
	int contador = 0;
	for (int i = sem->nextProcess; i < TOTALPROCESSES && contador < TOTALPROCESSES; i++, contador++){
		sem->nextProcess++;
		if ( sem->nextProcess == TOTALPROCESSES - 1)
			sem->nextProcess = 0;
		if ( i == TOTALPROCESSES - 1)
			i = -1;
		if ( sem->processesList[i] != 0 )
			return i;
	}
	return -1;
}



int getSem(char * name) {
	int aux = -1;
	for (int i=0; i < TOTALSEM && aux == -1; i++) {
		if ( strcmp(semaphores[i].name, name) )
			return i;
	}
	return aux;
}

uint64_t sem_wait(char * name) {
	int index = getSem(name);
	if ( index != -1){
		while (_xadd(-1, semaphores[index].status) <= 0) {
			_xadd(1, semaphores[index].status);
			sleep(&semaphores[index]); //ojo signal lost
		}
	}
	return 1;
}

uint64_t sem_post(char * name) {
	int index = getSem(name);
	if ( index != -1){
		_xadd(1,semaphores[index].status);
		_xadd(1,semaphores[index].signals);
		wakeup(&semaphores[index]);
	}
	return 1;
}

void wakeup(semaphore_t * s) {
	int index = findNextProcess(s);
	if ( index != -1){
		int pid = s->processesList[index];
		s->processesList[index] = 0;
		s->processes--;
    	blockProcess(pid);
	}
}