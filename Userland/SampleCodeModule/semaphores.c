typedef struct {
	char * name;
	int * status;
	int * processesList; //procesos bloqueados
	int processes;
}semaphore_t

semaphore_t semaphores[10];
int cantSem = 0;


int searchAvailSem() {
    for (int i=0; i < 10; i++) {
       if ( semaphores[i].name == 0) 
        return i;
    }
    return -1;
}

int addSem(char * name,void * mem, int pid) {
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
	for (int i=0; i < 10 && aux == -1; i++) {
		if (strcmp(semaphores[i].name, name))
			aux = i;
	}
	return aux;
}

void addProcessSem(int pid,semaphore_t * sem) {
	sem->processesList[(sem->processes)++] = pid;
}


void * sem_create(char * name) {
	int sem;
    int create;
	if ((sem = semPresent()) == -1) {
	void * aux = malloc(sizeof(int))//reservar espacio para int y retornar puntero 
    create = addSem(name,aux,getPid());
    if (create == -1)
        return (void *)0;
	}
	else {
		addProcessSem(getPid(),&semaphores[sem]);
	}
	return aux;
}



void removeSem(int index) {
    semaphores[index].name = 0;
}

void desbloquearProcesoSem(semaphore_t * sem) {
    if ( sem->processes > 0) {
        blockProcess(sem->processesList[sem->processes-1]);
        (sem->processes)--;
    }
}

int semEmpty(int sem) {
    if ( semaphores[sem].processes == 0)
        return 1;
    return 0;
}

int semGetByStatus(int * status) {
	int aux = -1;
	for (int i=0; i < 10 && aux == -1; i++) {
		if (semaphores[i].status, status))
			aux = i;
	}
	return aux;
}

void sem_wait(void *s) {
	if (_xchg(s,1)) { 
	_dec(s);
	}
	else {
	blockProcess(getPid()); //(registrando el motivo) y tal vez conviene hacer todo en una funcion nueva para evitar doble syscall (SYSCALL)
	}
}

void sem_post(void *s) {
    int sem;	
    sem = semGetByStatus(s);
    if (sem != -1) {
	//revisar si alguien esta esperando y cambiar su estado
    if (!semEmpty(sem))
        desbloquearProcesoSem(&semaphores[t])
    }
    _inc(s);
}