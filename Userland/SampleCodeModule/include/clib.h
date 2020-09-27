#ifndef _CLIB_H
#define _CLIB_H

#include <asmlib.h>
#include <stdint.h>
#define KILLED 0x0
#define BLOCKED 0x1
#define READY 0x2

void putChar(char c);
void printf(char * buffer);
char getChar();
void printRed(char * buffer);
void getmem(int num, uint64_t*buffer);
void putDec(uint64_t num);
void putHex(uint64_t num);
void putBin(uint64_t num);
void putDouble(double num);
int strcmp(char * str1, char * str2);
int strcmpParams(char * str1, char * str2);
int getHours();
int getMinutes();
int getSeconds();
char * getCpuVendor(char * buffer);
char * getCpuBrand(char * buffer);
int getCpuModel();
int* receiveRegisters();
void putFilledHex (uint64_t num);
uint64_t getTemp();
int stringHexToInt(char * s);
void * malloc(uint64_t size);
void getFree(void * p);
void createProcess(void* rip);
void exit();
int getPid();
int killProcess(int pid);
int changeProcessState(int pid,int state);
int blockProcess(int pid);
void listProcesses();

#endif