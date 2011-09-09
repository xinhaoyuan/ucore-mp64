#ifndef __RIVER_VM_DEV_H__
#define __RIVER_VM_DEV_H__

#include <palacios/vmm.h>

#define VM_DEV_CALLBACK_HCALL   0xC0C0
#define VM_DEV_SHADOW_MEM_START 0xEF000000

void *vm_dev_init(struct guest_info *vm, unsigned int buf_psize);

#endif
