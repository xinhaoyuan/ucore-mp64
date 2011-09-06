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

#ifndef __VMM_CTRL_REGS_H
#define __VMM_CTRL_REGS_H

#ifdef __V3VEE__


#include <palacios/vm_guest.h>

#define EFER_MSR                 0xc0000080

struct cr0_real {
    uint_t pe    : 1;
    uint_t mp    : 1;
    uint_t em    : 1;
    uint_t ts    : 1;
} __attribute__((packed));


struct cr0_32 {
    uint_t pe    : 1;
    uint_t mp    : 1;
    uint_t em    : 1;
    uint_t ts    : 1;
    uint_t et    : 1;
    uint_t ne    : 1;
    uint_t rsvd1 : 10;
    uint_t wp    : 1;
    uint_t rsvd2 : 1;
    uint_t am    : 1;
    uint_t rsvd3 : 10;
    uint_t nw    : 1;
    uint_t cd    : 1;
    uint_t pg    : 1;
} __attribute__((packed));


struct cr0_64 {
    uint_t pe    : 1;
    uint_t mp    : 1;
    uint_t em    : 1;
    uint_t ts    : 1;
    uint_t et    : 1;
    uint_t ne    : 1;
    uint_t rsvd1 : 10;
    uint_t wp    : 1;
    uint_t rsvd2 : 1;
    uint_t am    : 1;
    uint_t rsvd3 : 10;
    uint_t nw    : 1;
    uint_t cd    : 1;
    uint_t pg    : 1;

    uint_t  rsvd4;  // MBZ
} __attribute__((packed));


struct cr2_32 {
    uint_t pf_vaddr;
} __attribute__((packed));

struct cr2_64 {
    ullong_t pf_vaddr;
} __attribute__((packed));


struct cr3_32 {
    uint_t rsvd1             : 3;
    uint_t pwt               : 1;
    uint_t pcd               : 1;
    uint_t rsvd2             : 7;
    uint_t pdt_base_addr    : 20;
} __attribute__((packed));


struct cr3_32_PAE {
    uint_t rsvd1             : 3;
    uint_t pwt               : 1;
    uint_t pcd               : 1;
    uint_t pdpt_base_addr    : 27;
} __attribute__((packed));


struct cr3_64 {
    uint_t rsvd1             : 3;
    uint_t pwt               : 1;
    uint_t pcd               : 1;
    uint_t rsvd2             : 7;
    ullong_t pml4t_base_addr : 40;
    uint_t rsvd3             : 12; 
} __attribute__((packed));


struct cr4_32 {
    uint_t vme               : 1;
    uint_t pvi               : 1;
    uint_t tsd               : 1;
    uint_t de                : 1;
    uint_t pse               : 1;
    uint_t pae               : 1;
    uint_t mce               : 1;
    uint_t pge               : 1;
    uint_t pce               : 1;
    uint_t osf_xsr           : 1;
    uint_t osx               : 1;
    uint_t rsvd1             : 21;
} __attribute__((packed));

struct cr4_64 {
    uint_t vme               : 1;
    uint_t pvi               : 1;
    uint_t tsd               : 1;
    uint_t de                : 1;
    uint_t pse               : 1;
    uint_t pae               : 1;
    uint_t mce               : 1;
    uint_t pge               : 1;
    uint_t pce               : 1;
    uint_t osf_xsr           : 1;
    uint_t osx               : 1;
    uint_t rsvd1             : 21;
    uint_t rsvd2             : 32;
} __attribute__((packed));



struct efer_64 {
    uint_t sce              : 1;
    uint_t rsvd1            : 7; // RAZ
    uint_t lme              : 1;
    uint_t rsvd2            : 1; // MBZ
    uint_t lma              : 1;
    uint_t nxe              : 1;
    uint_t svme             : 1;
    uint_t rsvd3            : 1; // MBZ
    uint_t ffxsr            : 1;
    uint_t rsvd4            : 12; // MBZ
    uint_t rsvd5            : 32; // MBZ
} __attribute__((packed));


struct rflags {
    uint_t cf                : 1;  // carry flag
    uint_t rsvd1             : 1;  // Must be 1
    uint_t pf                : 1;  // parity flag
    uint_t rsvd2             : 1;  // Read as 0
    uint_t af                : 1;  // Auxillary flag
    uint_t rsvd3             : 1;  // Read as 0
    uint_t zf                : 1;  // zero flag
    uint_t sf                : 1;  // sign flag
    uint_t tf                : 1;  // trap flag
    uint_t intr              : 1;  // interrupt flag
    uint_t df                : 1;  // direction flag
    uint_t of                : 1;  // overflow flag
    uint_t iopl              : 2;  // IO privilege level
    uint_t nt                : 1;  // nested task
    uint_t rsvd4             : 1;  // read as 0
    uint_t rf                : 1;  // resume flag
    uint_t vm                : 1;  // Virtual-8086 mode
    uint_t ac                : 1;  // alignment check
    uint_t vif               : 1;  // virtual interrupt flag
    uint_t vip               : 1;  // virtual interrupt pending
    uint_t id                : 1;  // ID flag
    uint_t rsvd5             : 10; // Read as 0
    uint_t rsvd6             : 32; // Read as 0
} __attribute__((packed));





/*
// First opcode byte
static const uchar_t cr_access_byte = 0x0f;

// Second opcode byte
static const uchar_t lmsw_byte = 0x01;
static const uchar_t lmsw_reg_byte = 0x6;
static const uchar_t smsw_byte = 0x01;
static const uchar_t smsw_reg_byte = 0x4;
static const uchar_t clts_byte = 0x06;
static const uchar_t mov_to_cr_byte = 0x22;
static const uchar_t mov_from_cr_byte = 0x20;
*/


int v3_handle_cr0_write(struct guest_info * info);
int v3_handle_cr0_read(struct guest_info * info);

int v3_handle_cr3_write(struct guest_info * info);
int v3_handle_cr3_read(struct guest_info * info);

int v3_handle_cr4_write(struct guest_info * info);
int v3_handle_cr4_read(struct guest_info * info);


int v3_handle_efer_write(uint_t msr, struct v3_msr src, void * priv_data);
int v3_handle_efer_read(uint_t msr, struct v3_msr * dst, void * priv_data);


#endif // ! __V3VEE__


#endif
