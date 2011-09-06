/*
 * This file is part of the Palacios Virtual Machine Monitor developed
 * by the V3VEE Project with funding from the United States National 
 * Science Foundation and the Department of Energy.  
 *
 * The V3VEE Project is a joint project between Northwestern University
 * and the University of New Mexico.  You can find out more at 
 * http://www.v3vee.org
 *
 * Copyright (c) 2008, Steven Jaconette <stevenjaconette2007@u.northwestern.edu> 
 * Copyright (c) 2008, Jack Lange <jarusl@cs.northwestern.edu> 
 * Copyright (c) 2008, The V3VEE Project <http://www.v3vee.org> 
 * All rights reserved.
 *
 * Author: Steven Jaconette <stevenjaconette2007@u.northwestern.edu>
 *
 * This is free software.  You are permitted to use,
 * redistribute, and modify it as specified in the file "V3VEE_LICENSE".
 */

#ifndef __VMM_DIRECT_PAGING_H__
#define __VMM_DIRECT_PAGING_H__

#ifdef __V3VEE__

#include <palacios/vmm_mem.h>
#include <palacios/vmm_paging.h>

int v3_init_passthrough_pts(struct guest_info * guest_info);
int v3_reset_passthrough_pts(struct guest_info * guest_info);

int v3_handle_passthrough_pagefault(struct guest_info * info, addr_t fault_addr, pf_error_t error_code);
int v3_handle_nested_pagefault(struct guest_info * info, addr_t fault_addr, pf_error_t error_code);

int v3_activate_passthrough_pt(struct guest_info * info);

int v3_invalidate_passthrough_addr(struct guest_info * info, addr_t inv_addr);
int v3_invalidate_nested_addr(struct guest_info * info, addr_t inv_addr);

#endif // ! __V3VEE__

#endif
