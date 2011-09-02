#include <mp.h>
#include <trap.h>
#include <x86/spinlock.h>
#include <proc.h>
#include <kio.h>

PLS int lapic_id;
PLS int lcpu_idx;
PLS int lcpu_count;

static spinlock_s kern_lock = {0};

PLS static int local_kern_locking;

#define mp_debug(...)

int
mp_init(void)
{
	lapic_id = lapic_id_get();
	lcpu_idx = lcpu_idx_get();
	lcpu_count = lcpu_count_get();

	local_kern_locking = 0;
	
	return 0;
}

int
kern_enter(void)
{
	int flag;
	local_intr_save_hw(flag);
	if (!local_kern_locking)
	{
		mp_debug("CPU %d ENTERING KERN\n", lapic_id);
		if (!spinlock_acquire_try(&kern_lock))
		{
			mp_debug("CPU %d WAIT FOR ENTERING KERN\n", lapic_id);
			spinlock_acquire(&kern_lock);
			mp_debug("CPU %d WAIT FINISHED\n", lapic_id);
		}

		local_kern_locking = 1;
		local_intr_restore_hw(flag);

		return 0;
	}
	else return 1;
	
}

void
kern_leave(void)
{
	int flag;
	local_intr_save_hw(flag);
	if (local_kern_locking)
	{
		mp_debug("CPU %d LEAVING KERN\n", lapic_id);
		
		local_kern_locking = 0;
		spinlock_release(&kern_lock);
	}
	local_intr_restore_hw(flag);
}
