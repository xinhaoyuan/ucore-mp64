/* 
 * This file is part of the Palacios Virtual Machine Monitor developed
 * by the V3VEE Project with funding from the United States National 
 * Science Foundation and the Department of Energy.  
 *
 * The V3VEE Project is a joint project between Northwestern University
 * and the University of New Mexico.  You can find out more at 
 * http://www.v3vee.org
 *
 * Copyright (c) 2009, Lei Xia <lxia@northwestern.edu>
 * Copyright (c) 2009, Chang Seok Bae <jhuell@gmail.com>
 * Copyright (c) 2009, The V3VEE Project <http://www.v3vee.org> 
 * All rights reserved.
 *
 * Author:  Lei Xia <lxia@northwestern.edu>
 *             Chang Seok Bae <jhuell@gmail.com>
 *
 * This is free software.  You are permitted to use,
 * redistribute, and modify it as specified in the file "V3VEE_LICENSE".
 */

#ifndef __DEVICES_PCI_H__
#define __DEVICES_PCI_H__

#ifdef __V3VEE__


#include <palacios/vmm_types.h>
#include <palacios/vmm_rbtree.h>

#include <devices/pci_types.h>

struct vm_device;


typedef enum { PCI_BAR_IO, 
	       PCI_BAR_MEM24, 
	       PCI_BAR_MEM32, 
	       PCI_BAR_MEM64_LOW, 
	       PCI_BAR_MEM64_HIGH, 
	       PCI_BAR_PASSTHROUGH,
	       PCI_BAR_NONE } pci_bar_type_t;

typedef enum {PCI_STD_DEVICE, PCI_TO_PCI_BRIDGE, PCI_CARDBUS, PCI_MULTIFUNCTION, PCI_PASSTHROUGH} pci_device_type_t;



// For the rest of the subclass codes see:
// http://www.acm.uiuc.edu/sigops/roll_your_own/7.c.1.html

#define PCI_AUTO_DEV_NUM (-1)

struct pci_device;

struct v3_pci_bar {
    pci_bar_type_t type;
    
    union {
	struct {
	    int num_pages;
	    addr_t default_base_addr;
	    int (*mem_read)(addr_t guest_addr, void * dst, uint_t length, void * private_data);
	    int (*mem_write)(addr_t guest_addr, void * src, uint_t length, void * private_data);
	};

	struct {
	    int num_ports;
	    uint16_t default_base_port;
	    int (*io_read)(ushort_t port, void * dst, uint_t length, void * private_data);
	    int (*io_write)(ushort_t port, void * src, uint_t length, void * private_data);
	};
	
	struct {
	    int (*bar_init)(int bar_num, uint32_t * dst,void * private_data);
	    int (*bar_write)(int bar_num, uint32_t * src, void * private_data);
	};
    };
    
    void * private_data;

    // Internal PCI data
    uint32_t val;
    int updated;
    uint32_t mask;
};


#define PCI_IO_MASK 0xfffffffc
#define PCI_MEM_MASK 0xfffffff0
#define PCI_MEM24_MASK 0x000ffff0

#define PCI_IO_BASE(bar_val) (bar_val & PCI_IO_MASK)
#define PCI_MEM32_BASE(bar_val) (bar_val & PCI_MEM_MASK)
#define PCI_MEM24_BASE(bar_val) (bar_val & PCI_MEM24_MASK)

struct pci_device {

    pci_device_type_t type;

    union {
	uint8_t config_space[256];

	struct {
	    struct pci_config_header config_header;
	    uint8_t config_data[192];
	} __attribute__((packed));
    } __attribute__((packed));

    struct v3_pci_bar bar[6];

    struct rb_node dev_tree_node;

    uint_t bus_num;

    union {
	uint8_t devfn;
	struct {
	    uint8_t fn_num       : 3;
	    uint8_t dev_num      : 5;
	} __attribute__((packed));
    } __attribute__((packed));

    char name[64];

    int (*config_update)(uint_t reg_num, void * src, uint_t length, void * priv_data);

    int (*cmd_update)(struct pci_device * pci_dev, uchar_t io_enabled, uchar_t mem_enabled);
    int (*ext_rom_update)(struct pci_device * pci_dev);

    int (*config_write)(uint_t reg_num, void * src, uint_t length, void * private_data);
    int (*config_read)(uint_t reg_num, void * dst, uint_t length, void * private_data);


    int ext_rom_update_flag;
    int bar_update_flag;

    void * priv_data;
};


int v3_pci_set_irq_bridge(struct vm_device * pci_bus, int bus_num,
			  int (*raise_pci_irq)(struct vm_device * dev, struct pci_device * pci_dev), 
			  int (*lower_pci_irq)(struct vm_device * dev, struct pci_device * pci_dev), 
			  struct vm_device * bridge_dev);


int v3_pci_raise_irq(struct vm_device * pci_bus, int bus_num, struct pci_device * dev);
int v3_pci_lower_irq(struct vm_device * pci_bus, int bus_num, struct pci_device * dev);

struct pci_device * 
v3_pci_register_device(struct vm_device * pci,
		       pci_device_type_t dev_type, 
		       int bus_num,
		       int dev_num,
		       int fn_num,
		       const char * name,
		       struct v3_pci_bar * bars,
		       int (*config_update)(uint_t reg_num, void * src, uint_t length, void * private_data),
		       int (*cmd_update)(struct pci_device *pci_dev, uchar_t io_enabled, uchar_t mem_enabled),
		       int (*ext_rom_update)(struct pci_device *pci_dev),
		       void * priv_data);


struct pci_device * 
v3_pci_register_passthrough_device(struct vm_device * pci,
				   int bus_num,
				   int dev_num,
				   int fn_num,
				   const char * name,
				   int (*config_write)(uint_t reg_num, void * src, uint_t length, void * private_data),
				   int (*config_read)(uint_t reg_num, void * dst, uint_t length, void * private_data),
				   void * private_data);


#endif

#endif

