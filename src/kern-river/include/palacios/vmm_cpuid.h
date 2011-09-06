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


#ifndef __VMM_CPUID_H__
#define __VMM_CPUID_H__


#ifdef __V3VEE__

#include <palacios/vmm_types.h>
#include <palacios/vmm_rbtree.h>



struct guest_info;

void v3_init_cpuid_map(struct guest_info * info);


struct v3_cpuid_hook {
    uint32_t cpuid;
    
    int (*hook_fn)(struct guest_info * info, uint32_t cpuid, \
		   uint32_t * eax, uint32_t * ebx, \
		   uint32_t * ecx, uint32_t * edx, \
		   void * private_data);
    void * private_data;

    struct rb_node tree_node;
};



struct v3_cpuid_map {
    struct rb_root map;
};

void v3_print_cpuid_map(struct guest_info * info);

int v3_hook_cpuid(struct guest_info * info, uint32_t cpuid, 
		  int (*hook_fn)(struct guest_info * info, uint32_t cpuid, \
				 uint32_t * eax, uint32_t * ebx, \
				 uint32_t * ecx, uint32_t * edx, \
				 void * private_data), 
		  void * private_data);

int v3_unhook_cpuid(struct guest_info * info, uint32_t cpuid);

int v3_handle_cpuid(struct guest_info * info);



#endif

#endif
