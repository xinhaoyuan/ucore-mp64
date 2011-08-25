#include <kio.h>
#include <mp.h>
#include <intr.h>

void
kbd_intr(int n, struct trapframe *tf)
{
	kprintf("KBD\n");
}

void
__kern_entry(void)
{
	if (lapic_id() == 3)
	{
		irq_handler_set(IRQ_KBD, &kbd_intr);
		irq_enable(IRQ_KBD);
		kprintf("SET UP KBD INTR\n");
	}
	
	while (1) ;
}
