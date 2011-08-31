#include <mp.h>
#include <pmm.h>
#include <intr.h>
#include <libs/x86.h>
#include <libs/x86/bitsearch.h>
#include <libs/string.h>

#include <mm/kmm.h>

#define META_SIZE (sizeof(uintptr_t))
#define MIN_SHIFT 3
#define MIN_ALLOC (1 << MIN_SHIFT)
#define ALLOC_DELTA_SHIFT 10
#define MAX_SHIFT (MIN_SHIFT + ALLOC_DELTA_SHIFT)
#define MAX_ALLOC (MIN_ALLOC << ALLOC_DELTA_SHIFT)
#define MIN_BLOCK_PAGE 4
#define MAX_BLOCK_PAGE 256

typedef struct kmm_ctrl_s
{
	size_t    alloc_size;
	size_t    block_psize;
	uintptr_t head;
} kmm_ctrl_s;

PLS static kmm_ctrl_s ctrl[ALLOC_DELTA_SHIFT + 1];

int
kmm_init(void)
{
	int i;
	for (i = 0; i <= ALLOC_DELTA_SHIFT; ++ i)
	{
		ctrl[i].alloc_size = MIN_ALLOC << i;
		ctrl[i].head = (uintptr_t)-1;
		ctrl[i].block_psize = MIN_BLOCK_PAGE;
	}

	return 0;
}

static void *
kmm_ialloc(kmm_ctrl_s *ctrl)
{
	if (ctrl->head == (uintptr_t)-1)
	{
		uintptr_t psize = ctrl->block_psize;
		if (ctrl->block_psize < MAX_BLOCK_PAGE)
			ctrl->block_psize <<= 1;

		uintptr_t pa = kalloc_pages(psize);
		if (pa == 0) return NULL;
		void *block = KADDR_DIRECT(pa);

		memset(block, 0, psize << PGSHIFT);

		uintptr_t *tail = (uintptr_t *)(
			(uintptr_t)block +
			(((psize << PGSHIFT) / ctrl->alloc_size - 1) * (ctrl->alloc_size)));
		*tail = (uintptr_t)-1;

		ctrl->head = (uintptr_t)block;
	}
	 
	uintptr_t *head = (uintptr_t *)ctrl->head;
	ctrl->head = (*head == 0) ? (ctrl->head + ctrl->alloc_size) : (*head);

	*head = (uintptr_t)ctrl;

	return head + 1;
}

void *
kalloc(size_t size)
{
	local_irq_save();
	int size_index = bsr(size + META_SIZE - 1) - MIN_SHIFT + 1;
	 
	void *result;
	if (size_index < 0 || size_index > ALLOC_DELTA_SHIFT)
		result = NULL;
	else result = kmm_ialloc((kmm_ctrl_s *)ctrl + size_index);

	local_irq_restore();
	return result;
}

void
kfree(void *ptr)
{
	if (ptr == NULL) return;
	local_irq_save();
	 
	uintptr_t *head = ((uintptr_t *)ptr - 1);
	kmm_ctrl_s *ctrl = (kmm_ctrl_s *)*head;
	
	*head = ctrl->head;
	ctrl->head = (uintptr_t)head;
	
	local_irq_restore();
}
