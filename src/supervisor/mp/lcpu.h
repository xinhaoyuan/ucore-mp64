#ifndef __LCPU_H__
#define __LCPU_H__

#include <lapic.h>
#include <trap.h>

typedef void(*ex_handler_f)(int ex, struct trapframe *tf);
typedef void(*irq_handler_f)(int irq, struct trapframe *tf);

typedef struct lcpu_static_s
{
	pgd_t *init_pgdir;
	ex_handler_f ex_handler[EXCEPTION_COUNT];
	irq_handler_f tick_handler;
} lcpu_static_s;

typedef struct lcpu_dynamic_s
{
} lcpu_dynamic_s;

typedef struct irq_control_s
{
	int lcpu_apic_id;
	irq_handler_f handler;
} irq_control_s;

extern irq_control_s irq_control[IRQ_COUNT];

extern unsigned int   lcpu_id_set[LAPIC_COUNT];
/* Indexed by APIC ID */
extern lcpu_static_s  lcpu_static[LAPIC_COUNT];
extern lcpu_dynamic_s lcpu_dynamic[LAPIC_COUNT];

void lcpu_init(void) __attribute__((noreturn));

#endif
