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
 * Author: Chang Bae <c.s.bae@u.northwestern.edu>
 *
 * This is free software.  You are permitted to use,
 * redistribute, and modify it as specified in the file "V3VEE_LICENSE".
 */

#ifndef __VMM_INSTRUMENT_H__
#define __VMM_INSTRUMENT_H__

#ifdef __V3VEE__

#ifdef CONFIG_INSTRUMENT_VMM 

#include <palacios/vmm_types.h>
#include <palacios/vmm_ringbuffer.h>


void v3_init_instrumentation() __attribute__((__no_instrument_function__));

#endif // INSTRUMENT_VMM

#endif // __V3VEE__

#endif // 


