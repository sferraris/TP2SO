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
        case 7: saveProgramData((uint64_t) p2, (uint64_t) p3);break;
        case 8: return (void *) getCpuTemp();
        case 9: return malloc((uint64_t) p2);
        case 10: free(p2);break;
        case 11: return (void *) createProcess((uint64_t) p2, (char **) p3);break;
        case 12: exit();break;
        case 13: return (void *) getPid();
        case 14: return (void *) changeStateFromShell((uint64_t) p2, (uint64_t) p3);
        case 15: return listProcesses();break;
        case 16: nice((uint64_t) p2,(uint64_t) p3);break;
        case 17: yield();break;
        case 18: return (void *) getLock((int) p2);//SACAR
        case 19: increase((int) p2);break; //SACAR
        case 20: decrease((int) p2);break; //SACAR
        case 21: return (void *) pipeOpen((int *) p2);
        case 22: pipeClose((uint64_t) p2);break;
        case 23: return listPipes();
        case 24: return (void *) sem_post((char *) p2);
        case 25: return(void *) sem_wait((char *) p2);
        case 26: return (void *) sem_open((char *) p2, (uint64_t) p3);
        case 27: return (void *) sem_close((char *) p2);
        case 28: return printSemaphores();
        case 29: wait((uint64_t) p2);break;
        default: printString("Invalid syscall number\n");
    }
    return (void *) 0;
}