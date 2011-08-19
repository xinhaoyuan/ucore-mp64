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
#include <sysconf.h>
#include <x86.h>
#include <lcpu.h>

void kern_init(void) __attribute__((noreturn));

void
kern_init(void) {
    extern char edata[], end[];
    memset(edata, 0, end - edata);

    cons_init();                // init the console

    const char *message = "(THU.CST) os is loading ...";
    cprintf("%s\n\n", message);

    print_kerninfo();

	/* Get the self apic id for locating the TSS */
	uint32_t b;
	cpuid(1, NULL, &b, NULL, NULL);
	int cur_apic_id = (b >> 24) & 0xff;
	sysconf.lcpu_boot = cur_apic_id;

    pmm_init();                 // init physical memory management

    pic_init();                 // init interrupt controller
    idt_init();                 // init interrupt descriptor table

	acpi_conf_init();
	lapic_init();
	ioapic_init();

	mp_init();

	lcpu_init();
}
