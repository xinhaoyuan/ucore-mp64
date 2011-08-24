#include <lcpu.h>
#include <sysconf.h>
#include <ioapic.h>
#include <lapic.h>
#include <trap.h>
#include <clock.h>
#include <kern.h>

void
lcpu_init(void)
{
	if (lapic_id() == sysconf.lcpu_boot)
	{
		clock_init();               // init clock interrupt
		intr_enable();              // enable irq interrupt
		
		ioapic_enable(ioapic_id_set[0], IRQ_KBD, 0);
	}
	
	jump_kern();
}
