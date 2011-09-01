#include <intr.h>
#include <proc/proc.h>
#include <mp/mp.h>
#include <mm/kmm.h>
#include <trap/trap.h>
#include <libs/string.h>

#define SCHED_NODE_TO_PROC(sch)											\
	((proc_t)((char *)(sch) - (uintptr_t)(&((proc_t)0)->sched_node)))
#define EVENT_POOL_TO_PROC(sch)											\
	((proc_t)((char *)(sch) - (uintptr_t)(&((proc_t)0)->event_pool)))

#define PROC_TIME_SLICE_DEFAULT 50

PLS volatile proc_t proc_current;
PLS static proc_s proc_idle;

static void
proc_switch(proc_t proc)
{
	proc_t old = proc_current;
	proc_current = proc;
	context_switch(&old->kern_ctx, &proc->kern_ctx);
}

void
proc_schedule(void)
{
	local_irq_save();
	if (proc_current->time_slice == 0 ||
		proc_current->status != PROC_STATUS_RUNNABLE)
	{
		if (proc_current->type != PROC_TYPE_IDLE &&
			proc_current->status == PROC_STATUS_RUNNABLE)
			sched_attach(&proc_current->sched_node);

		struct sched_node_s *s = sched_pick();
		proc_t nproc;
		  
		if (s != NULL)
		{
			sched_detach(s);
			nproc = SCHED_NODE_TO_PROC(s);
		}
		else
		{
			nproc = &proc_idle;
		}

		nproc->time_slice = PROC_TIME_SLICE_DEFAULT;
		if (nproc != proc_current)
		{
			proc_switch(nproc);
		}
	}
	local_irq_restore();
}

static void
proc_public_entry(proc_t proc)
{
	event_loop(&proc->event_pool);
}

static void
proc_ep_touch(event_pool_t pool)
{
	/* assume intr is disabled */
	proc_t proc = EVENT_POOL_TO_PROC(pool);
	if (proc->status != PROC_STATUS_RUNNABLE)
	{
		proc->status = PROC_STATUS_RUNNABLE;
		sched_attach(&proc->sched_node);
	}
}

static void
proc_ep_exhaust(event_pool_t pool)
{
	proc_t proc = EVENT_POOL_TO_PROC(pool);
	proc->idle();
}

static void
proc_ep_stop(event_pool_t pool)
{
	// proc_t proc = EVENT_POOL_TO_PROC(pool);
	/* XXX */
}

int
proc_open(proc_t proc, const char *name, proc_idle_f idle, void *private, uintptr_t stack)
{
	if (name != NULL)
	{
		strncpy(proc->name, name, MAX_PROC_NAME);
	}
	else
	{
		strcpy(proc->name, "unnamed");
	}
	proc->name[MAX_PROC_NAME - 1] = 0;

	proc->type     = PROC_TYPE_UNKNOWN;
	proc->idle     = idle;
	proc->private  = private;

	proc->irq_save_level = 0;
	 
	context_fill(&proc->kern_ctx, (void(*)(void *))proc_public_entry, proc, stack);
	sched_node_init(&proc->sched_node);
	event_pool_init(&proc->event_pool, proc_ep_touch, proc_ep_exhaust, proc_ep_stop);
	 
	proc->time_slice = PROC_TIME_SLICE_DEFAULT;
	proc->status     = PROC_STATUS_UNINIT;

	return 0;
}

#include <kio.h>

static void
idle_idle(void)
{
	proc_current->time_slice = 0;
	proc_schedule();
}

int
proc_init(void)
{
	int err;
	if ((err = sched_init())) return err;

	/* XXX */
	event_pool_init(NULL, NULL, NULL, NULL);
	
	proc_t proc = &proc_idle;

	strcpy(proc->name, "idle");
	proc->name[MAX_PROC_NAME - 1] = 0;

	proc->type     = PROC_TYPE_IDLE;
	proc->idle     = idle_idle;
	proc->private  = NULL;

	proc->irq_save_level = 0;

	proc->kern_ctx.lcpu = lapic_id;
	proc->kern_ctx.stk_top = 0x0;
		
	sched_node_init(&proc->sched_node);
	event_pool_init(&proc->event_pool, proc_ep_touch, proc_ep_exhaust, proc_ep_stop);
	
	proc->time_slice = PROC_TIME_SLICE_DEFAULT;
	proc->status     = PROC_STATUS_RUNNABLE;

	proc_current = proc;
	return 0;
}
