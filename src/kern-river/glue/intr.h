#ifndef __GLUE_INTR_H__
#define __GLUE_INTR_H__

#define intr_handler_set (*intr_handler_set_ptr)
#define irq_enable       (*irq_enable_ptr)
#define irq_disable      (*irq_disable_ptr)
#define irq_ack          (*irq_ack_ptr)

#include <trap.h>

/* It is assumed that supervisor shared same structure of trapframe
 * with kernel */
typedef void(*intr_handler_f)(struct trapframe *tf);

extern void intr_handler_set(int intr_no, intr_handler_f h);
extern void irq_enable(int irq);
extern void irq_disable(int irq);
extern void irq_ack(int irq);

#endif
