#include <kio.h>
#include <mp.h>
#include <intr.h>
#include <vmm.h>

void
__kern_entry(void)
{
	if (lapic_id() == 3)
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
