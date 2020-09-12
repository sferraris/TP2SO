#include <programData.h>

struct ProgramData{
    uint64_t rip;
    uint64_t rsp;
};

struct ProgramData windowsData[2];
int program = 1;

void switchProgram()
{
    program = 1-program;
}

void initData(struct ProgramData *wind, uint64_t rip, uint64_t rsp)
{
    wind->rip = rip;
    wind->rsp = rsp;
}

void saveProgramData(uint64_t rip, uint64_t rsp)
{
    struct ProgramData WindowData;
    initData(&WindowData, rip, rsp);
    windowsData[program] = WindowData;
}

void updateIp()
{
    rebootProgram(windowsData[program].rip, windowsData[program].rsp);
}