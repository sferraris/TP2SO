#ifndef _MEMLIB_H_
#define _MEMLIB_H_

#include <lib.h>

#define TOTALHEAP (1024 * 1024 * 4)
#define HEAPBLOCK (1024 * 4)
#define TOTALBLOCKS (TOTALHEAP / HEAPBLOCK)
#define HEAPSTART 0x600000
#define EB 0
#define FB 1
#define EOB 2
#define MAXLEVEL 12

void * malloc (uint64_t size);
void free (void * p);
void initMem();

#endif