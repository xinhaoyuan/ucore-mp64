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


#ifndef __VMM_MEM_H
#define __VMM_MEM_H


#ifdef __V3VEE__ 


#include <palacios/vmm_types.h>

#include <palacios/vmm_paging.h>
#include <palacios/vmm_rbtree.h>


struct guest_info;



/* These are the types of physical memory address regions
 * from the perspective of the HOST
 */
typedef enum shdw_region_type { 
    SHDW_REGION_WRITE_HOOK,                 /* This region is mapped as read-only (page faults on write) */
    SHDW_REGION_FULL_HOOK,                  /* This region is mapped as not present (always generate page faults) */
    SHDW_REGION_ALLOCATED,                  /* Region is a section of host memory */
} v3_shdw_region_type_t;




struct v3_shadow_region {
    addr_t                  guest_start; 
    addr_t                  guest_end; 

    v3_shdw_region_type_t   host_type;
  
    addr_t                  host_addr; /* This either points to a host address mapping */


    /* Called when data is read from a memory page */
    int (*read_hook)(addr_t guest_addr, void * dst, uint_t length, void * priv_data);
    /* Called when data is written to a memory page */
    int (*write_hook)(addr_t guest_addr, void * src, uint_t length, void * priv_data);

    void * priv_data;

    struct rb_node tree_node;
};


typedef struct v3_shdw_map {
    struct v3_shadow_region base_region;


    addr_t hook_hva;

    struct rb_root shdw_regions;
} v3_shdw_map_t;




int v3_init_shadow_map(struct guest_info * info);
void v3_delete_shadow_map(struct guest_info * info);


int v3_add_shadow_mem(struct guest_info * guest_info, 
		      addr_t guest_addr_start,
		      addr_t guest_addr_end,
		      addr_t host_addr);

int v3_hook_full_mem(struct guest_info * info, addr_t guest_addr_start, addr_t guest_addr_end,
		     int (*read)(addr_t guest_addr, void * dst, uint_t length, void * priv_data),
		     int (*write)(addr_t guest_addr, void * src, uint_t length, void * priv_data),
		     void * priv_data);

int v3_hook_write_mem(struct guest_info * info, addr_t guest_addr_start, addr_t guest_addr_end,
		      addr_t host_addr,
		      int (*write)(addr_t guest_addr, void * src, uint_t length, void * priv_data),
		      void * priv_data);



int v3_unhook_mem(struct guest_info * info, addr_t guest_addr_start);


void v3_delete_shadow_region(struct guest_info * info, struct v3_shadow_region * reg);




struct v3_shadow_region * v3_get_shadow_region(struct guest_info * info, addr_t guest_addr);
addr_t v3_get_shadow_addr(struct v3_shadow_region * reg, addr_t guest_addr);





void v3_print_mem_map(struct guest_info * info);





const uchar_t * v3_shdw_region_type_to_str(v3_shdw_region_type_t type);



int handle_special_page_fault(struct guest_info * info, addr_t fault_addr, addr_t gp_addr, pf_error_t access_info);

int v3_handle_mem_wr_hook(struct guest_info * info, addr_t guest_va, addr_t guest_pa, 
			  struct v3_shadow_region * reg, pf_error_t access_info);
int v3_handle_mem_full_hook(struct guest_info * info, addr_t guest_va, addr_t guest_pa, 
			    struct v3_shadow_region * reg, pf_error_t access_info);

#endif /* ! __V3VEE__ */


#endif
