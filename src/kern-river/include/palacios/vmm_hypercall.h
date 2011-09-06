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


#ifndef __VMM_HYPERCALL_H__
#define __VMM_HYPERCALL_H__

#ifdef __V3VEE__
#include <palacios/vmm_types.h>
#include <palacios/vmm_rbtree.h>

typedef struct rb_root v3_hypercall_map_t;

struct guest_info;

void v3_init_hypercall_map(struct guest_info * info);


int v3_register_hypercall(struct guest_info * info, uint_t hypercall_id, 
			  int (*hypercall)(struct guest_info * info, uint_t hcall_id, void * priv_data),
			  void * priv_data);



int v3_handle_hypercall(struct guest_info * info);



#endif

#endif
