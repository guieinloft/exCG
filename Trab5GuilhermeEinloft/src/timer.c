#include <stdlib.h>
#include <stdint.h>
#include <Windows.h>
#include "timer.h"

void timer_init(struct timer *timer)
{
    QueryPerformanceFrequency(&(timer->frequency));
	QueryPerformanceCounter(&(timer->t1));
	timer->deltatime = 0;
}

void timer_update(struct timer *timer)
{
    QueryPerformanceFrequency(&(timer->frequency));
	QueryPerformanceCounter(&(timer->t2));

	timer->deltatime = (float)(timer->t2.QuadPart - timer->t1.QuadPart)
            / timer->frequency.QuadPart;

	timer->t1 = timer->t2;
}
