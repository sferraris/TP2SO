#include <protest.h>

//TO BE INCLUDED
void endless_loop(){
  while(1);
}

uint32_t my_create_process(char * name){
    char * argv[] = {endless_loop, name, 0};
    return createProcess(3, argv);
}

uint32_t my_kill(uint32_t pid){
  return killProcess(pid);
}

uint32_t my_block(uint32_t pid){
  return blockProcess(pid);
}

uint32_t my_unblock(uint32_t pid){
    return blockProcess(pid);
}

#define MAX_PROCESSES 10 //Should be around 80% of the the processes handled by the kernel

enum State {ERROR, RUNNING, BLOCK, KILL};

typedef struct P_rq{
  uint32_t pid;
  enum State state;
}p_rq;

void test_processes(){
  p_rq p_rqs[MAX_PROCESSES];
  uint8_t rq;
  uint8_t alive = 0;
  uint8_t action;

  while (1){
    // Create MAX_PROCESSES processes
    for(rq = 0; rq < MAX_PROCESSES; rq++){
      p_rqs[rq].pid = my_create_process("endless_loop");  // TODO: Port this call as required

      if (p_rqs[rq].pid == -1){                           // TODO: Port this as required
        printf("Error creating process\n");               // TODO: Port this as required
        return;
      }else{
        p_rqs[rq].state = RUNNING;
        alive++;
      }
    }
    // Randomly kills, blocks or unblocks processes until every one has been killed
    while (alive > 0){
      for(rq = 0; rq < MAX_PROCESSES; rq++){
        action = GetUniform(2) % 2; 

        switch(action){
          case 0:
            if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCK){
              if (my_kill(p_rqs[rq].pid) == -1){          // TODO: Port this as required
                printf("Error killing process\n");        // TODO: Port this as required
                return;
              }
              p_rqs[rq].state = KILL;
              alive--;
            }
            break;

          case 1:
            if (p_rqs[rq].state == RUNNING){
              if(my_block(p_rqs[rq].pid) == -1){          // TODO: Port this as required
                printf("Error blocking process\n");       // TODO: Port this as required
                return;
              }
              p_rqs[rq].state = BLOCK;
            }
            break;
        }
      }
      // Randomly unblocks processes
      for(rq = 0; rq < MAX_PROCESSES; rq++)
        if (p_rqs[rq].state == BLOCK && GetUniform(2) % 2){
          if(my_unblock(p_rqs[rq].pid) == -1){            // TODO: Port this as required
            printf("Error unblocking process\n");         // TODO: Port this as required
            return;
          }
          p_rqs[rq].state = RUNNING; 
        }
    }
  }
}

int proTest(){
  test_processes();
  return 0;
}
