#ifndef __GLUE_CLOCK_H__
#define __GLUE_CLOCK_H__

#include <libs/types.h>

#define tick_init (*tick_init_ptr)
extern void tick_init(int freq);

#endif
