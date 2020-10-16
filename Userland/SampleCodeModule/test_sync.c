#include <test_sync.h>
void inc(uint64_t sem, uint64_t value, uint64_t N);

uint64_t my_create_process__(char * name, uint64_t sem, uint64_t value, uint64_t N){
  char * argv[] = {&inc, name, 0, sem, value, N};
  return createProcess(6, argv);
}

uint64_t my_sem_open(char *sem_id, uint64_t initialValue){
  return sem_open(sem_id, initialValue);
}

uint64_t my_sem_wait(char *sem_id){
  return sem_wait(sem_id);
}

uint64_t my_sem_post(char *sem_id){
  return sem_post(sem_id);
}

uint64_t my_sem_close(char *sem_id){
  return sem_close(sem_id);
}

#define TOTAL_PAIR_PROCESSES 2
#define SEM_ID "sem"

uint64_t global;  //shared memory

void slowInc(uint64_t *p, int64_t inc){
  uint64_t aux = *p;
  aux += inc;
  yield();
  *p = aux;
}

void inc(uint64_t sem, uint64_t value, uint64_t N){
  uint64_t i;
  if (sem && !my_sem_open(SEM_ID, 1)){
    printf("ERROR OPENING SEM\n");
    exit();
  }
  
  for (i = 0; i < N; i++){
    putDec(i);
    if (sem) my_sem_wait(SEM_ID);
    slowInc(&global, value);
    //printf("\nGLOBAL: ");
    //putDec(global);
    if (sem) my_sem_post(SEM_ID);
    
  }

  if (sem) my_sem_close(SEM_ID);

  printf("Final value: ");
  putDec(global);
  putChar('\n');
  exit();
}

void test_sync(){
  uint64_t i;

  global = 0;

  printf("CREATING PROCESSES...(WITH SEM)\n");

  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    my_create_process__("inc", 1, 1, 10);
    my_create_process__("inc", 1, -1, 10);
  }

}

void test_no_sync(){
  uint64_t i;

  global = 0;

  printf("CREATING PROCESSES...(WITHOUT SEM)\n");

  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    my_create_process__("inc", 0, 1, 10);
    my_create_process__("inc", 0, -1, 10);
  }
}

int Tsync(){
  test_sync();
  return 0;
}
