#ifndef LOCK_H
#define LOCK_H

#include <interrupts.h>

#define TOTALSEMS 10

int getSem(int index);
void increase(uint64_t index);
void decrease(uint64_t index);

#endif