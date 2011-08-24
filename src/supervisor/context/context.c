#include <context.h>
#include <lapic.h>

extern void __context_switch(uintptr_t *from_esp, uintptr_t *from_pc, uintptr_t to_esp, uintptr_t to_pc) __attribute__((regparm(0)));
extern void __context_init(void) __attribute__((regparm(0)));

context_s a, b;

void
e1(void *arg)
{
	while (1)
	{
		cprintf("HELLO FROM e1\n");
		context_switch(&a, &b);
	}
}

void
e2(void *arg)
{
	while (1)
	{
		cprintf("HELLO FROM e2\n");
		context_switch(&b, &a);
	}
}

int
context_init(void)
{
	context_fill(&a, e1, NULL, 0, page2kva(alloc_page()));
	context_fill(&b, e2, NULL, 0, page2kva(alloc_page()));

	context_switch(NULL, &a);
	
	return 0;
}

static void
__context_deadend(context_t ctx)
{
	while (1) ;
}

void
context_fill(context_t ctx, void (*entry)(void *arg), void *arg, uintptr_t vpt, uintptr_t stk_top)
{
	if (ctx->lcpu == -1) ctx->lcpu = lapic_id();
	 
	ctx->vpt     = vpt;
	ctx->stk_top = stk_top;
	ctx->stk_ptr = ctx->stk_top;
	 
	ctx->stk_ptr -= sizeof(void *);
	*(void **)ctx->stk_ptr = ctx;

	ctx->stk_ptr -= sizeof(void *);
	*(void **)ctx->stk_ptr = NULL;

	ctx->stk_ptr -= sizeof(uintptr_t);
	*(uintptr_t *)ctx->stk_ptr = 0;
	uintptr_t ebp = ctx->stk_ptr;

	ctx->stk_ptr -= sizeof(void *);
	*(void **)ctx->stk_ptr = arg;

	ctx->stk_ptr -= sizeof(void *);
	*(void **)ctx->stk_ptr = &__context_deadend;

	ctx->stk_ptr -= sizeof(void *);
	*(void **)ctx->stk_ptr = entry;

	ctx->pc = (uintptr_t)__context_init;
}

void
context_switch(context_t old, context_t to)
{
	/* lcpu_set_kstack_top(to->lcpu, to->stk_top); */
	/* lcpu_set_vpt_delay(to->lcpu, to->vpt); */

	if (old != NULL)
	{
		__context_switch(&old->stk_ptr, &old->pc, to->stk_ptr, to->pc);
	}
	else
	{
		uintptr_t __unused;
		__context_switch(&__unused, &__unused, to->stk_ptr, to->pc);
	}
}
