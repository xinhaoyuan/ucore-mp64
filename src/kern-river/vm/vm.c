#include <vm/vm.h>

#include <palacios/vmm.h>
#include <palacios/vmm_io.h>
#include <palacios/vmm_host_events.h>

#include <debug/io.h>
#include <glue_pmm.h>
#include <mm/kmm.h>
#include <proc/proc.h>
#include <libs/stdarg.h>
#include <mp/mp.h>
#include <libs/x86/spinlock.h>
#include <trap/trap.h>
#include <libs/printfmt.h>

#define assert(a) do { if (!a) { kprintf("ASSERT %s FAILED\n", #a); while (1) ; } } while (0)

static struct guest_info *irq_guest_map[IRQ_COUNT];

static void
palacios_dispatch_interrupt(struct trapframe *tf)
{
	struct v3_interrupt intr = {
		.irq		= tf->tf_trapno - IRQ_OFFSET,
		.error		= tf->tf_err,
		.should_ack	= 1,
	};

	struct guest_info *vm = irq_guest_map[intr.irq];
	if (vm)
		v3_deliver_irq(vm, &intr);
}

static void
palacios_print(const char *fmt, ...)
{
#if 0
#define FMT_LEN 120
	char new_fmt[FMT_LEN];
	new_fmt[snprintf(new_fmt, FMT_LEN, "[PALACIOS] %s", fmt)] = 0;
	
	va_list ap;
	va_start(ap, fmt);
	vkprintf(new_fmt, ap);
	va_end(ap);
#endif
}

static void *
palacios_allocate_pages(int pages)
{
	// palacios_print("alloc pages %d\n", pages);
	uintptr_t result = kalloc_pages(pages);
	// palacios_print("  result = %p\n", result);
	return (void *)result;
}

static void
palacios_free_page(void *page_phys)
{
	// palacios_print("free_page %p\n", page_phys);
	kfree_pages((uintptr_t)page_phys, 1);
}

static void *
palacios_alloc(unsigned int size)
{
	// palacios_print("alloc %d\n", size);
	void *result = kalloc(size);
	// palacios_print("  result = %p\n", result);
	return result;
}

static void
palacios_free(void *ptr)
{
	// palacios_print("free %p\n", ptr);
	return kfree(ptr);
}

static void *
palacios_vaddr_to_paddr(void *vaddr)
{
	return (void *)PADDR_DIRECT(vaddr);
}

static void *
palacios_paddr_to_vaddr(void *paddr)
{
	return (void *)KADDR_DIRECT(paddr);
}

static int
palacios_hook_interrupt(struct guest_info *vm,
						unsigned int vector)
{
	kprintf("INTR %d HOOKED\n", vector);
	irq_guest_map[vector] = vm;
	intr_handler_set(IRQ_OFFSET + vector, palacios_dispatch_interrupt);
	return 0;
}

static int
palacios_ack_interrupt(int vector)
{
	kprintf("INTR %d ACKED\n", vector);
	irq_ack(vector);
	return 0;
}

static unsigned int
palacios_get_cpu_khz(void) 
{
	return lcpu_freq / 1000;
}

static void
palacios_yield_cpu(void)
{
	proc_yield();
}

static void
__kernel_thread_entry(void *arg)
{
	((proc_entry_f)proc_current->private)(arg);
	/* XXX: clean */
}

static void 
palacios_start_kernel_thread(int (*fn)(void *arg),
							 void *arg,
							 char *thread_name)
{
	proc_t p = (proc_t)kalloc(sizeof(proc_s));
    proc_open(p, thread_name, __kernel_thread_entry, arg, fn,
			  (uintptr_t)KADDR_DIRECT(kalloc_pages(4)) + (4 << PGSHIFT));
	p->status = PROC_STATUS_WAIT;
	proc_notify(p);
}

static void 
palacios_xcall(int cpu_id, 
			   void (*fn)(void *arg),
			   void *arg)
{
	assert(cpu_id == 0);
	fn(arg);
}

static void 
palacios_start_thread_on_cpu(int cpu_id, 
							 int (*fn)(void * arg), 
							 void *arg, 
							 char *thread_name)
{
	assert(cpu_id == 0);
	palacios_start_kernel_thread(fn, arg, thread_name);
}

static unsigned int 
palacios_get_cpu(void) 
{
	return lcpu_idx;
}


static void *
palacios_mutex_alloc(void)
{
	spinlock_t lock = (spinlock_t)kalloc(sizeof(spinlock_s));
	if (lock)
		spinlock_init(lock);
	return lock;
}

static void
palacios_mutex_free(void *mutex)
{
	kfree(mutex);
}

static void 
palacios_mutex_lock(void *mutex, int must_spin)
{
	while (!spinlock_acquire_try((spinlock_t )mutex))
		proc_yield();
}

static void 
palacios_mutex_unlock(void *mutex)
{
	spinlock_release((spinlock_t )mutex);
}

static void
palacios_interrupt_cpu(struct guest_info *vm, int cpu_id)
{
	assert(cpu_id == 0);
	/* XXX */
}

static struct v3_os_hooks os_hooks =
{
	.print				 = palacios_print,
	.allocate_pages		 = palacios_allocate_pages,
	.free_page			 = palacios_free_page,
	.malloc				 = palacios_alloc,
	.free				 = palacios_free,
	.vaddr_to_paddr		 = palacios_vaddr_to_paddr,
	.paddr_to_vaddr		 = palacios_paddr_to_vaddr,
	.hook_interrupt		 = palacios_hook_interrupt,
	.ack_irq			 = palacios_ack_interrupt,
	.get_cpu_khz		 = palacios_get_cpu_khz,
	.start_kernel_thread = palacios_start_kernel_thread,
	.yield_cpu			 = palacios_yield_cpu,
	.mutex_alloc		 = palacios_mutex_alloc,
	.mutex_free			 = palacios_mutex_free,
	.mutex_lock			 = palacios_mutex_lock, 
	.mutex_unlock		 = palacios_mutex_unlock,
	.get_cpu			 = palacios_get_cpu,
	.interrupt_cpu		 = palacios_interrupt_cpu,
	.call_on_cpu		 = palacios_xcall,
	.start_thread_on_cpu = palacios_start_thread_on_cpu,
};

static struct guest_info *vm_info;

int
vm_init(void)
{
	Init_V3(&os_hooks, 1);

	extern char _binary_vm_image_start[];
	vm_info = v3_create_vm((void *)_binary_vm_image_start);

	if (!vm_info)
	{
		return -1;
	}

	return 0;
}

void
vm_start(void)
{
	v3_start_vm(vm_info, 0);
}
