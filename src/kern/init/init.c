#include <types.h>
#include <stdio.h>
#include <string.h>
#include <kdebug.h>
#include <trap.h>
#include <intr.h>
#include <pmm.h>
#include <vmm.h>
#include <ide.h>
#include <fs.h>
#include <swap.h>
#include <proc.h>
#include <sched.h>
#include <kio.h>

int kern_init(void) __attribute__((noreturn));

int
kern_init(void) {
#if 0
    extern char edata[], end[];
    memset(edata, 0, end - edata);
	
    cons_init();                // init the console
#endif

    const char *message = "(THU.CST) os is loading ...";
    kprintf("%s\n\n", message);

    print_kerninfo();

    pmm_init();                 // init physical memory management

#if 0
    pic_init();                 // init interrupt controller
    idt_init();                 // init interrupt descriptor table
#endif

    vmm_init();                 // init virtual memory management
    sched_init();               // init scheduler
    proc_init();                // init process table
    sync_init();                // init sync struct

    ide_init();                 // init ide devices
    swap_init();                // init swap
    fs_init();                  // init fs
#if 0
    clock_init();               // init clock interrupt
#endif
    intr_enable();              // enable irq interrupt

    cpu_idle();                 // run idle process
}

