#include <syscallDispatcher.h>

void print_handler(int fd, void* p) {
    switch(fd) {
        case 1: printChar((char) p);break;
        case 2: printString((char*) p);break;
        case 3: printStringRed( (char*) p);break;
        default:  printString("Invalid file descriptor\n");
    }
}

uint64_t read_time(int fd) {
    return readRTC(fd);
}

char * data_handler(int fd, char * buffer) {
    switch(fd) {
        case 1: return cpuVendor(buffer);break;
        case 2: return cpuBrand(buffer);break;
        default:  printString("Invalid file descriptor\n");
    }
    return (char *) 0;
}
void exit(){
   changeState(KILLED);
}

int changeProcessState(int pid, int state){
    return changeStatePid(pid, state);
}

char readKey() {
    if (isBack())
        return 0;
    return read_key();
}

void* syscallDispatcher(int p1, void* p2, void* p3) {
    switch(p1) {
        case 1: print_handler((int) p2, p3);break;
        case 2: return readKey();break;
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
        case 14: return changeProcessState((uint64_t) p2, (int) p3);
        case 15: return listProcesses();break;
        case 16: nice((int) p2,(int) p3);break;
        default: printString("Invalid syscall number\n");
    }
    return (void *) 0;
}