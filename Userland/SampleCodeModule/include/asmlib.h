#ifndef _ASMLIB_H
#define _ASMLIB_H

#include <stdint.h>

void write (int fd, void* buffer);
char read();
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
void createProcessAsm(void * rip);
void exitAsm();
int getPidAsm();
int killProcessAsm(int pid);
int changeProcessStateAsm(int pid,int state);
void listProcessesAsm();
void niceAsm(int pid,int pri);

#endif