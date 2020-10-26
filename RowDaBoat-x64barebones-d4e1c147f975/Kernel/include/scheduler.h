#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <memlib.h>
#include <interrupts.h>
#include <video_driver.h>
#include <semaphores.h>
#include <stdint.h>

#define STACKSIZE (1024 * 4)
#define PROCESSES 100
#define KILLED 0
#define BLOCKED 1
#define READY 2
#define PRIORITIES 10
#define DEFAULTPRI 4
#define FOREGROUNDPROCESSES 10

void * schedule(void * rsp);
uint64_t createProcess(int argc, char * argv[]);
int changeStatePid(int pid, int state);
void liberateResourcesPid(int pid);
uint64_t getPid();
char * listProcesses();
void nice(int pid,int pri);
int isBack();
void detectChar();
int getFD(int f);
uint64_t changeStateFromShell(int pid, int state);
void waitForChar(int pid);
void wait(int pid);
void yield();

#endif