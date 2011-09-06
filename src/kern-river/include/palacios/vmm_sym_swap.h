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

#ifndef __VMM_SYM_SWAP_H__
#define __VMM_SYM_SWAP_H__

#ifdef __V3VEE__ 
#ifdef CONFIG_SYMBIOTIC_SWAP

#include <palacios/vmm_types.h>
#include <palacios/vmm_paging.h>
#include <palacios/vmm_hashtable.h>


struct v3_swap_ops {
    void * (*get_swap_entry)(uint32_t pg_index, void * private_data);
};


struct v3_swap_dev {
    uint8_t present;

    struct v3_swap_ops * ops;

    void * private_data;
};


struct v3_sym_swap_state {
    struct v3_swap_dev devs[256];

#ifdef CONFIG_SYMBIOTIC_SWAP_TELEMETRY
    uint32_t read_faults;
    uint32_t write_faults;
    uint32_t flushes;
    uint32_t mapped_pages;
    uint32_t list_size;
#endif

    // shadow pointers
    struct hashtable * shdw_ptr_ht;
};

// Present = 0 and Dirty = 0
// fixme
static inline int is_swapped_pte32(pte32_t * pte) {
    return ((pte->present == 0) && (*(uint32_t *)pte != 0));
}



int v3_init_sym_swap(struct guest_info * info);

int v3_register_swap_disk(struct guest_info * info, int dev_index, 
			  struct v3_swap_ops * ops, void * private_data);

int v3_swap_in_notify(struct guest_info * info, int pg_index, int dev_index);


int v3_get_vaddr_perms(struct guest_info * info, addr_t vaddr, pte32_t * guest_pte, pf_error_t * page_perms);
addr_t v3_get_swapped_pg_addr(struct guest_info * info, pte32_t * guest_pte);
addr_t v3_map_swp_page(struct guest_info * info, pte32_t * shadow_pte, pte32_t * guest_pte, void * swp_page_ptr);

int v3_swap_flush(struct guest_info * info);


#endif
#endif
#endif
