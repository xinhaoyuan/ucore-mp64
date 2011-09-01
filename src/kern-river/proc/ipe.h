#ifndef __RIVER_IPE_H__
#define __RIVER_IPE_H__

#include <libs/types.h>
#include <proc/event.h>

extern event_t ipe_event;

int ipe_init(void);
int ipe_activate(int lapic_id, uintptr_t event_handle);

#endif
