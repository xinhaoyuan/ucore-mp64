#include <clock.h>

volatile size_t ticks;

void
clock_init(void)
{
	ticks = 0;
	tick_init(100);
}
