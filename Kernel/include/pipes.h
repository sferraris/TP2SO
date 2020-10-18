#ifndef PIPES_H_
#define PIPES_H_

#include <scheduler.h>
#include <video_driver.h>

#define MAXPIPES 10
#define PIPESIZE 1000
#define PIPEPROCESSES 10

typedef struct {
    int created;
    char data[PIPESIZE];
    int rProcesses[PIPEPROCESSES];
    int wProcesses[PIPEPROCESSES];
    unsigned int nread;
    unsigned int nwrite;
}pipe_t;

int pipewrite(pipe_t *p,char *addr,int n);
int piperead(pipe_t *p, char *addr, int n);
int pipeOpen(int p[2]);
void pipeClose(int index);
pipe_t * getPipe(int p);
char * listPipes();

#endif