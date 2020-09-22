#include <memlib.h>
#include <video_driver.h> //testeo

int bitmap[TOTALBLOCKS];
int initialized = 0;

int mapLocation (int blocks);
void setBitmap (int loc, int blocks);
void unsetBitmap (int loc);

void * malloc (uint64_t size) {
    uint64_t aux = size;
    if (initialized == 0) {
        memset(HEAPSTART, 0, TOTALHEAP);
        initialized = 1;
    }
    int blocksCount = (size/HEAPBLOCK) + (((size%HEAPBLOCK)==0)? 0 : 1);
    int loc = mapLocation(blocksCount);
    if (loc == -1)
    {
        printString("llego paloteado");
        return -1; //chequear
    }
    setBitmap (loc, blocksCount);
    return (void *) (HEAPSTART+ loc*HEAPBLOCK); 
}

int mapLocation (int blocks) {
    int avail = 0;
    int i;
    for (i=0; i < TOTALBLOCKS && avail < blocks; i++) {
        if (bitmap[i] > EB)
            avail = 0;
        else
            avail++;
    }
    return (avail < blocks)? -1 : (i-blocks);
}

void setBitmap (int loc, int blocks) {
    int i;
    for (i=0; i < blocks-1; i++)
        bitmap[loc+i] = FB;
    bitmap[loc+i] = EOB; 
}

void free(void * p) {
    int aux = (int) p;
    unsetBitmap((aux - HEAPSTART)/HEAPBLOCK);
}

void unsetBitmap (int loc) {
    int i=loc;
    while (bitmap[i] != EOB)
        bitmap[i++] = EB;
    bitmap[i] = EB;
}