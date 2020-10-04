#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <interrupts.h>
#include <video_driver.h>
#include <scheduler.h>

void key_handler();
int* getRegisters();
char read_key();

#endif