#include <programData.h>

    uint64_t rip;
    uint64_t rsp;

void saveProgramData(uint64_t newrip, uint64_t newrsp) {
    rip = newrip;
    rsp = newrsp;
}

void updateIp() {
    rebootProgram(rip, rsp);
}