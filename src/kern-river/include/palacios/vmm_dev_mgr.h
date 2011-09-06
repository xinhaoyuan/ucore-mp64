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

#ifndef _VMM_DEV_MGR
#define _VMM_DEV_MGR

#ifdef __V3VEE__

#include <palacios/vmm_types.h>
#include <palacios/vmm_list.h>
#include <palacios/vmm_string.h>
#include <palacios/vmm_hashtable.h>
#include <palacios/vmm_config.h>


struct guest_info;

struct v3_device_ops;


struct vm_device {
    char name[32];
  
    void * private_data;

    struct v3_device_ops * ops;

    struct guest_info * vm;

    struct list_head dev_link;


    uint_t num_io_hooks;
    struct list_head io_hooks;
};


struct vmm_dev_mgr {
    uint_t num_devs;
    struct list_head dev_list;
    struct hashtable * dev_table;

    struct list_head blk_list;
    struct hashtable * blk_table;

    struct list_head net_list;
    struct hashtable * net_table;

    struct list_head console_list;
    struct hashtable * console_table;

};




int v3_create_device(struct guest_info * info, const char * dev_name, v3_cfg_tree_t * cfg);
void v3_free_device(struct vm_device * dev);


struct vm_device * v3_find_dev(struct guest_info * info, const char * dev_name);


// Registration of devices

//
// The following device manager functions should only be called
// when the guest is stopped
//



int v3_init_dev_mgr(struct guest_info * info);
int v3_dev_mgr_deinit(struct guest_info * info);




int v3_init_devices();


struct v3_device_ops {
    int (*free)(struct vm_device *dev);


    int (*reset)(struct vm_device *dev);

    int (*start)(struct vm_device *dev);
    int (*stop)(struct vm_device *dev);


    //int (*save)(struct vm_device *dev, struct *iostream);
    //int (*restore)(struct vm_device *dev, struct *iostream);
};






int v3_dev_hook_io(struct vm_device   *dev,
		   ushort_t            port,
		   int (*read)(ushort_t port, void * dst, uint_t length, struct vm_device * dev),
		   int (*write)(ushort_t port, void * src, uint_t length, struct vm_device * dev));

int v3_dev_unhook_io(struct vm_device   *dev,
		     ushort_t            port);


int v3_attach_device(struct guest_info * vm, struct vm_device * dev);
int v3_detach_device(struct vm_device * dev);

struct vm_device * v3_allocate_device(char * name, struct v3_device_ops * ops, void * private_data);


struct v3_device_info {
    char * name;
    int (*init)(struct guest_info * info, v3_cfg_tree_t * cfg);
};


#define device_register(name, init_dev_fn)				\
    static char _v3_device_name[] = name;				\
    static struct v3_device_info _v3_device				\
    __attribute__((__used__))						\
	__attribute__((unused, __section__ ("_v3_devices"),		\
		       aligned(sizeof(addr_t))))			\
	= {_v3_device_name , init_dev_fn};




void v3_print_dev_mgr(struct guest_info * info);


struct v3_dev_blk_ops {
    uint64_t (*get_capacity)(void * private_data);
    // Reads always operate on 2048 byte blocks
    int (*read)(uint8_t * buf, uint64_t lba, uint64_t num_bytes, void * private_data);
    int (*write)(uint8_t * buf, uint64_t lba, uint64_t num_bytes, void * private_data);
};

struct v3_dev_net_ops {
    int (*send)(uint8_t * buf, uint32_t count, void * private_data, struct vm_device *dest_dev);
    int (*receive)(uint8_t * buf, uint32_t count, void * private_data, struct vm_device *src_dev);

};

struct v3_dev_console_ops {

};

int v3_dev_add_blk_frontend(struct guest_info * info, 
			    char * name, 
			    int (*connect)(struct guest_info * info, 
					    void * frontend_data, 
					    struct v3_dev_blk_ops * ops, 
					    v3_cfg_tree_t * cfg, 
					    void * private_data), 
			    void * priv_data);
int v3_dev_connect_blk(struct guest_info * info, 
		       char * frontend_name, 
		       struct v3_dev_blk_ops * ops, 
		       v3_cfg_tree_t * cfg, 
		       void * private_data);

int v3_dev_add_net_frontend(struct guest_info * info, 
			    char * name, 
			    int (*connect)(struct guest_info * info, 
					    void * frontend_data, 
					    struct v3_dev_net_ops * ops, 
					    v3_cfg_tree_t * cfg, 
					    void * private_data), 
			    void * priv_data);

int v3_dev_connect_net(struct guest_info * info, 
		       char * frontend_name, 
		       struct v3_dev_net_ops * ops, 
		       v3_cfg_tree_t * cfg, 
		       void * private_data);


#endif // ! __V3VEE__

#endif
