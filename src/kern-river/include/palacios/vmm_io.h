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

#ifndef __VMM_IO_H
#define __VMM_IO_H


#ifdef __V3VEE__

#include <palacios/vmm_types.h>
#include <palacios/vmm_util.h>
#include <palacios/vmm_rbtree.h>




struct guest_info;

void v3_init_io_map(struct guest_info * info);




/* External API */
int v3_hook_io_port(struct guest_info * info, uint16_t port, 
		    int (*read)(uint16_t port, void * dst, uint_t length, void * priv_data),
		    int (*write)(uint16_t port, void * src, uint_t length, void * priv_data), 
		    void * priv_data);

int v3_unhook_io_port(struct guest_info * info, uint16_t port);





struct v3_io_hook {
    uint16_t port;

    // Reads data into the IO port (IN, INS)
    int (*read)(uint16_t port, void * dst, uint_t length, void * priv_data);

    // Writes data from the IO port (OUT, OUTS)
    int (*write)(uint16_t port, void * src, uint_t length, void * priv_data);



    void * priv_data;
  
    struct rb_node tree_node;

};

struct v3_io_map {
    struct rb_root map;

    int (*update_map)(struct guest_info * info, uint16_t port, int hook_read, int hook_write);

    void * arch_data;
};

struct v3_io_hook * v3_get_io_hook(struct guest_info * info, uint16_t port);


void v3_print_io_map(struct guest_info * info);

void v3_refresh_io_map(struct guest_info * info);


void v3_outb(uint16_t port, uint8_t value);
uint8_t v3_inb(uint16_t port);

void v3_outw(uint16_t port, uint16_t value);
uint16_t v3_inw(uint16_t port);

void v3_outdw(uint16_t port, uint_t value);
uint_t v3_indw(uint16_t port);



#endif // !__V3VEE__





#endif
