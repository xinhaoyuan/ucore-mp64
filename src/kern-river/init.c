#include <proc/proc.h>
#include <mp/mp.h>
#include <drivers/timer.h>
#include <drivers/hpet.h>
#include <trap/trap.h>
#include <mm/kmm.h>
#include <glue_kio.h>
#include <glue_intr.h>
#include <debug/io.h>
#include <glue_pmm.h>
#include <proc/ipe.h>
#include <drivers/rand.h>
#include <drivers/pci.h>
#include <proc/eproc.h>
#include <vm/vm.h>

PLS static event_s __init_event;
PLS static eproc_s init_eproc;

PLS event_t init_event = &__init_event;
PLS int init_finished = 0;

#if IPE_PACKET_TEST

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

#endif

static void
do_init(event_t e)
{
	/* For each processor, the init event is activated to do
	 * initializations of system services when all system components are
	 * ready */

	if (lcpu_idx == mpconf_main_lcpu_idx)
	{
		vm_init();
		vm_start();
	}

	/* ipe init here is also an all lcpu barrier */
	ipe_init();

	kprintf("LCPU %d DONE\n", lcpu_idx);
	/* All initialzations are done */
	init_finished = 1;
}

void
__kern_entry(void)
{
	mp_init();
	proc_init();

	eproc_open(&init_eproc, "init", (void(*)(void))proc_wait_try, NULL, 8192);
	event_open(init_event, &init_eproc.event_pool, do_init, NULL);
	
	kmm_init();
	trap_init();

	hpet_init();
	timer_init();

	rand_init();

	local_intr_enable_hw;
	timer_measure();

	do_idle();
	/* PANIC - IDLE ENDS HERE */
	kprintf("[%d]IDLE EXIT\n", lapic_id);
	while (1) asm volatile ("hlt");
}
