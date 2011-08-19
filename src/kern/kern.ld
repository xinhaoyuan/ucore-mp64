OUTPUT_FORMAT("elf64-x86-64", "elf64-x86-64", "elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)
ENTRY(entry)

KERNEL_BASE = 0xFFFFFFFF80000000;

SECTIONS {

	. = KERNEL_BASE;

	PROVIDE(kern_start = .);

    .text : {
        *(.text .stub .text.* .gnu.linkonce.t.*)
    }

    .rodata : {
        *(.rodata .rodata.* .gnu.linkonce.r.*)
    }

    /* Adjust the address for the data segment to the next page */
    . = ALIGN(0x1000);

	PROVIDE(kern_ro_end = .);

    .data : {
        *(.data)
    }

    .bss : {
        *(.bss)
    }

	. = ALIGN(0x1000);

	PROVIDE(kern_end = .);

    /DISCARD/ : {
        *(.eh_frame .note.GNU-stack)
    }
}
