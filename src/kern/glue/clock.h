#ifndef __GLUE_CLOCK_H__
#define __GLUE_CLOCK_H__

#include <types.h>
#include <intr.h>

extern volatile size_t ticks;

#define tick_init (*tick_init_ptr)

extern void tick_init(int freq, irq_handler_f handler);

#endif
