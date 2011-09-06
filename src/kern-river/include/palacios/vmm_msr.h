/* 
 * This file is part of the Palacios Virtual Machine Monitor developed
 * by the V3VEE Project with funding from the United States National 
 * Science Foundation and the Department of Energy.  
 *
 * The V3VEE Project is a joint project between Northwestern University
 * and the University of New Mexico.  You can find out more at 
 * http://www.v3vee.org
 *
 * Copyright (c) 2008, Jack Lange <jarusl@cs.northwestern.edu> 
 * Copyright (c) 2008, The V3VEE Project <http://www.v3vee.org> 
 * All rights reserved.
 *
 * Author: Jack Lange <jarusl@cs.northwestern.edu>
 *
 * This is free software.  You are permitted to use,
 * redistribute, and modify it as specified in the file "V3VEE_LICENSE".
 */

#ifndef __VMM_MSR_H__
#define __VMM_MSR_H__


#ifdef __V3VEE__

#include <palacios/vmm_types.h>
#include <palacios/vmm_list.h>

struct guest_info;


struct v3_msr {

    union {
	uint64_t value;

	struct {
	    uint32_t lo;
	    uint32_t hi;
	} __attribute__((packed));
    } __attribute__((packed));
} __attribute__((packed));


typedef struct v3_msr v3_msr_t;

struct v3_msr_hook {
    uint_t msr;
  
    int (*read)(uint_t msr, struct v3_msr * dst, void * priv_data);
    int (*write)(uint_t msr, struct v3_msr src, void * priv_data);

    void * priv_data;

    struct list_head link;
};



struct v3_msr_hook;

struct v3_msr_map {
    uint_t num_hooks;
    struct list_head hook_list;

    int (*update_map)(struct guest_info * info, uint_t msr, int hook_read, int hook_write);
    void * arch_data;

};


void v3_init_msr_map(struct guest_info * info);

int v3_unhook_msr(struct guest_info * info, uint_t msr);

int v3_hook_msr(struct guest_info * info, uint_t msr,
		int (*read)(uint_t msr, struct v3_msr * dst, void * priv_data),
		int (*write)(uint_t msr, struct v3_msr src, void * priv_data), 
		void * priv_data);


struct v3_msr_hook * v3_get_msr_hook(struct guest_info * info, uint_t msr);

void v3_refresh_msr_map(struct guest_info * info);

void v3_print_msr_map(struct guest_info * info);

int v3_handle_msr_write(struct guest_info * info);

int v3_handle_msr_read(struct guest_info * info);



#endif // ! __V3VEE__

#endif
