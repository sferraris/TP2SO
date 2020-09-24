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