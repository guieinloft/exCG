/*
 * timer
 * calcula a diferenca de tempo entre dois momentos
 * baseado na frequencia
 */
struct timer {
	LARGE_INTEGER frequency;
	LARGE_INTEGER t1;
	LARGE_INTEGER t2;
	float deltatime;
};

/* inicializa o momento 1 */
void timer_init(struct timer *timer);
/* calcula a diferenca entre os dois momentos e atualiza o mom. 2 */
void timer_update(struct timer *timer);
