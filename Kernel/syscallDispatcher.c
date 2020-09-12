#include <syscallDispatcher.h>

void print_handler(int fd, void* p)
{
    switch(fd)
    {
        case 1: printChar((char) p);break;
        case 2: printString((char*) p);break;
        case 3: printStringRed( (char*) p);break;
        default:  printString("Invalid file descriptor\n");
    }
}

uint64_t read_time(int fd)
{
    return readRTC(fd);
}

char * data_handler(int fd, char * buffer)
{
    switch(fd)
    {
        case 1: return cpuVendor(buffer);break;
        case 2: return cpuBrand(buffer);break;
        default:  printString("Invalid file descriptor\n");
    }
    return (char *) 0;
}

void* syscallDispatcher(int p1, void* p2, void* p3)
{
    switch(p1)
    {
        case 1: print_handler((int) p2, p3);break;
        case 2: return read_key();break;
        case 3: changeWindow();changeKeyboard();switchProgram();updateIp();break;
        case 4: return read_time((int) p2);break;
        case 5: return data_handler((int) p2, (char *)p3);
        case 6: return cpuModel();
        case 7: return getRegisters();
        case 8: saveProgramData((uint64_t) p2, (uint64_t) p3);switchProgram();break;
        case 9: return getCpuTemp();
        default: printString("Invalid syscall number\n");
    }
    return (void *) 0;
}