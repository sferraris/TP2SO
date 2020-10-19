#include <lock.h>

int abigLock;

int asemaphores[TOTALSEMS];

int getLock(int index) {
    return asemaphores[index];
}

void arelease() {
    _xchg(&abigLock,0);
}

void aacquire() {
    while (_xchg(&abigLock,1));
}

void increase(uint64_t index) {
    aacquire();
    asemaphores[index]++;
    arelease();
}

void decrease(uint64_t index) {
    aacquire();
    asemaphores[index]--;
    arelease();
}