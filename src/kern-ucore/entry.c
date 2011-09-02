#include <glue_mp.h>
#include <slab.h>
#include <vmm.h>
#include <pmm.h>
#include <slab.h>
#include <trap.h>
#include <sched.h>
#include <proc.h>
#include <swap.h>
#include <fs.h>
#include <clock.h>
#include <intr.h>

void
__kern_entry(void)
{
	if (lapic_id_get() == 0)
	{
		pmm_init();
		slab_init();
				
		trap_init();
		vmm_init();                 // init virtual memory management

		sched_init();               // init scheduler
		proc_init();                // init process table

		sync_init();                // init sync struct

		ide_init();
		swap_init();                // init swap
		fs_init();                  // init fs

		clock_init();
		intr_enable();

		cpu_idle();                 // run idle process
	}
	
	while (1) ;
}
