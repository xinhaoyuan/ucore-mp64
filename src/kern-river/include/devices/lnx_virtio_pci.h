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


#ifndef __DEVICES_LNX_VIRTIO_PCI_H__
#define __DEVICES_LNX_VIRTIO_PCI_H__

#ifdef __V3VEE__


/* PCI Vendor IDs (from Qemu) */
#define VIRTIO_VENDOR_ID    0x1af4 // Redhat/Qumranet
#define VIRTIO_SUBVENDOR_ID 0x1af4 // Redhat/Qumranet
#define VIRTIO_SUBDEVICE_ID 0x1100 // Qemu

// PCI Device IDs
#define VIRTIO_NET_DEV_ID         0x1000
#define VIRTIO_BLOCK_DEV_ID       0x1001
#define VIRTIO_BALLOON_DEV_ID     0x1002
#define VIRTIO_CONSOLE_DEV_ID     0x1003
#define VIRTIO_SYMBIOTIC_DEV_ID   0x100a

#define VIRTIO_NET_SUBDEVICE_ID 1
#define VIRTIO_BLOCK_SUBDEVICE_ID 2
#define VIRTIO_BALLOON_SUBDEVICE_ID 5
#define VIRTIO_SYMBIOTIC_SUBDEVICE_ID 10


#define HOST_FEATURES_PORT 0
#define GUEST_FEATURES_PORT 4
#define VRING_PG_NUM_PORT 8
#define VRING_SIZE_PORT 12
#define VRING_Q_SEL_PORT 14
#define VRING_Q_NOTIFY_PORT 16
#define VIRTIO_STATUS_PORT 18
#define VIRTIO_ISR_PORT 19

#define VIRTIO_PAGE_SHIFT 12


/* Descriptor flags */
/* This marks a buffer as continuing via the next field. */
#define VIRTIO_NEXT_FLAG       0x1
/* This marks a buffer as write-only (otherwise read-only). */
#define VIRTIO_WR_ONLY_FLAG      0x2


/* Used Flags */
/* This means don't notify other side when buffer added. */
#define VRING_NO_NOTIFY_FLAG  0x1


/* Avail Flags */
/* This means don't interrupt guest when buffer consumed. */
#define VIRTIO_NO_IRQ_FLAG      0x1


/* ISR Flags */
#define VIRTIO_ISR_ACTIVE 0x1
#define VIRTIO_ISR_CFG_CHANGED 0x2



/* The virtio configuration space is a hybrid io/memory mapped model 
 * All IO is done via IO port accesses
 * The IO ports access fields in a virtio data structure, and the base io port 
 *    coincides with the base address of the in memory structure
 * There is a standard virtio structure of 20 bytes, followed by a 
 *    device specific structure of n bytes.
 * 
 */
struct virtio_config {
    uint32_t host_features;
    uint32_t guest_features;
    uint32_t vring_page_num;
    uint16_t vring_ring_size;
    uint16_t vring_queue_selector;
    uint16_t vring_queue_notifier;
    uint8_t status;
    uint8_t pci_isr;
} __attribute__((packed));


struct vring_desc {
    uint64_t addr_gpa;
    uint32_t length;
    uint16_t flags;
    uint16_t next;
} __attribute__((packed));

struct vring_avail {
    uint16_t flags;
    uint16_t index;
    uint16_t ring[0];
} __attribute__((packed));


struct vring_used_elem {
    uint32_t id;
    uint32_t length;
} __attribute__((packed));

struct vring_used {
    uint16_t flags;
    uint16_t index;
    struct vring_used_elem ring[0];
} __attribute__((packed));



struct virtio_queue {
    uint16_t queue_size;
  
    uint16_t cur_avail_idx;
    uint16_t last_avail_idx;
    bool idx_overflow;

    addr_t ring_desc_addr;
    addr_t ring_avail_addr;
    addr_t ring_used_addr;
  

    struct vring_desc * desc; // We can treat this as an array...
    struct vring_avail * avail;
    struct vring_used * used;

    uint32_t pfn;
};



#endif

#endif
