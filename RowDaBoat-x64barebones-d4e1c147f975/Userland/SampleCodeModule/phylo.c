// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <phylo.h>

#define LEFT(n) (i+(n)-1)%(n)
#define RIGHT(n) (i+1)%(n)

char sems[MAXPHYLOS][5];
int state[MAXPHYLOS];
int pidArray[MAXPHYLOS];
uint64_t cantPhylos;

void printPhylos() {
    for (int i = 0; i< cantPhylos; i++) {
        if (state[i] == 2)
            printf("E ");
        else
            printf(". ");
    }
    printf("\n");
}

void test(int i){
    if((state[i] == HUNGRY) && (state[LEFT(cantPhylos)] != EATING) && (state[RIGHT(cantPhylos)] != EATING)){
        state[i] = EATING;
        printPhylos();
        sem_post(sems[i]);
    }
}

void put_forks(int i){
    sem_wait(GLOBAL);
    state[i] = THINKING;
    test(LEFT(cantPhylos));
    test(RIGHT(cantPhylos));
    sem_post(GLOBAL);
}

void take_fork(int i) {
    sem_wait(GLOBAL);
    state[i] = HUNGRY;
    test(i);
    sem_post(GLOBAL);
    sem_wait(sems[i]);
}

void think() {
    for (int i=0; i < THINKTIME; i++);
}

void eat() {
    for (int i=0; i < EATTIME; i++);
}

void philosopher(int i){
    while(1){
        think();
        take_fork(i);
        eat();
        put_forks(i);
    }
}

void semCreate(int index) {
    sems[index][0] = 's';
    sems[index][1] = 'e';
    sems[index][2] = 'm';
    sems[index][3] = '0' + index;
    sems[index][4] = 0;
    sem_open(sems[index], 0);
}

void addPhylo() {
    sem_wait(GLOBAL);
    if (cantPhylos < MAXPHYLOS) {
        semCreate(cantPhylos);
        char *argv[] = {(char *)philosopher, "phylosopher", 0, 0,(char *) 1, (char *)cantPhylos};
        pidArray[cantPhylos] = createProcess(6, argv);
        cantPhylos++;
    }
    else
        printf("Can not add more phylos\n");
    sem_post(GLOBAL);
}

void removePhylo() {
    sem_wait(GLOBAL);
    if (cantPhylos > 1) {
        cantPhylos--;
        killProcess(pidArray[cantPhylos]);
        sem_close(sems[cantPhylos]);
        state[cantPhylos] = THINKING;
    }
    else
        printf("Can not remove any more phylos\n");
    sem_post(GLOBAL);
}

void exitPhylo() {
    sem_wait(GLOBAL);
    while (cantPhylos > 0) {
        cantPhylos--;
        killProcess(pidArray[cantPhylos]);
        sem_close(sems[cantPhylos]);
        state[cantPhylos] = THINKING;
    }
    sem_close(GLOBAL);
    printf("Exiting Phylo\n");
    exit();
}

void mainPhylo(int phyl) {
    char * argv[] = {(char *)philosopher, "phylosopher", BACKGROUND, STD_IN, (char *)STD_OUT, 0};
    sem_open(GLOBAL,1);
    for (uint64_t i = 0; i < phyl; i++) {
        semCreate(i);
        argv[5] =(char *) i;
        sem_wait(GLOBAL);
        cantPhylos++;
        sem_post(GLOBAL);
        pidArray[cantPhylos-1] = createProcess(6, argv);
    }
    while (1) {
        char c;
        getChar(&c);
        switch(c) {
            case 'a': addPhylo();break;
            case 'r': removePhylo();break;
            case 'e': exitPhylo();
        }
    }
}