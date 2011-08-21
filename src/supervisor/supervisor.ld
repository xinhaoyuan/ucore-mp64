/* Simple linker script for the ucore kernel.
   See the GNU ld 'info' manual ("info ld") to learn the syntax. */

OUTPUT_FORMAT("elf64-x86-64", "elf64-x86-64", "elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)
ENTRY(kern_entry32)

KERNEL_BASE = 0xFFFF800000000000;
ENTRY32_BASE  = 0x00200000;
BOOT_AP_BASE  = 0x00008000;
BOOT_AP_MAGIC = 0x001F8000;

SECTIONS {

	. = 0x8000;

	PROVIDE(boot_ap_entry_64 = . + BOOT_AP_MAGIC + KERNEL_BASE);

    .boot_ap : AT(ADDR(.boot_ap) + BOOT_AP_MAGIC) {
        *boot_1ap.S.o*(.text .rodata .data)
    }

	. = ALIGN(0x1000);
	. += BOOT_AP_MAGIC;

    .entry32 : AT(ADDR(.entry32)) {
        *entry32.S.o*(.text .rodata .data)
    }

    . = ALIGN(0x1000);
    . += KERNEL_BASE;

    .text : AT(ADDR(.text) - KERNEL_BASE) {
        *(.text .stub .text.* .gnu.linkonce.t.*)
    }

    PROVIDE(__etext = . - KERNEL_BASE); /* Define the 'etext' symbol to this value */

    .rodata : AT(ADDR(.rodata) - KERNEL_BASE) {
        *(.rodata .rodata.* .gnu.linkonce.r.*)
    }

    /* Adjust the address for the data segment to the next page */
    . = ALIGN(0x1000);

	.kernbin : AT(ADDR(.kernbin) - KERNEL_BASE) {
		*kern-bin.o.*(.data)
	}

	. = ALIGN(0x1000);

    /* The data segment */
    .data : AT(ADDR(.data) - KERNEL_BASE) {
        *(.data)
    }

    PROVIDE(__edata = . - KERNEL_BASE);

    .bss : AT(ADDR(.bss) - KERNEL_BASE) {
        *(.bss)
    }

    PROVIDE(__end = . - KERNEL_BASE);

    /DISCARD/ : {
        *(.eh_frame .note.GNU-stack)
    }
}
