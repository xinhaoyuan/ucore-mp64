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

#ifndef __SVM_IO_H
#define __SVM_IO_H

#ifdef __V3VEE__

#include <palacios/vm_guest.h>
#include <palacios/vmcb.h>
#include <palacios/vmm.h>


struct svm_io_info {
    uint_t type        : 1;  // (0=out, 1=in)
    uint_t rsvd        : 1;  // Must be Zero
    uint_t str         : 1;  // string based io
    uint_t rep         : 1;  // repeated io
    uint_t sz8         : 1;  // 8 bit op size
    uint_t sz16        : 1;  // 16 bit op size
    uint_t sz32        : 1;  // 32 bit op size
    uint_t addr16      : 1;  // 16 bit addr
    uint_t addr32      : 1;  // 32 bit addr
    uint_t addr64      : 1;  // 64 bit addr
    uint_t rsvd2       : 6;  // Should be Zero
    uint16_t port;           // port number
} __attribute__((packed));


int v3_init_svm_io_map(struct guest_info * info);

int v3_handle_svm_io_in(struct guest_info * info, struct svm_io_info * io_info);
int v3_handle_svm_io_ins(struct guest_info * info, struct svm_io_info * io_info);
int v3_handle_svm_io_out(struct guest_info * info, struct svm_io_info * io_info);
int v3_handle_svm_io_outs(struct guest_info * info, struct svm_io_info * io_info);

#endif // !__V3VEE__


#endif
