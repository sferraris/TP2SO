// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <programData.h>
#include <video_driver.h>
    uint64_t rip;
    uint64_t rsp;

void saveProgramData(uint64_t newrip, uint64_t newrsp) {
    printHex(newrip);
    rip = newrip;
    rsp = newrsp;
}

void updateIp() {
    rebootProgram(rip, rsp);
}