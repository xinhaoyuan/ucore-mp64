#include <boot_ap.S.h>
#include <string.h>
#include <pmm.h>
#include <sysconf.h>
#include <lapic.h>

/* Provided by link script */
extern char boot_ap_entry_64[];

int
mp_init(void)
{
	memset(KADDR(BOOT_AP_ENTRY), 0, PGSIZE);
	memmove(KADDR(BOOT_AP_ENTRY), boot_ap_entry_64, PGSIZE);

	memset(KADDR(BOOT_AP_CR3), 0, 8);
	memmove(KADDR(BOOT_AP_CR3), &boot_cr3, sizeof(boot_cr3));

	memset(KADDR(BOOT_AP_LAPIC_PHYS), 0, 8);
	memmove(KADDR(BOOT_AP_LAPIC_PHYS), &sysconf.lapic_phys, sizeof(sysconf.lapic_phys));

	int i, max_apic = 0;
	for (i = 0; i != sysconf.lcpu_count; ++ i)
		if (lcpu_id_set[i] > max_apic) max_apic = lcpu_id_set[i];

	uintptr_t boot_ap_stack = page2kva(alloc_pages(max_apic));
	memset(KADDR(BOOT_AP_STACK_BASE), 0, 8);
	memmove(KADDR(BOOT_AP_STACK_BASE), &boot_ap_stack, sizeof(boot_ap_stack));
	
	int apic_id;
	for (i = 0; i != sysconf.lcpu_count; ++ i)
	{
		apic_id = lcpu_id_set[i];
		if (apic_id != sysconf.lcpu_boot)
		{
			cprintf("BOOTING CPU %d\n", apic_id);
			lapic_startap(apic_id, BOOT_AP_ENTRY);
		}
		else cprintf("SKIP CPU %d\n", apic_id);
	 }

	return 0;
}

void
ap_init(void)
{
	cprintf("Another CPU Running\n");
	while (1) ;
}
