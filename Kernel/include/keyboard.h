#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_
#include <interrupts.h>
#include <video_driver.h>

void key_handler();
void initializeKeyboard();
void changeKeyboard();
int* getRegisters();
char read_key();

#endif