#include <stdarg.h>
#include <types.h>

/* Symbols from supervisor */
typedef struct context_s
{
	 uintptr_t stk_ptr;
	 uintptr_t stk_top;

	 uintptr_t pc;
	 
	 int       lcpu;
	 uintptr_t vpt;
	 
	 struct trapframe *tf;
} context_s;

typedef context_s *context_t;

extern void (*context_switch)(context_t from, context_t to);
extern void (*context_fill)(context_t ctx,
							void (*entry)(void *arg), void *arg,
							uintptr_t vpt, uintptr_t stk_top);
extern int (*vkprintf)(const char *fmt, va_list ap);
extern int (*lapic_id)(void);

int
kprintf(const char *fmt, ...)
{
	va_list ap;
    int cnt;
    va_start(ap, fmt);
    cnt = vkprintf(fmt, ap);
    va_end(ap);
    return cnt;
}

void
__kern_entry(void)
{
	kprintf("HELLO FROM %d!\n", lapic_id());
	while (1) ;
}
