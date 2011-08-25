#ifndef __GLUE_INTR_H__
#define __GLUE_INTR_H__

#define ex_handler_set  (*ex_handler_set_ptr)
#define irq_handler_set (*irq_handler_set_ptr)
#define irq_enable      (*irq_enable_ptr)
#define irq_disable     (*irq_disable_ptr)
#define irq_ack         (*irq_ack_ptr)

#include <trap.h>

/* It is assumed that supervisor shared same structure of trapframe
 * with kernel */
typedef void(*ex_handler_f)(int ex, struct trapframe *tf);
typedef void(*irq_handler_f)(int irq, struct trapframe *tf);

extern void ex_handler_set(int ex, ex_handler_f h);
extern void irq_handler_set(int irq, irq_handler_f h);
extern void irq_enable(int irq);
extern void irq_disable(int irq);
extern void irq_ack(int irq);

#endif
