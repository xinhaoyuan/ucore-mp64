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

#ifndef __DEVICES_CONSOLE_H__
#define __DEVICES_CONSOLE_H__

#ifdef __V3VEE__


struct v3_console_ops {

    // filled in by the backend device
    int (*update_screen)(uint_t x, uint_t y, uint_t length, void * private_data);
    int (*update_cursor)(uint_t x, uint_t y, void * private_data);
    int (*scroll)(int rows, void * private_data);
};



int v3_cons_get_fb(struct vm_device * frontend_dev, uint8_t * dst, uint_t offset, uint_t length);
int v3_console_register_cga(struct vm_device * cga_dev, struct v3_console_ops * ops, void * private_data);

#endif // ! __V3VEE__


#endif
