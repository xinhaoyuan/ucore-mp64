#include <boot_ap.S.h>
#include <string.h>
#include <pmm.h>
#include <sysconf.h>
#include <lcpu.h>
#include <x86.h>
#include <trap.h>
#include <lapic.h>
#include <memlayout.h>

/* Provided by link script */
extern char boot_ap_entry_64[];

static uintptr_t boot_ap_stack;

int
mp_init(void)
{
	memset(VADDR_DIRECT(BOOT_AP_ENTRY), 0, PGSIZE);
	memmove(VADDR_DIRECT(BOOT_AP_ENTRY), boot_ap_entry_64, PGSIZE);

	memset(VADDR_DIRECT(BOOT_AP_CR3), 0, 8);
	memmove(VADDR_DIRECT(BOOT_AP_CR3), &boot_cr3, sizeof(boot_cr3));

	memset(VADDR_DIRECT(BOOT_AP_LAPIC_PHYS), 0, 8);
	memmove(VADDR_DIRECT(BOOT_AP_LAPIC_PHYS), &sysconf.lapic_phys, sizeof(sysconf.lapic_phys));

	int i, max_apic = 0;
	for (i = 0; i != sysconf.lcpu_count; ++ i)
	{
		lcpu_static[lcpu_id_set[i]].public.idx = i;
		lcpu_static[lcpu_id_set[i]].public.lapic_id = lcpu_id_set[i];
		
		lcpu_id_inv[lcpu_id_set[i]] = i;
		if (lcpu_id_set[i] > max_apic) max_apic = lcpu_id_set[i];
	}

	boot_ap_stack = (uintptr_t)page2va(alloc_pages(max_apic));
	memset(VADDR_DIRECT(BOOT_AP_STACK_BASE), 0, 8);
	memmove(VADDR_DIRECT(BOOT_AP_STACK_BASE), &boot_ap_stack, sizeof(boot_ap_stack));

	int apic_id;
	for (i = 0; i < sysconf.lcpu_count; ++ i)
	{
#if HAS_DRIVER_OS
		if (i == sysconf.lcpu_count - 1) break;
#endif
		apic_id = lcpu_id_set[i];
		if (apic_id != sysconf.lcpu_boot)
		{
			lapic_ap_start(apic_id, BOOT_AP_ENTRY);
		}
	}

#if HAS_DRIVER_OS
	extern volatile int lcpu_init_count;

	/* Set up for the Linux driver OS */
	while (lcpu_init_count < sysconf.lcpu_count - 2) ;
	extern char _binary_bzImage_start[];
	extern char _binary_bzImage_end[];

	/* ALL THE MAGIC FROM BOOT.TXT */
	struct {
		uint8_t	setup_sects;
		uint16_t	root_flags;
		uint32_t	syssize;
		uint16_t	ram_size;
#define RAMDISK_IMAGE_START_MASK	0x07FF
#define RAMDISK_PROMPT_FLAG		    0x8000
#define RAMDISK_LOAD_FLAG		    0x4000
		uint16_t	vid_mode;
		uint16_t	root_dev;
		uint16_t	boot_flag;
		uint16_t	jump;
		uint32_t	header;
		uint16_t	version;
		uint32_t	realmode_swtch;
		uint16_t	start_sys;
		uint16_t	kernel_version;
		uint8_t	type_of_loader;
		uint8_t	loadflags;
#define LOADED_HIGH	    (1<<0)
#define QUIET_FLAG	    (1<<5)
#define KEEP_SEGMENTS	(1<<6)
#define CAN_USE_HEAP	(1<<7)
		uint16_t	setup_move_size;
		uint32_t	code32_start;
		uint32_t	ramdisk_image;
		uint32_t	ramdisk_size;
		uint32_t	bootsect_kludge;
		uint16_t	heap_end_ptr;
		uint8_t	ext_loader_ver;
		uint8_t	ext_loader_type;
		uint32_t	cmd_line_ptr;
		uint32_t	initrd_addr_max;
		uint32_t	kernel_alignment;
		uint8_t	relocatable_kernel;
		uint8_t	_pad2[3];
		uint32_t	cmdline_size;
		uint32_t	hardware_subarch;
		uint64_t	hardware_subarch_data;
		uint32_t	payload_offset;
		uint32_t	payload_length;
		uint64_t	setup_data;
	} __attribute__((packed)) *hdr;

	extern char __boot_dos_entry[];
	uintptr_t boot_dos_entry = (uintptr_t)__boot_dos_entry + 0x1F8000 + SVBASE;
	
	memmove(VADDR_DIRECT(0x8000), (void *)boot_dos_entry, 0x8000);
	memmove(VADDR_DIRECT(0x10000), _binary_bzImage_start, 0x8000);
	strcpy(VADDR_DIRECT(0x1e000), "mem=%dM", RESERVED_DRIVER_OS_SIZE >> 20);
	hdr = (void *)0x101F1;
	hdr->type_of_loader = 0xFF;
	hdr->loadflags = LOADED_HIGH | CAN_USE_HEAP;
	// hdr->setup_move_size = !!!ignore
	hdr->ramdisk_size = 0;
	hdr->cmd_line_ptr = 0x1e000;
	hdr->heap_end_ptr = 0xe000 - 0x200;

	uintptr_t start = ((hdr->setup_sects ? hdr->setup_sects : 4) + 1) * 512;
	memmove(VADDR_DIRECT(0x100000), _binary_bzImage_start + start,
			_binary_bzImage_end - _binary_bzImage_start - start);

	lapic_ap_start(lcpu_id_set[sysconf.lcpu_count - 1], 0x8000);
#endif
	
	return 0;
}

/* Copy from pmm.c */
static inline void
lgdt(struct pseudodesc *pd) {
    asm volatile ("lgdt (%0)" :: "r" (pd));
    asm volatile ("movw %%ax, %%es" :: "a" (KERNEL_DS));
    asm volatile ("movw %%ax, %%ds" :: "a" (KERNEL_DS));
    // reload cs & ss
    asm volatile (
        "movq %%rsp, %%rax;"            // move %rsp to %rax
        "pushq %1;"                     // push %ss
        "pushq %%rax;"                  // push %rsp
        "pushfq;"                       // push %rflags
        "pushq %0;"                     // push %cs
        "call 1f;"                      // push %rip
        "jmp 2f;"
        "1: iretq; 2:"
        :: "i" (KERNEL_CS), "i" (KERNEL_DS));
}

void
ap_boot_init(void)
{
    lgdt(&gdt_pd);
	ltr(GD_TSS(lapic_id()));
	lidt(&idt_pd);
	lapic_init_ap();

	lcpu_init();
}
