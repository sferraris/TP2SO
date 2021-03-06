#ifndef _CLIB_H
#define _CLIB_H

#include <asmlib.h>
#include <stdint.h>

#define KILLED 0
#define BLOCKED 1
#define READY 2

#define BACKGROUND 0
#define FOREGROUND 1

#define STD_IN 0
#define STD_OUT 1

void putChar(char c);
void printf(char * buffer);
void getChar(char * c);
int write(int output, char * buffer, int n);
int read(int input, char * buffer, int n);
void getmem(int num, uint64_t*buffer);
void putDec(int num);
void putHex(uint64_t num);
void putBin(uint64_t num);
void putDouble(double num);
char * dectostr(uint64_t value);
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
void free(void * p);
int createProcess(int argc, char * argv[]);
void exit();
void yield();
int getPid();
int killProcess(int pid);
char * listProcesses();
void changePriority(int pid,int pri);
int blockProcess(int pid);
int pipe(int p[2]);
void close(int index);
char * listPipes();
uint64_t sem_post(char * sem);
uint64_t sem_wait(char * sem);
uint64_t sem_open(char * sem, int status);
uint64_t sem_close(char * sem);
int isVowel(char c);
char * listSemaphores();
void strcpy(char * str1, char * str2);
void strcat(char * str1, char * str2);
void wait(int pid);
int strlen(char * buffer);
int isNumberHexa(char c);

#endif