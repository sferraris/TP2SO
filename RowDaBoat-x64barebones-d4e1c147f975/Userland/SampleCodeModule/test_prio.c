// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <test_prio.h>

#define MINOR_WAIT 10000000                               // TODO: To prevent a process from flooding the screen
#define WAIT      100000000                              // TODO: Long enough to see theese processes beeing run at least twice

uint64_t _my_getpid(){
  return getPid();
}

void _my_nice(uint64_t pid, uint64_t newPrio){
  changePriority(pid, newPrio);
}

uint64_t _my_kill(uint64_t pid){
  return killProcess(pid);
}

uint32_t _my_block(uint32_t pid){
  return blockProcess(pid);
}

uint32_t _my_unblock(uint32_t pid){
    return blockProcess(pid);
}

void bussy_wait(uint64_t n){
  uint64_t i;
  for (i = 0; i < n; i++);
}

void _endless_loop(){
  uint64_t pid = _my_getpid();

  while(1) {
      putDec(pid);
      printf(" ");
      bussy_wait(MINOR_WAIT);
  }
}

uint64_t _my_create_process(char * name){
  char * argv[] = {(char *)_endless_loop, name, BACKGROUND, STD_IN, STD_OUT};
  return createProcess(3, argv);
}

#define TOTAL_PROCESSES 3

void test_prio(){
  uint64_t pids[TOTAL_PROCESSES];
  uint64_t i;

  for(i = 0; i < TOTAL_PROCESSES; i++)
    pids[i] = _my_create_process("_endless_loop");

  bussy_wait(WAIT);
  printf("\nCHANGING PRIORITIES...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++){
     switch (i % 3){
      case 0:
        _my_nice(pids[i], 9); //lowest priority 
        break;
      case 1:
        _my_nice(pids[i], 4); //medium priority
        break;
      case 2:
        _my_nice(pids[i], 0); //highest priority
        break;
    }
  }
  bussy_wait(WAIT);
  printf("\nBLOCKING...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++)
    _my_block(pids[i]);

  printf("CHANGING PRIORITIES WHILE BLOCKED...\n");
  for(i = 0; i < TOTAL_PROCESSES; i++){
    switch (i % 3){
      case 0:
        _my_nice(pids[i], 4); //medium priority 
        break;
      case 1:
        _my_nice(pids[i], 4); //medium priority
        break;
      case 2:
        _my_nice(pids[i], 4); //medium priority
        break;
    }
  }

  printf("UNBLOCKING...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++)
    _my_unblock(pids[i]);

  bussy_wait(WAIT);
  printf("\nKILLING...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++)
    _my_kill(pids[i]);
}

int priorityTest(){
  test_prio();
  exit();
  return 0;
}
