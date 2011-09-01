#include <driver/timer.h>
#include <intr.h>
#include <trap/trap.h>
#include <proc/proc.h>
#include <clock.h>
#include <libs/crh.h>
#include <proc/event.h>
#include <pmm.h>
#include <debug/io.h>
#include <mp/mp.h>
#include <driver/hpet.h>
#include <init.h>

PLS crh_s crh;

PLS volatile uint64_t timer_tick;
PLS volatile uint64_t timer_freq;

#define TIMER_CLOSED  0
#define TIMER_OPEN    1

#define CRH_NODE_TO_TIMER(node) ((timer_t)								\
								 ((char *)(node) - (uintptr_t)&(((timer_t)0)->crh_node)))

static void
timer_queue_append(crh_node_t node)
{
	if (node != NULL)
	{
		crh_node_t cur = node;
		while (1)
		{
			timer_t t = CRH_NODE_TO_TIMER(cur);
			t->status = TIMER_CLOSED;
			if (cur->next == cur)
			{
				node = NULL;
				event_activate(&t->event);
				break;
			}

			cur->next->prev = cur->prev;
			cur->prev->next = cur->next;

			if (cur == node)
			{
				node = cur = cur->next;
				event_activate(&t->event);
			}
			else
			{
				cur = cur->next;
				event_activate(&t->event);

				if (cur == node) break;
			}
		}
	}
}

PLS uint64_t old_hpet_tick;
PLS uint64_t old_tick;
PLS uint64_t new_hpet_tick;
PLS uint64_t new_tick;

#define MEASURE_TICK 200

PLS static timer_s measure_timer;

static void
measure_event(event_t e)
{
	new_hpet_tick = *hpet_tick;
	new_tick = timer_tick;
	
	uint64_t freq = (new_tick - old_tick) / ((double)(new_hpet_tick - old_hpet_tick) / hpet_tick_freq) * 2 + 1;
	timer_freq = freq >> 1;

	event_activate(init_event);
}

void
timer_measure(void)
{
	old_hpet_tick = *hpet_tick;
	old_tick = timer_tick;
	
	event_open(&measure_timer.event, &proc_current->event_pool, measure_event, NULL);
	timer_open(&measure_timer, old_tick + MEASURE_TICK);
}

void
timer_open(timer_t timer, uint64_t tick)
{
	timer->status       = TIMER_OPEN;
	timer->crh_node.key = tick;
	
	local_irq_save();
	if (crh_insert(&crh, &timer->crh_node) != 0)
	{
		timer->crh_node.next = timer->crh_node.prev = &timer->crh_node;
		timer_queue_append(&timer->crh_node);
	}
	local_irq_restore();
}

void
timer_close(timer_t timer)
{
	local_irq_save();
	switch (timer->status)
	{
	case TIMER_OPEN:
		crh_remove(&crh, &timer->crh_node);
		break;
	}
	timer->status = TIMER_CLOSED;
	local_irq_restore();
}

int
timer_is_closed(timer_t timer)
{
	return timer->status == TIMER_CLOSED;
}

static void
tick_handler(int irq_no, uint64_t tick)
{
	if (proc_current->time_slice > tick)
		proc_current->time_slice -= tick;
	else
	{
		proc_current->time_slice = 0;
	}

	while (tick > 0)
	{
		uint64_t s = crh_max_step(&crh);
		if (s == 0)
		{
			/* XXX error if the return is not NULL */
			crh_set_base(&crh, timer_tick += tick);
			break;
		}
		  
		if (tick < s) s = tick;
		crh_node_t node = crh_set_base(&crh, timer_tick += s);
		timer_queue_append(node);

		tick -= s;
	}
}

int
timer_init(void)
{
	crh_init(&crh);
	crh_set_base(&crh, 0);
	timer_tick = 0;

	trap_irq_handler_set(IRQ_TIMER, tick_handler);
	tick_init(100);

	return 0;
}
