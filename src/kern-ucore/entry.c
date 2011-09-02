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
#include <x86/spinlock.h>
#include <mp.h>
#include <ide.h>
#include <trap.h>

static int init_finished = 0;

void
__kern_entry(void)
{
	mp_init();
	
	if (lcpu_idx == 0)
	{
		kern_enter();
		
		pmm_init_ap();
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

		kern_leave();
		init_finished = 1;
	}
	else
	{
		while (init_finished == 0);
		kern_enter();
		
		pmm_init_ap();
		trap_init_ap();
		proc_init_ap();
		
		kern_leave();
	}

	intr_enable();
	cpu_idle();                 // run idle process
	
	while (1) ;
}
