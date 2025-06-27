#include <stdlib.h>
#include <sys/time.h>
#include "timer.h"

void timer_init(struct timer *timer)
{
	gettimeofday(&(timer->t1), NULL);
	timer->deltatime = 1000/60;
}

void timer_update(struct timer *timer)
{
	gettimeofday(&(timer->t2), NULL);

	timer->deltatime = timer->t2.tv_sec - timer->t1.tv_sec
			+ (timer->t2.tv_usec - timer->t1.tv_usec) / 1000000.0;

	timer->t1 = timer->t2;
}
