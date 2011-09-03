#include <mp.h>
#include <trap.h>
#include <x86/spinlock.h>
#include <proc.h>
#include <kio.h>
#include <assert.h>

PLS int volatile lapic_id;
PLS int volatile lcpu_idx;
PLS int volatile lcpu_count;

static spinlock_s kern_lock = {0};

PLS static int volatile local_kern_locking;

#if 0
#define mp_debug(a ...) kprintf(a)
#else
#define mp_debug(a ...)
#endif

int
mp_init(void)
{
	lapic_id = lapic_id_get();
	lcpu_idx = lcpu_idx_get();
	lcpu_count = lcpu_count_get();

	local_kern_locking = 0;
	
	return 0;
}

void
kern_enter(int source)
{
	int flags;
	local_intr_save_hw(flags);
	if (local_kern_locking != 0)
	{
		mp_debug("CPU %d ERROR %d %d\n", lapic_id, source, (read_rflags() & FL_IF) != 0);
		assert(0);
	}
	
	local_kern_locking = 1;
	mp_debug("CPU %d PID %d ENTERING KERN %d %d\n", lapic_id, current == NULL ? -1 : current->pid, source, (read_rflags() & FL_IF) != 0);
	// if (!spinlock_acquire_try(&kern_lock))
	{
		// mp_debug("CPU %d WAIT FOR ENTERING KERN\n", lapic_id);
		spinlock_acquire(&kern_lock);
	}
	mp_debug("CPU %d ENTERED KERN\n", lapic_id);
	local_kern_locking = 0;
	local_intr_restore_hw(flags);
}

void
kern_leave(void)
{
	int flags;
	local_intr_save_hw(flags);
	mp_debug("CPU %d LEAVING KERN\n", lapic_id);
	spinlock_release(&kern_lock);
	local_intr_restore_hw(flags);
}

#include <glue_mp.h>
#include <glue_pmm.h>
#include <glue_memlayout.h>
#include <vmm.h>

void
mp_set_mm_pagetable(struct mm_struct *mm)
{
	pgd_t* pgd;
	int flags;

	local_intr_save_hw(flags);

	/* XXX: kernel thread */
	if (mm == NULL)
	{
		pgd = init_pgdir_get();
	}
	else
	{
		pgd = mm->pgdir;
		pgd[NPGENTRY - 1] = init_pgdir_get()[NPGENTRY - 1];

	}

	lcr3(PADDR_DIRECT(pgd));
	
	local_intr_restore_hw(flags);
}
