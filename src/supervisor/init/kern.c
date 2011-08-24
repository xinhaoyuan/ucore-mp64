#include <kern.h>
#include <ide.h>
#include <pmm.h>
#include <mmu.h>
#include <lapic.h>
#include <x86.h>
#include <spinlock.h>

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

spinlock_s kprintf_lock;

void
jump_kern(void)
{
	pgd_t *pgdir;
	pgdir = lcpu_static[lapic_id()].init_pgdir = (pgd_t *)page2kva(alloc_page());
	memcpy(pgdir, boot_pgdir, PGSIZE);
	
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
	memmove((void *)kern_bootinfo.kern_data,
			kern_data + kern_bootinfo.kern_data - kern_bootinfo.kern_text,
			kern_bootinfo.kern_bss - kern_bootinfo.kern_data);
	memset((void *)kern_bootinfo.kern_bss,
		   0, kern_bootinfo.kern_end - kern_bootinfo.kern_bss);

	spinlock_init(&kprintf_lock);
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

EXPORT_SYMBOL(context_fill);
EXPORT_SYMBOL(context_switch);
EXPORT_SYMBOL(vkprintf);
EXPORT_SYMBOL(lapic_id);
