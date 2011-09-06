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
 * Author: Andy Gocke <agocke@gmail.com>
 *
 * This is free software.  You are permitted to use,
 * redistribute, and modify it as specified in the file "V3VEE_LICENSE".
 */

#ifndef __VMX_CTRL_REGS_H__
#define __VMX_CTRL_REGS_H__

#ifdef __V3VEE__

#include <palacios/vmx_handler.h>

struct guest_info;

int v3_vmx_handle_cr0_access(struct guest_info * info, 
			     struct vmx_exit_cr_qual * cr_qual, 
			     struct vmx_exit_info * exit_info);
int v3_vmx_handle_cr3_access(struct guest_info * info, 
			     struct vmx_exit_cr_qual * cr_qual);


#endif

#endif
