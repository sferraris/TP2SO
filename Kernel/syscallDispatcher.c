#include <syscallDispatcher.h>

int print_handler(int output, char * buffer, int n) {
    int aux = getFD(1);
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

int read_handler(int input,char * buffer, int n) {
    int aux = getFD(0);
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
        case 1: return print_handler((int)p2, (char *) p3, (int)p4);break;
        case 2: return read_handler((int)p2, (char *) p3, (int)p4);break;
        case 3: return read_time((int) p2);break;
        case 4: return data_handler((int) p2, (char *)p3);
        case 5: return cpuModel();
        case 6: return getRegisters();
        case 7: saveProgramData((uint64_t) p2, (uint64_t) p3);break;
        case 8: return getCpuTemp();
        case 9: return malloc((uint64_t) p2);
        case 10: free(p2);break;
        case 11: return createProcess((int) p2, (char *) p3);break;
        case 12: exit();break;
        case 13: return getPid();
        case 14: return changeStateFromShell((uint64_t) p2, (int) p3);
        case 15: return listProcesses();break;
        case 16: nice((int) p2,(int) p3);break;
        case 17: yield();break;
        case 18: return getLock((int) p2);
        case 19: increase((int) p2);break;
        case 20: decrease((int) p2);break;
        case 21: return pipeOpen((int *) p2);
        case 22: pipeClose((int) p2);break;
        case 23: return listPipes();
        case 24: return sem_post((char *) p2);
        case 25: return sem_wait((char *) p2);
        case 26: return sem_open((char *) p2, (int) p3);
        case 27: return sem_close((char *) p2);
        case 28: return printSemaphores();
        default: printString("Invalid syscall number\n");
    }
    return (void *) 0;
}