#ifndef __GLUE_UCORE_MP_H__
#define __GLUE_UCORE_MP_H__

#include <glue_mp.h>
#include <types.h>

extern volatile int lapic_id;
extern volatile int lcpu_idx;
extern volatile int lcpu_count;

int mp_init(void);

struct mm_struct;

void kern_enter(int source);
void kern_leave(void);
void mp_set_mm_pagetable(struct mm_struct *mm);

#endif
