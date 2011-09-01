#include <proc/proc.h>
#include <mp/mp.h>
#include <driver/timer.h>
#include <driver/hpet.h>
#include <trap/trap.h>
#include <mm/kmm.h>
#include <kio.h>
#include <intr.h>
#include <debug/io.h>

PLS event_s __init_event;
PLS event_t init_event = &__init_event;

static void
do_init(event_t e)
{
	kprintf("LCPU %d INITIALIZED\n");
}

void
__kern_entry(void)
{
	mp_init();
	proc_init();

	event_open(init_event, &proc_current->event_pool, do_init, NULL);
	
	kmm_init();
	trap_init();

	hpet_init();
	timer_init();

	local_intr_enable_hw;
	timer_measure();
	
	event_loop(&proc_current->event_pool);
	/* XXX: PANIC - IDLE ENDS HERE */
	while (1) ;
}
