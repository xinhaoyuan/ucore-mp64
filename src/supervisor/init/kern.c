#include <kern.h>
#include <ide.h>
#include <pmm.h>
#include <mmu.h>

kern_bootinfo_s kern_bootinfo;
void *kern_data;

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
	while (1) ;
}
