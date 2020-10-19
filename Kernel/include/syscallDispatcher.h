#ifndef _SYSCALLDISPATCHER_H
#define _SYSCALLDISPATCHER_H

#include <stdint.h>
#include <video_driver.h>
#include <keyboard.h>
#include <lib.h>
#include <programData.h>
#include <libasm.h>
#include <memlib.h>
#include <scheduler.h>
#include <lock.h>
#include <pipes.h>

void * syscallDispatcher(int p1, void* p2, void* p3, void* p4);

#endif