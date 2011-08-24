#include <kio.h>
#include <mp.h>

void
__kern_entry(void)
{
	kprintf("HELLO FROM %d!\n", lapic_id());
	while (1) ;
}
