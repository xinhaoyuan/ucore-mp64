#include <list.h>
#include <sync.h>
#include <proc.h>
#include <sched.h>
#include <stdio.h>
#include <assert.h>
#include <sched_MLFQ.h>
#include <kio.h>

static list_entry_t timer_list;

static struct sched_class *sched_class;

static struct run_queue *rq;

static inline void
sched_class_enqueue(struct proc_struct *proc) {
    if (proc != idleproc) {
        sched_class->enqueue(rq, proc);
    }
}

static inline void
sched_class_dequeue(struct proc_struct *proc) {
    sched_class->dequeue(rq, proc);
}

static inline struct proc_struct *
sched_class_pick_next(void) {
    return sched_class->pick_next(rq);
}

static void
sched_class_proc_tick(struct proc_struct *proc) {
    if (proc != idleproc) {
        sched_class->proc_tick(rq, proc);
    }
    else {
        proc->need_resched = 1;
    }
}

static struct run_queue __rq[4];

void
sched_init(void) {
    list_init(&timer_list);

    rq = __rq;
    list_init(&(rq->rq_link));
    rq->max_time_slice = 8;

    int i;
    for (i = 1; i < sizeof(__rq) / sizeof(__rq[0]); i ++) {
        list_add_before(&(rq->rq_link), &(__rq[i].rq_link));
        __rq[i].max_time_slice = rq->max_time_slice * (1 << i);
    }

    sched_class = &MLFQ_sched_class;
    sched_class->init(rq);

    kprintf("sched class: %s\n", sched_class->name);
}

#include <mp.h>

void
wakeup_proc(struct proc_struct *proc) {
    assert(proc->state != PROC_ZOMBIE);
    bool intr_flag;
    local_intr_save(intr_flag);
    {
        if (proc->state != PROC_RUNNABLE) {
            proc->state = PROC_RUNNABLE;
            proc->wait_state = 0;
            if (proc != current) {
				assert(proc->pid >= lcpu_count);
                sched_class_enqueue(proc);
            }
        }
        else {
            warn("wakeup runnable process.\n");
        }
    }
    local_intr_restore(intr_flag);
}

#include <vmm.h>

void
schedule(void) {
    bool intr_flag;
    struct proc_struct *next;
	list_entry_t head;
	
    local_intr_save(intr_flag);
    {
        current->need_resched = 0;
		if (current->mm)
		{
			assert(current->mm->lapic == lapic_id);
			current->mm->lapic = -1;
		}
        if (current->state == PROC_RUNNABLE && current->pid >= lcpu_count) {
            sched_class_enqueue(current);
        }
		list_init(&head);
		while (1)
		{
			next = sched_class_pick_next();
			if (next != NULL) sched_class_dequeue(next);

			if (next && next->mm && next->mm->lapic != -1)
			{
				list_add(&head, &(next->run_link));
			}
			else
			{
				list_entry_t *cur;
				while ((cur = list_next(&head)) != &head)
				{
					list_del_init(cur);
					sched_class_enqueue(le2proc(cur, run_link));
				}

				break;
			}
		}
        if (next == NULL) {
            next = idleproc;
        }
        next->runs ++;
		assert(!next->mm || next->mm->lapic == -1);
		if (next->mm)
			next->mm->lapic = lapic_id;
        if (next != current) {
            proc_run(next);
        }
    }
    local_intr_restore(intr_flag);
}

void
add_timer(timer_t *timer) {
    bool intr_flag;
    local_intr_save(intr_flag);
    {
        assert(timer->expires > 0 && timer->proc != NULL);
        assert(list_empty(&(timer->timer_link)));
        list_entry_t *le = list_next(&timer_list);
        while (le != &timer_list) {
            timer_t *next = le2timer(le, timer_link);
            if (timer->expires < next->expires) {
                next->expires -= timer->expires;
                break;
            }
            timer->expires -= next->expires;
            le = list_next(le);
        }
        list_add_before(le, &(timer->timer_link));
    }
    local_intr_restore(intr_flag);
}

void
del_timer(timer_t *timer) {
    bool intr_flag;
    local_intr_save(intr_flag);
    {
        if (!list_empty(&(timer->timer_link))) {
            if (timer->expires != 0) {
                list_entry_t *le = list_next(&(timer->timer_link));
                if (le != &timer_list) {
                    timer_t *next = le2timer(le, timer_link);
                    next->expires += timer->expires;
                }
            }
            list_del_init(&(timer->timer_link));
        }
    }
    local_intr_restore(intr_flag);
}

void
run_timer_list(void) {
    bool intr_flag;
    local_intr_save(intr_flag);
    {
        list_entry_t *le = list_next(&timer_list);
        if (le != &timer_list) {
            timer_t *timer = le2timer(le, timer_link);
            assert(timer->expires != 0);
            timer->expires --;
            while (timer->expires == 0) {
                le = list_next(le);
                struct proc_struct *proc = timer->proc;
                if (proc->wait_state != 0) {
                    assert(proc->wait_state & WT_INTERRUPTED);
                }
                else {
                    warn("process %d's wait_state == 0.\n", proc->pid);
                }
                wakeup_proc(proc);
                del_timer(timer);
                if (le == &timer_list) {
                    break;
                }
                timer = le2timer(le, timer_link);
            }
        }
        sched_class_proc_tick(current);
    }
    local_intr_restore(intr_flag);
}

