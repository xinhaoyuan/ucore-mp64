#include <trap/trap.h>
#include <libs/types.h>
#include <libs/x86/bitsearch.h>
#include <mp.h>
#include <intr.h>
#include <proc/proc.h>

#define IRQ_COUNT 32

PLS static uint64_t irq_mask;
PLS static uint64_t irq_accumulate[IRQ_COUNT];
PLS static irq_handler_f irq_handler[IRQ_COUNT];

static void
irq_process(void)
{
	int irq_no;
	irq_handler_f h;
	uint64_t acc;
	/* Assume irq is hw-disabled  */
	while (irq_mask)
	{
		irq_no = bsr(irq_mask);
		h      = irq_handler[irq_no];
		if (h)
		{
			acc = irq_accumulate[irq_no];
			
			irq_accumulate[irq_no] = 0;
			irq_mask ^= 1 << irq_no;

			local_intr_enable_hw;
			h(irq_no, acc);
			local_intr_disable_hw;
		}
		else
		{
			irq_mask ^= 1 << irq_no;
		}
	}
}

void
local_irq_save(void)
{
	int flag;
	local_intr_save_hw(flag);
	++ proc_current->irq_save_level;
	local_intr_restore_hw(flag);
}

void
local_irq_restore(void)
{
	int flag;
	local_intr_save_hw(flag);
	if (proc_current->irq_save_level == 1 && irq_mask)
	{
		irq_process();
		proc_schedule();
	}
	-- proc_current->irq_save_level;
	local_intr_restore_hw(flag);
}

void
trap_dispatch(struct trapframe *tf)
{
	if (tf->tf_trapno < IRQ_COUNT)
	{
		/* EXCEPTION */
		
	}
	else if (tf->tf_trapno < IRQ_OFFSET + IRQ_COUNT)
	{
		/* IRQ */
		int irq_no = tf->tf_trapno - IRQ_OFFSET;
		irq_mask |= (1 << irq_no);
		++ irq_accumulate[irq_no];
		if (proc_current->irq_save_level == 0)
		{
			++ proc_current->irq_save_level;
			
			irq_process();
			proc_schedule();
			
			-- proc_current->irq_save_level;
		}
	}
}

int
trap_init(void)
{
	int i;
	for (i = 0; i != 32; ++i)
	{
		intr_handler_set(i, trap_dispatch);
	}

	irq_mask = 0;
	for (i = 0; i != IRQ_COUNT; ++ i)
	{
		irq_accumulate[i] = 0;
		irq_handler[i] = NULL;
	}
}

void
trap_irq_handler_set(int irq_no, irq_handler_f handler)
{
	int flag;
	local_intr_save_hw(flag);
	intr_handler_set(IRQ_OFFSET + irq_no, trap_dispatch);
	irq_handler[irq_no] = handler;
	if (irq_accumulate[irq_no]) irq_mask |= (1 << irq_no);
	local_intr_restore_hw(flag);
}