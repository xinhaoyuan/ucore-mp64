#include <proc/proc.h>
#include <mp/mp.h>
#include <kio.h>
#include <intr.h>

void
__kern_entry(void)
{
	mp_init();
	proc_init();

	kmm_init();
	trap_init();

	timer_init();
	
	local_intr_enable_hw;
	event_loop(&proc_current->event_pool);
	/* XXX: PANIC - IDLE ENDS HERE */
	while (1) ;
}
