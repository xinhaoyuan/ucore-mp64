#include <proc/proc.h>
#include <mp/mp.h>
#include <driver/timer.h>
#include <driver/hpet.h>
#include <trap/trap.h>
#include <mm/kmm.h>
#include <kio.h>
#include <intr.h>
#include <debug/io.h>
#include <pmm.h>

PLS event_s __init_event;
PLS event_t init_event = &__init_event;
PLS proc_s  init_proc;

static void
do_init(event_t e)
{
	if (lapic_id == 1)
		lapic_ipi_issue(2);
	
	kprintf("LCPU %d INITIALIZED\n", lcpu_idx);
}

static void
init_idle(void)
{
	proc_current->status = PROC_STATUS_WAIT;
	proc_schedule();
}

void
__kern_entry(void)
{
	mp_init();
	proc_init();

	/* XXX: UGLY */
	proc_open(&init_proc, "init", init_idle, NULL, (uintptr_t)KADDR_DIRECT(kalloc_pages(4)) + 4 * PGSIZE);
	init_proc.status = PROC_STATUS_WAIT;
	event_open(init_event, &init_proc.event_pool, do_init, NULL);
	
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
