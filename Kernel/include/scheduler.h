#ifndef SCHEDULER_H_
#define SCHEDULER_H_
#include <memlib.h>
#include <interrupts.h>
#define STACKSIZE (1024 * 4)
#define PROCESSES 100
#define KILLED 0
#define BLOCKED 1
#define READY 2
void * schedule(void * rsp);
void createProcess(void * rip);

#endif