#include <kern.h>
#include <ide.h>
#include <pmm.h>
#include <mmu.h>
#include <lapic.h>
#include <x86.h>

kern_bootinfo_s kern_bootinfo;
char *kern_data;

void
load_kern(void)
{
	ide_read_secs(0, 1, &kern_bootinfo, PGSIZE / SECTSIZE);
	uintptr_t kern_data_size = kern_bootinfo.kern_bss - kern_bootinfo.kern_text;
	kern_data = page2kva(alloc_pages(kern_data_size / PGSIZE));
	ide_read_secs(
		0,
		1 + (kern_bootinfo.kern_text - kern_bootinfo.kern_start) / SECTSIZE,
		kern_data,
		kern_data_size / SECTSIZE);
}

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
	
	((void(*)(void))kern_bootinfo.kern_entry)();
	
	while (1) ;
}

EXPORT_SYMBOL(cprintf);
EXPORT_SYMBOL(lapic_id);
