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
#include <proc/ipe.h>
#include <driver/rand.h>

PLS static event_s __init_event;
PLS static proc_s  init_proc;

PLS event_t init_event = &__init_event;
PLS int init_finished = 0;

static ipe_packet_s ping_packet;

static void
ping_packet_handler(ipe_packet_t packet)
{
	kprintf("CPU %d: PING!\n", lapic_id);
}

static void
ping_packet_back_handler(ipe_packet_t packet)
{
	kprintf("CPU %d: PONG!\n", lapic_id);
}

static void
do_init(event_t e)
{
	ipe_init();

	if (lapic_id == 2)
	{
		ipe_packet_send(1, &ping_packet);
	}
	
	init_finished = 1;
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

	if (lapic_id == 2)
	{
		ipe_packet_init(&ping_packet, ping_packet_handler, ping_packet_back_handler, NULL);
	}
	
	kmm_init();
	trap_init();

	hpet_init();
	timer_init();

	rand_init();
	
	local_intr_enable_hw;
	timer_measure();
	
	event_loop(&proc_current->event_pool);
	/* XXX: PANIC - IDLE ENDS HERE */
	while (1) ;
}
