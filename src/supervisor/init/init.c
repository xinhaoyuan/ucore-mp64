#include <types.h>
#include <stdio.h>
#include <string.h>
#include <console.h>
#include <kdebug.h>
#include <picirq.h>
#include <trap.h>
#include <clock.h>
#include <intr.h>
#include <pmm.h>
#include <ioapic.h>
#include <lapic.h>
#include <acpi_conf.h>

int kern_init(void) __attribute__((noreturn));

int
kern_init(void) {
    extern char edata[], end[];
    memset(edata, 0, end - edata);

    cons_init();                // init the console

    const char *message = "(THU.CST) os is loading ...";
    cprintf("%s\n\n", message);

    print_kerninfo();

    pmm_init();                 // init physical memory management

    pic_init();                 // init interrupt controller
    idt_init();                 // init interrupt descriptor table

	acpi_conf_init();
	lapic_init();
	ioapic_init();

#if 0
    clock_init();               // init clock interrupt
    intr_enable();              // enable irq interrupt

	ioapic_enable(ioapic_id_set[0], IRQ_KBD, 0);
#else
	mp_init();
#endif
	cprintf("ALL DONE!\n");
    /* do nothing */
    while (1);
}

