#ifndef __RIVER_MP_H__
#define __RIVER_MP_H__

#include <glue_mp.h>

extern int lapic_id;
extern int lcpu_idx;
extern int lcpu_count;

int mp_init(void);

#endif
