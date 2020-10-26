// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <syscallDispatcher.h>

uint64_t print_handler(int fd, char * buffer, int n) {
    int aux = getFD(fd);
    if (aux == 1) {
        printString(buffer);
        return n;
    }
    else
        return pipewrite(getPipe(aux), buffer, n);
}

char readKey() {
    if (isBack())
        return 0;
    return read_key();
}

uint64_t read_handler(int fd, char * buffer, int n) {
    int aux = getFD(fd);
    if (aux == 0) {
        *buffer = readKey();
        return 1;
    }
    else
        return piperead(getPipe(aux), buffer, n);
}

uint64_t read_time(int fd) {
    return readRTC(fd);
}

char * data_handler(int fd, char * buffer) {
    switch(fd) {
        case 1: return cpuVendor(buffer);
        case 2: return cpuBrand(buffer);
        default:  printString("Invalid file descriptor\n");
    }
    return (char *) 0;
}
void exit(){
   changeStatePid(getPid(), KILLED);
}

void* syscallDispatcher(int p1, void* p2, void* p3, void* p4) {
    switch(p1) {
        case 1: return (void *) print_handler((uint64_t) p2, (char *)p3, (uint64_t) p4);break;
        case 2: return (void *) read_handler((uint64_t) p2, (char *)p3, (uint64_t) p4);break;
        case 3: return (void *) read_time((uint64_t) p2);break;
        case 4: return data_handler((uint64_t) p2, (char *)p3);
        case 5: return (void *) cpuModel();
        case 6: return getRegisters();
        case 7: return (void *) getCpuTemp();
        case 8: return malloc((uint64_t) p2);
        case 9: free(p2);break;
        case 10: return (void *) createProcess((uint64_t) p2, (char **) p3);break;
        case 11: exit();break;
        case 12: return (void *) getPid();
        case 13: return (void *) changeStateFromShell((uint64_t) p2, (uint64_t) p3);
        case 14: return listProcesses();break;
        case 15: nice((uint64_t) p2,(uint64_t) p3);break;
        case 16: yield();break;
        case 17: return (void *) pipeOpen((int *) p2);
        case 18: pipeClose((uint64_t) p2);break;
        case 19: return listPipes();
        case 20: return (void *) sem_post((char *) p2);
        case 21: return(void *) sem_wait((char *) p2);
        case 22: return (void *) sem_open((char *) p2, (uint64_t) p3);
        case 23: return (void *) sem_close((char *) p2);
        case 24: return printSemaphores();
        case 25: wait((uint64_t) p2);break;
        default: printString("Invalid syscall number\n");
    }
    return (void *) 0;
}