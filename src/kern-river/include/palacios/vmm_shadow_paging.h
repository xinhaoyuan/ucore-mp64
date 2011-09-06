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

#ifndef __VMM_SHADOW_PAGING_H__
#define __VMM_SHADOW_PAGING_H__


#ifdef __V3VEE__

#include <palacios/vmm_util.h>
#include <palacios/vmm_paging.h>
#include <palacios/vmm_hashtable.h>
#include <palacios/vmm_list.h>
#include <palacios/vmm_msr.h>


struct shadow_page_state {

    // virtualized control registers
    v3_reg_t guest_cr3;
    v3_reg_t guest_cr0;
    v3_msr_t guest_efer;

    // list of allocated shadow pages
    struct list_head page_list;


#ifdef CONFIG_SHADOW_PAGING_TELEMETRY
    uint_t guest_faults;
#endif

};



struct guest_info;


int v3_init_shadow_page_state(struct guest_info * info);


int v3_handle_shadow_pagefault(struct guest_info * info, addr_t fault_addr, pf_error_t error_code);
int v3_handle_shadow_invlpg(struct guest_info * info);


int v3_activate_shadow_pt(struct guest_info * info);
int v3_invalidate_shadow_pts(struct guest_info * info);


#endif // ! __V3VEE__

#endif
