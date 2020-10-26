#ifndef _ASMLIB_H
#define _ASMLIB_H

#include <stdint.h>

int writeAsm (int output, char * buffer, int n);
int readAsm (int input, char * buffer, int n);
int getTime (int fd);
void putNum(int fd, uint64_t num);
char * getCpuData(int fd, char * buffer);
int readCpuModel();
void readmem(int num, uint64_t * buffer);
void zero_exception_creator();
void invalid_opcode_creator();
int* readRegisters();
void sendIp();
int getCpuTemp();
void * getMalloc(uint64_t size);
void getFree(void * p);
int createProcessAsm(int argc, char * argv[]);
void exitAsm();
int getPidAsm();
int killProcessAsm(int pid);
int changeProcessStateAsm(int pid,int state);
char * listProcessesAsm();
void niceAsm(int pid,int pri);
void * _xchg(void * sem,int valor);
void * _inc(int * lock);
void * _dec(int * lock);
int _xadd(int num,int *s);
int _getLock(int index);
void _increaseSignal(int index);
void _decreaseSignal(int index);
int pipeAsm(int p[2]);
void closeAsm(int index);
char * _listPipes();
uint64_t semPostAsm(char * sem);
uint64_t semWaitAsm(char * sem);
uint64_t semOpenAsm(char * sem, int status);
uint64_t semCloseAsm(char * sem);
char * _listSemaphores();
void waitAsm(int pid);
void yieldAsm();

#endif