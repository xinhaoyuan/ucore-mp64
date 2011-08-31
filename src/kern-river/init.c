#include <proc/proc.h>
#include <mp/mp.h>
#include <kio.h>

void
__kern_entry(void)
{
	mp_init();
	kmm_init();
	proc_init();

	event_loop(&proc_current->event_pool);
	/* XXX: PANIC - IDLE ENDS HERE */
	while (1) ;
}
