#ifndef _VIDEO_DRIVER_H_
#define _VIDEO_DRIVER_H_

#include <stdint.h>

#define WIDTH 1024
#define HEIGHT 768
#define BASESIZE 2
#define LETSIZE (8*BASESIZE)
#define BORDER 3    
#define FLOOR (HEIGHT-BORDER-LETSIZE-BASESIZE)
#define FIRSTPOS (BASESIZE + BORDER)
#define LASTPOS (WIDTH - BORDER)

void printBorder();
void printChar(char let); 
void printString ( char* string);
void printCharRed(char let);
void printStringRed ( char* string);

void printBin(uint64_t value);
void printHex(uint64_t value);
void printDec(uint64_t value);
void printBase(uint64_t value, uint32_t base);

void printBar();
void unprintBar();

#endif