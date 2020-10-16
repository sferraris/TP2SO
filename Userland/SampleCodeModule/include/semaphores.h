#ifndef SEMAPHORES_H
#define SEMAPHORES_H

#include <clib.h>
#include <asmlib.h>
#define TOTALSEM 10
#define TOTALPROCESSES 10
uint64_t sem_post(char * name);
uint64_t sem_wait(char * name);
uint64_t sem_open(char * name, int status);
uint64_t sem_close(char * name);

#endif