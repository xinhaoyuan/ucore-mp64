#include <proc/ipe.h>
#include <memlayout.h>
#include <libs/types.h>
#include <mp.h>
#include <mp/mp.h>
#include <debug/io.h>
#include <pmm.h>
#include <libs/x86/atom.h>
#include <driver/rand.h>
#include <driver/timer.h>
#include <proc/proc.h>

#define RBUF_PAGES 16
#define RBUF_SIZE  (RBUF_PAGES << PGSHIFT)
#define RBUF_COUNT (RBUF_SIZE / sizeof(uintptr_t))

#define IPE_RETRY 10
#define IPE_REFRESH_INV 1

PLS static volatile char __local_recv_buffer[RBUF_SIZE] __attribute__((aligned(PGSIZE))) = {0};
PLS static volatile uintptr_t *local_recv_buffer = (uintptr_t *)__local_recv_buffer;
static volatile uintptr_t *remote_recv_buffer[LAPIC_COUNT];

PLS static proc_s ipe_proc;
PLS static event_s __ipe_event;
PLS event_t ipe_event = &__ipe_event;
PLS static timer_s ipe_timer;

static void
ipe_idle(void)
{
	proc_current->status = PROC_STATUS_WAIT;
	proc_schedule();
}

static void
do_ipe(event_t e)
{
	int i;
	for (i = 0; i != RBUF_COUNT; ++ i)
	{
		if (local_recv_buffer[i])
		{
			uintptr_t handle = local_recv_buffer[i];
			local_recv_buffer[i] = 0;
			
			if (handle != 0)
			{
				event_activate((event_t)handle);
			}
		}
	}

	if (e == &ipe_timer.event)
	{
		timer_open(&ipe_timer, timer_tick + IPE_REFRESH_INV * timer_freq);
	}
}

volatile static int ipe_ready = 0;

int
ipe_init(void)
{
	remote_recv_buffer[lapic_id] = (uintptr_t *)KADDR_DIRECT(PTE_ADDR(*VPT_ENTRY(local_recv_buffer)));
	
	proc_open(&ipe_proc, "ipe", ipe_idle, NULL, (uintptr_t)KADDR_DIRECT(kalloc_pages(4)) + 4 * PGSIZE);
	ipe_proc.status = PROC_STATUS_WAIT;
	event_open(ipe_event, &ipe_proc.event_pool, do_ipe, NULL);
	event_open(&ipe_timer.event, &ipe_proc.event_pool, do_ipe, NULL);
	timer_open(&ipe_timer, timer_tick + IPE_REFRESH_INV * timer_freq);

	/* XXX: use naive CAS here =_= for atom inc */
	while (1)
	{
		int old = ipe_ready;
		if (cmpxchg32(&ipe_ready, old, old + 1) == old) break;
	}
	
	while (ipe_ready != lcpu_count) ;
	
	return 0;
}

int
ipe_activate(int target_lapic_id, uintptr_t event_handle)
{
	if (lapic_id == target_lapic_id)
	{
		event_activate((event_t)event_handle);
		return 0;		
	}
	
	int retry;
	for (retry = 0; retry < IPE_RETRY; ++ retry)
	{
		int idx = rand16() & (RBUF_COUNT - 1);
		if (cmpxchg64(remote_recv_buffer[target_lapic_id] + idx, 0, event_handle) == 0) break;
	}

	if (retry == IPE_RETRY)
	{
		lapic_ipi_issue(target_lapic_id);
		return -1;
	}

	return 0;
}
