#include <kern.h>
#include <ide.h>
#include <pmm.h>
#include <mmu.h>
#include <lapic.h>
#include <ioapic.h>
#include <lcpu.h>
#include <x86.h>
#include <spinlock.h>
#include <stdio.h>
#include <sysconf.h>

kern_bootinfo_s kern_bootinfo;
char *kern_data;

static void
read_secs(unsigned short ideno, uint32_t secno, void *dst, size_t nsecs)
{
	while (nsecs > 0)
	{
		size_t curn = nsecs > 64 ? 64 : nsecs;
		ide_read_secs(ideno, secno, dst, curn);

		nsecs -= curn;
		secno += curn;
		dst    = (char *)dst + curn * SECTSIZE;
	}
}

void
load_kern(void)
{
	ide_read_secs(0, 1, &kern_bootinfo, PGSIZE / SECTSIZE);
	uintptr_t kern_data_size = kern_bootinfo.kern_bss - kern_bootinfo.kern_text;
	kern_data = page2kva(alloc_pages(kern_data_size / PGSIZE));
	read_secs(
		0,
		1 + (kern_bootinfo.kern_text - kern_bootinfo.kern_start) / SECTSIZE,
		kern_data,
		kern_data_size / SECTSIZE);
}

static spinlock_s kprintf_lock;

#define KRES_COUNT 1024

static struct {
	int lcpu;
	spinlock_s lock;
} kres[KRES_COUNT];

void
jump_kern(void)
{
	pgd_t *pgdir;
	pgdir = lcpu_static[lapic_id()].init_pgdir = (pgd_t *)page2kva(alloc_page());
	memcpy(pgdir, boot_pgdir, PGSIZE);

	pgdir[0]        = 0;
	pgdir[PGX(VPT)] = PADDR(pgdir) | PTE_P | PTE_W;

	uintptr_t i;
	for (i = kern_bootinfo.kern_text; i < kern_bootinfo.kern_end; i += PGSIZE)
	{
		pte_t *pte = get_pte(pgdir, i, 1);
		if (i < kern_bootinfo.kern_data)
		{
			/* Copy all readonly data */
			*pte = PADDR(kern_data + i - kern_bootinfo.kern_text) | PTE_W | PTE_P;
		}
		else
		{
			/* And alloc all writable data */
			*pte = page2pa(alloc_page()) | PTE_W | PTE_P;
		}
	}

	lcr3(PADDR(pgdir));
	/* Setup cr0 protection */
	uint64_t cr0 = rcr0();
    cr0 |= CR0_PE | CR0_PG | CR0_AM | CR0_WP | CR0_NE | CR0_TS | CR0_EM | CR0_MP;
    cr0 &= ~(CR0_TS | CR0_EM);
    lcr0(cr0);
	
	memmove((void *)kern_bootinfo.kern_data,
			kern_data + kern_bootinfo.kern_data - kern_bootinfo.kern_text,
			kern_bootinfo.kern_bss - kern_bootinfo.kern_data);
	memset((void *)kern_bootinfo.kern_bss,
		   0, kern_bootinfo.kern_end - kern_bootinfo.kern_bss);

	spinlock_init(&kprintf_lock);
	for (i = 0; i != KRES_COUNT; ++ i)
	{
		kres[i].lcpu = -1;
		spinlock_init(&kres[i].lock);
	}
	((void(*)(void))kern_bootinfo.kern_entry)();
	
	while (1) ;
}

#include <stdarg.h>
#include <sync.h>

int
vkprintf(const char *fmt, va_list ap)
{
	bool intr_save;
	
	local_intr_save(intr_save);
	spinlock_acquire(&kprintf_lock);
	int cnt = vcprintf(fmt, ap);
	spinlock_release(&kprintf_lock);
	local_intr_restore(intr_save);

	return cnt;
}

void
kputchar(int c)
{
	bool intr_save;
	
	local_intr_save(intr_save);
	spinlock_acquire(&kprintf_lock);
	cputchar(c);
	spinlock_release(&kprintf_lock);
	local_intr_restore(intr_save);
}

char *
kreadline(const char *prompt)
{
	return readline(prompt);
}

