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

#ifndef __VMM_EXCP_H__
#define __VMM_EXCP_H__

#ifdef __V3VEE__


#include <palacios/vmm_types.h>

#define DE_EXCEPTION          0x00  
#define DB_EXCEPTION          0x01
#define NMI_EXCEPTION         0x02
#define BP_EXCEPTION          0x03
#define OF_EXCEPTION          0x04
#define BR_EXCEPTION          0x05
#define UD_EXCEPTION          0x06
#define NM_EXCEPTION          0x07
#define DF_EXCEPTION          0x08
#define TS_EXCEPTION          0x0a
#define NP_EXCEPTION          0x0b
#define SS_EXCEPTION          0x0c
#define GPF_EXCEPTION         0x0d
#define PF_EXCEPTION          0x0e
#define MF_EXCEPTION          0x10
#define AC_EXCEPTION          0x11
#define MC_EXCEPTION          0x12
#define XF_EXCEPTION          0x13
#define SX_EXCEPTION          0x1e


struct guest_info;

struct v3_excp_state {

    /* We need to rework the exception state, to handle stacking */
    uint_t excp_pending;
    uint_t excp_num;
    uint_t excp_error_code_valid : 1;
    uint_t excp_error_code;
    
};


void v3_init_exception_state(struct guest_info * info);


int v3_raise_exception(struct guest_info * info, uint_t excp);
int v3_raise_exception_with_error(struct guest_info * info, uint_t excp, uint_t error_code);

int v3_excp_pending(struct guest_info * info);
int v3_get_excp_number(struct guest_info * info);
int v3_injecting_excp(struct guest_info * info, uint_t excp);

#endif 

#endif
