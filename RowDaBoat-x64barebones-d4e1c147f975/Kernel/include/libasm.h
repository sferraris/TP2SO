#ifndef LIBASM_H
#define LIBASM_H

char *cpuVendor(char *result);
char *cpuBrand(char *buffer);
uint64_t cpuModel();
uint64_t rebootProgram(uint64_t rip,uint64_t rsp);
uint64_t getCpuTemp();
uint8_t readRTC(int fd);
char getKey();


#endif