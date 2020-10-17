#include <lock.h>

int bigLock;

int semaphores[TOTALSEMS];


int getLock(int index) {
    return semaphores[index];
}

void release(void) {
    _xchg(&bigLock,0);
}

void acquire(void) {
    while (_xchg(&bigLock,1));
}

void increase(uint64_t index) {
    acquire();
    semaphores[index]++;
    release();
}

void decrease(uint64_t index) {
    acquire();
    semaphores[index]--;
    release();
}