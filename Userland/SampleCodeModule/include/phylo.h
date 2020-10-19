#ifndef PHYLO_H
#define PHYLO_H

#include <clib.h>

#define MAXPHYLOS 10
#define THINKTIME 100000000
#define EATTIME 100000000
#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define GLOBAL "global"
#define CANT "cant"

void createPhylo();
void mainPhylo(int phyl);

#endif