void
intr_handler_set(int intr_no, intr_handler_f h)
{
	if (intr_no >= IRQ_OFFSET && intr_no < IRQ_OFFSET + IRQ_COUNT &&
		intr_no != IRQ_OFFSET + IRQ_TIMER)
	{
		ioapic_disable(ioapic_id_set[0], intr_no - IRQ_OFFSET);
		irq_control[intr_no - IRQ_OFFSET].lcpu_apic_id = lapic_id();
	}
	lcpu_static[lapic_id()].intr_handler[intr_no] = h;
}

void
irq_enable(int irq_no)
{
	ioapic_enable(ioapic_id_set[0], irq_no, irq_control[irq_no].lcpu_apic_id);
}

void
irq_disable(int irq_no)
{
	ioapic_disable(ioapic_id_set[0], irq_no);
}

void
irq_ack(int irq_no)
{
	// lapic_send_eoi();
	// ioapic_send_eoi(irq_no);
}

pgd_t *
init_pgdir_get(void)
{
	return lcpu_static[lapic_id()].init_pgdir;
}

void
kpage_private_set(uintptr_t pa, void *data)
{
	pa2page(pa)->private = data;
}

void *
kpage_private_get(uintptr_t pa)
{
	return pa2page(pa)->private;
}

uintptr_t
kalloc_pages(size_t npages)
{
	return page2pa(alloc_pages(npages));
}

void
kfree_pages(uintptr_t pa, size_t npages)
{
	free_pages(pa2page(pa), npages);
}

void
tick_init(int freq)
{
	lapic_set_timer(freq);
}

int
kcons_getc(void)
{
	return cons_getc();
}

int
kacquire_try(int id)
{
	if (id < 0 || id >= KRES_COUNT) return 0;
	bool intr_flag;
	int result = lapic_id();
	
	local_intr_save(intr_flag);
	if (spinlock_acquire_try(&kres[id].lock))
	{
		kres[id].lcpu = result;
	}
	result = kres[id].lcpu == result;
	local_intr_restore(intr_flag);

	return result;
}

void
kacquire(int id)
{
	if (id < 0 || id >= KRES_COUNT) return;
	bool intr_flag;
	local_intr_save(intr_flag);
	if (kres[id].lcpu != lapic_id())
	{
		spinlock_acquire(&kres[id].lock);
		kres[id].lcpu = lapic_id();
	}
	local_intr_restore(intr_flag);
}

void
krelease(int id)
{
	if (id < 0 || id >= KRES_COUNT) return;
	bool intr_flag;
	local_intr_save(intr_flag);
	if (kres[id].lcpu == lapic_id())
	{
		kres[id].lcpu = -1;
		spinlock_release(&kres[id].lock);
	}
	local_intr_restore(intr_flag);
}

unsigned int
lcpu_idx(void)
{
	return lcpu_id_inv[lapic_id()];
}

unsigned int
lcpu_count(void)
{
	return sysconf.lcpu_count;
}

EXPORT_SYMBOL(context_fill);
EXPORT_SYMBOL(context_switch);
EXPORT_SYMBOL(vkprintf);
EXPORT_SYMBOL(kputchar);
EXPORT_SYMBOL(lapic_id);
EXPORT_SYMBOL(kreadline);
EXPORT_SYMBOL(intr_handler_set);
EXPORT_SYMBOL(irq_enable);
EXPORT_SYMBOL(irq_disable);
EXPORT_SYMBOL(irq_ack);
EXPORT_SYMBOL(init_pgdir_get);
EXPORT_SYMBOL(kpage_private_set);
EXPORT_SYMBOL(kpage_private_get);
EXPORT_SYMBOL(kalloc_pages);
EXPORT_SYMBOL(kfree_pages);
EXPORT_SYMBOL(load_rsp0);
EXPORT_SYMBOL(tick_init);
EXPORT_SYMBOL(kcons_getc);
EXPORT_SYMBOL(kacquire);
EXPORT_SYMBOL(kacquire_try);
EXPORT_SYMBOL(krelease);
EXPORT_SYMBOL(lcpu_idx);
EXPORT_SYMBOL(lcpu_count);
