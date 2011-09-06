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

#ifndef __VMM_LOCK_H__
#define __VMM_LOCK_H__

#ifdef __V3VEE__
#include <palacios/vmm_types.h>

typedef addr_t v3_lock_t;

int v3_lock_init(v3_lock_t * lock);
void v3_lock_deinit(v3_lock_t * lock);


void v3_lock(v3_lock_t lock);
void v3_unlock(v3_lock_t lock);


addr_t v3_lock_irqsave(v3_lock_t lock);
void v3_unlock_irqrestore(v3_lock_t lock, addr_t irq_state);


#endif

#endif
