#include <glue/kio.h>
#include <glue/mp.h>

void
__kern_entry(void)
{
	kprintf("HELLO FROM LCPU IDX %d\n", lcpu_idx());
	while (1) ;
}
