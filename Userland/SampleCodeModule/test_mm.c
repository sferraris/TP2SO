#include <mmtest.h>

#define MAX_BLOCKS 128
#define MAX_MEMORY (1024*1024*1) //Should be around 80% of memory managed by the MM

typedef struct MM_rq{
  void *address;
  uint32_t size;
}mm_rq;

void * memset1(void * destination, int32_t c, uint64_t length) {
    uint32_t chr = (uint32_t)c;
    char * dst = (char*)destination;

    while(length--) {
        dst[length] = chr;
        if (length < 26500) {
            putHex(&dst[length]);
            putChar(' ');
        }
    }

    return destination;
}

void test_mm(){
  mm_rq mm_rqs[MAX_BLOCKS];
  uint8_t rq;
  uint32_t total;

  while (1){
    rq = 0;
    total = 0;

    // Request as many blocks as we can
    while(rq < MAX_BLOCKS && total < MAX_MEMORY){
      mm_rqs[rq].size = GetUniform(MAX_MEMORY - total - 1) + 1;
      void* aux = malloc(mm_rqs[rq].size);
      if ( aux > 0x1000000 ){
        printf("MALLOC DE MIERDA");
        return;
      }

      mm_rqs[rq].address = aux; // TODO: Port this call as required
      printf("Size: ");
      putDec(mm_rqs[rq].size);
      putChar(' ');
      if (mm_rqs[rq].address != (void *) 0) {
          total += mm_rqs[rq].size;
          rq++;
      }
      printf("Total: ");
      putDec(total);
      putChar(' ');
    }
    printf("Salio de request ");
    // Set
    uint32_t i;
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != (void *)0) {
          printf("Adress: ");
          putHex(mm_rqs[i].address);
          putChar(' ');
          memset1(mm_rqs[i].address, i, mm_rqs[i].size); // TODO: Port this call as required
      }
    printf("Salio de set ");
    // Check
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != (void *)0)
        if(!memcheck(mm_rqs[i].address, i, mm_rqs[i].size))
          printf("ERROR!\n"); // TODO: Port this call as required
    printf("Salio del check ");
    // Free
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != (void *)0)
        free(mm_rqs[i].address);  // TODO: Port this call as required
    printf("Salio del free ");
  } 
}

int testMM(){
  test_mm();
  exit();
  return 0;
}