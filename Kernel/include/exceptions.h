#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#include <video_driver.h>
#include <interrupts.h>
#include <programData.h>

#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE_EXCEPTION_ID 6
void exceptionDispatcher(int exception);

#endif