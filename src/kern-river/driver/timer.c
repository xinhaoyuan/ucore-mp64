#include <driver/timer.h>
#include <intr.h>
#include <trap/trap.h>
#include <proc/proc.h>
#include <clock.h>

static void
tick_handler(int irq_no, uint64_t tick)
{
	if (proc_current->time_slice > tick)
		proc_current->time_slice -= tick;
	else
	{
		proc_current->time_slice = 0;
	}
}
	
int
timer_init(void)
{
	trap_irq_handler_set(IRQ_TIMER, tick_handler);
	tick_init(100);
	
	return 0;
}
