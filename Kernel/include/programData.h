#ifndef _PROGRAMDATA_H
#define _PROGRAMDATA_H
#include <stdint.h>
#include <video_driver.h>
#include <libasm.h>

void switchProgram();
void saveProgramData(uint64_t rip, uint64_t rsp);
void updateIp();

#endif