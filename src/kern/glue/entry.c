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
		pmm_init();
		slab_init();
	}
	
	while (1) ;
}
