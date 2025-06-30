struct timer {
	struct timeval t1;
	struct timeval t2;
	float deltatime;
};

void timer_init(struct timer *timer);
void timer_update(struct timer *timer);
