#include <time.h>

static unsigned long ticks = 0;

void timer_handler() {
	ticks++;
	schedule_handler();
	if (ticks%18==0)        //despues vemos
		printBar();
	else if (ticks%18==9)
		unprintBar();
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}