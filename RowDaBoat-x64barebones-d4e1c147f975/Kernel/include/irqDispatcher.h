#ifndef IRQDISPATCHER_H
#define IRQDISPATCHER_H
#include <time.h>
#include <keyboard.h>
#include <stdint.h>
#include <video_driver.h>
void irqDispatcher(uint64_t irq);
#endif