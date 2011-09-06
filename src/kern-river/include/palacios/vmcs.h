/* 
 * This file is part of the Palacios Virtual Machine Monitor developed
 * by the V3VEE Project with funding from the United States National 
 * Science Foundation and the Department of Energy.  
 *
 * The V3VEE Project is a joint project between Northwestern University
 * and the University of New Mexico.  You can find out more at 
 * http://www.v3vee.org
 *
 * Copyright (c) 2008, Peter Dinda <pdinda@northwestern.edu> 
 * Copyright (c) 2008, Jack Lange <jarusl@cs.northwestern.edu> 
 * Copyright (c) 2008, The V3VEE Project <http://www.v3vee.org> 
 * All rights reserved.
 *
 * Author: Peter Dinda <pdinda@northwestern.edu>
 *         Jack Lange <jarusl@cs.northwestern.edu>
 *
 * This is free software.  You are permitted to use,
 * redistribute, and modify it as specified in the file "V3VEE_LICENSE".
 */


#ifndef __VMCS_H__
#define __VMCS_H__

#ifdef __V3VEE__


#include <palacios/vmm_types.h>
#include <palacios/vm_guest.h>

/* VM-Exit Controls */
/* INTEL MANUAL: 20-16 vol. 3B */
#define   HOST_ADDR_SPACE_SIZE          0x00000200
#define   ACK_IRQ_ON_EXIT               0x00008000

/* Control register exit masks */
#define   CR4_VMXE      0x00002000




typedef enum {
    VMCS_GUEST_ES_SELECTOR       = 0x00000800,
    VMCS_GUEST_CS_SELECTOR       = 0x00000802,
    VMCS_GUEST_SS_SELECTOR       = 0x00000804,
    VMCS_GUEST_DS_SELECTOR       = 0x00000806,
    VMCS_GUEST_FS_SELECTOR       = 0x00000808,
    VMCS_GUEST_GS_SELECTOR       = 0x0000080A,
    VMCS_GUEST_LDTR_SELECTOR     = 0x0000080C,
    VMCS_GUEST_TR_SELECTOR       = 0x0000080E,
    /* 16 bit host state */
    VMCS_HOST_ES_SELECTOR        = 0x00000C00,
    VMCS_HOST_CS_SELECTOR        = 0x00000C02,
    VMCS_HOST_SS_SELECTOR        = 0x00000C04,
    VMCS_HOST_DS_SELECTOR        = 0x00000C06,
    VMCS_HOST_FS_SELECTOR        = 0x00000C08,
    VMCS_HOST_GS_SELECTOR        = 0x00000C0A,
    VMCS_HOST_TR_SELECTOR        = 0x00000C0C,
    /* 64 bit control fields */
    VMCS_IO_BITMAP_A_ADDR             = 0x00002000,
    VMCS_IO_BITMAP_A_ADDR_HIGH        = 0x00002001,
    VMCS_IO_BITMAP_B_ADDR             = 0x00002002,
    VMCS_IO_BITMAP_B_ADDR_HIGH        = 0x00002003,
    VMCS_MSR_BITMAP                   = 0x00002004,
    VMCS_MSR_BITMAP_HIGH              = 0x00002005,
    VMCS_EXIT_MSR_STORE_ADDR          = 0x00002006,
    VMCS_EXIT_MSR_STORE_ADDR_HIGH     = 0x00002007,
    VMCS_EXIT_MSR_LOAD_ADDR           = 0x00002008,
    VMCS_EXIT_MSR_LOAD_ADDR_HIGH      = 0x00002009,
    VMCS_ENTRY_MSR_LOAD_ADDR          = 0x0000200A,
    VMCS_ENTRY_MSR_LOAD_ADDR_HIGH     = 0x0000200B,
    VMCS_EXEC_PTR                     = 0x0000200C,
    VMCS_EXEC_PTR_HIGH                = 0x0000200D,
    VMCS_TSC_OFFSET                   = 0x00002010,
    VMCS_TSC_OFFSET_HIGH              = 0x00002011,
    VMCS_VAPIC_ADDR                   = 0x00002012,
    VMCS_VAPIC_ADDR_HIGH              = 0x00002013,
    VMCS_APIC_ACCESS_ADDR             = 0x00002014,
    VMCS_APIC_ACCESS_ADDR_HIGH        = 0x00002015,
    /* 64 bit guest state fields */
    VMCS_LINK_PTR                     = 0x00002800,
    VMCS_LINK_PTR_HIGH                = 0x00002801,
    VMCS_GUEST_DBG_CTL               = 0x00002802,
    VMCS_GUEST_DBG_CTL_HIGH          = 0x00002803,
    VMCS_GUEST_EFER                   = 0x00002805,
    VMCS_GUEST_EFER_HIGH              = 0x00002807,
    VMCS_GUEST_PERF_GLOBAL_CTRL       = 0x00002808,
    VMCS_GUEST_PERF_GLOBAL_CTRL_HIGH  = 0x00002809,

    VMCS_HOST_PERF_GLOBAL_CTRL        = 0x00002c04,
    VMCS_HOST_PERF_GLOBAL_CTRL_HIGH   = 0x00002c05,
    /* 32 bit control fields */
    VMCS_PIN_CTRLS                    = 0x00004000,
    VMCS_PROC_CTRLS                   = 0x00004002,
    VMCS_EXCP_BITMAP                  = 0x00004004,
    VMCS_PG_FAULT_ERR_MASK            = 0x00004006,
    VMCS_PG_FAULT_ERR_MATCH           = 0x00004008,
    VMCS_CR3_TGT_CNT                  = 0x0000400A,
    VMCS_EXIT_CTRLS                   = 0x0000400C,
    VMCS_EXIT_MSR_STORE_CNT           = 0x0000400E,
    VMCS_EXIT_MSR_LOAD_CNT            = 0x00004010,
    VMCS_ENTRY_CTRLS                  = 0x00004012,
    VMCS_ENTRY_MSR_LOAD_CNT           = 0x00004014,
    VMCS_ENTRY_INT_INFO               = 0x00004016,
    VMCS_ENTRY_EXCP_ERR               = 0x00004018,
    VMCS_ENTRY_INSTR_LEN              = 0x0000401A,
    VMCS_TPR_THRESHOLD                = 0x0000401C,
    VMCS_SEC_PROC_CTRLS               = 0x0000401e,
    /* 32 bit Read Only data fields */
    VMCS_INSTR_ERR                    = 0x00004400,
    VMCS_EXIT_REASON                  = 0x00004402,
    VMCS_EXIT_INT_INFO                = 0x00004404,
    VMCS_EXIT_INT_ERR                 = 0x00004406,
    VMCS_IDT_VECTOR_INFO              = 0x00004408,
    VMCS_IDT_VECTOR_ERR               = 0x0000440A,
    VMCS_EXIT_INSTR_LEN               = 0x0000440C,
    VMCS_EXIT_INSTR_INFO               = 0x0000440E,
    /* 32 bit Guest state fields */
    VMCS_GUEST_ES_LIMIT               = 0x00004800,
    VMCS_GUEST_CS_LIMIT               = 0x00004802,
    VMCS_GUEST_SS_LIMIT               = 0x00004804,
    VMCS_GUEST_DS_LIMIT               = 0x00004806,
    VMCS_GUEST_FS_LIMIT               = 0x00004808,
    VMCS_GUEST_GS_LIMIT               = 0x0000480A,
    VMCS_GUEST_LDTR_LIMIT             = 0x0000480C,
    VMCS_GUEST_TR_LIMIT               = 0x0000480E,
    VMCS_GUEST_GDTR_LIMIT             = 0x00004810,
    VMCS_GUEST_IDTR_LIMIT             = 0x00004812,
    VMCS_GUEST_ES_ACCESS              = 0x00004814,
    VMCS_GUEST_CS_ACCESS              = 0x00004816,
    VMCS_GUEST_SS_ACCESS              = 0x00004818,
    VMCS_GUEST_DS_ACCESS              = 0x0000481A,
    VMCS_GUEST_FS_ACCESS              = 0x0000481C,
    VMCS_GUEST_GS_ACCESS              = 0x0000481E,
    VMCS_GUEST_LDTR_ACCESS            = 0x00004820,
    VMCS_GUEST_TR_ACCESS              = 0x00004822,
    VMCS_GUEST_INT_STATE              = 0x00004824,
    VMCS_GUEST_ACTIVITY_STATE         = 0x00004826,
    VMCS_GUEST_SMBASE                 = 0x00004828,
    VMCS_GUEST_SYSENTER_CS            = 0x0000482A,
    /* 32 bit host state field */
    VMCS_HOST_SYSENTER_CS             = 0x00004C00,
    /* Natural Width Control Fields */
    VMCS_CR0_MASK                     = 0x00006000,
    VMCS_CR4_MASK                     = 0x00006002,
    VMCS_CR0_READ_SHDW                = 0x00006004,
    VMCS_CR4_READ_SHDW                = 0x00006006,
    VMCS_CR3_TGT_VAL_0                = 0x00006008,
    VMCS_CR3_TGT_VAL_1                = 0x0000600A,
    VMCS_CR3_TGT_VAL_2                = 0x0000600C,
    VMCS_CR3_TGT_VAL_3                = 0x0000600E,
    /* Natural Width Read Only Fields */
    VMCS_EXIT_QUAL                    = 0x00006400,
    VMCS_IO_RCX                       = 0x00006402,
    VMCS_IO_RSI                       = 0x00006404,
    VMCS_IO_RDI                       = 0x00006406,
    VMCS_IO_RIP                       = 0x00006408,
    VMCS_GUEST_LINEAR_ADDR            = 0x0000640A,
    /* Natural Width Guest State Fields */
    VMCS_GUEST_CR0                    = 0x00006800,
    VMCS_GUEST_CR3                    = 0x00006802,
    VMCS_GUEST_CR4                    = 0x00006804,
    VMCS_GUEST_ES_BASE                = 0x00006806,
    VMCS_GUEST_CS_BASE                = 0x00006808,
    VMCS_GUEST_SS_BASE                = 0x0000680A,
    VMCS_GUEST_DS_BASE                = 0x0000680C,
    VMCS_GUEST_FS_BASE                = 0x0000680E,
    VMCS_GUEST_GS_BASE                = 0x00006810,
    VMCS_GUEST_LDTR_BASE              = 0x00006812,
    VMCS_GUEST_TR_BASE                = 0x00006814,
    VMCS_GUEST_GDTR_BASE              = 0x00006816,
    VMCS_GUEST_IDTR_BASE              = 0x00006818,
    VMCS_GUEST_DR7                    = 0x0000681A,
    VMCS_GUEST_RSP                    = 0x0000681C,
    VMCS_GUEST_RIP                    = 0x0000681E,
    VMCS_GUEST_RFLAGS                 = 0x00006820,
    VMCS_GUEST_PENDING_DBG_EXCP       = 0x00006822,
    VMCS_GUEST_SYSENTER_ESP           = 0x00006824,
    VMCS_GUEST_SYSENTER_EIP           = 0x00006826,
    /* Natural Width Host State Fields */
    VMCS_HOST_CR0                     = 0x00006C00,
    VMCS_HOST_CR3                     = 0x00006C02,
    VMCS_HOST_CR4                     = 0x00006C04,
    VMCS_HOST_FS_BASE                 = 0x00006C06,
    VMCS_HOST_GS_BASE                 = 0x00006C08,
    VMCS_HOST_TR_BASE                 = 0x00006C0A,
    VMCS_HOST_GDTR_BASE               = 0x00006C0C,
    VMCS_HOST_IDTR_BASE               = 0x00006C0E,
    VMCS_HOST_SYSENTER_ESP            = 0x00006C10,
    VMCS_HOST_SYSENTER_EIP            = 0x00006C12,
    VMCS_HOST_RSP                     = 0x00006C14,
    VMCS_HOST_RIP                     = 0x00006C16,
} vmcs_field_t;



struct vmx_exception_bitmap {
    union {
        uint32_t value;
	struct {
	    uint_t de          : 1; // (0) divide by zero
	    uint_t db          : 1; // (1) Debug
	    uint_t nmi         : 1; // (2) Non-maskable interrupt
	    uint_t bp          : 1; // (3) Breakpoint
	    uint_t of          : 1; // (4) Overflow
	    uint_t br          : 1; // (5) Bound-Range
	    uint_t ud          : 1; // (6) Invalid-Opcode
	    uint_t nm          : 1; // (7) Device-not-available
	    uint_t df          : 1; // (8) Double Fault
	    uint_t ex9         : 1; 
	    uint_t ts          : 1; // (10) Invalid TSS
	    uint_t np          : 1; // (11) Segment-not-present
	    uint_t ss          : 1; // (12) Stack
	    uint_t gp          : 1; // (13) General Protection Fault
	    uint_t pf          : 1; // (14) Page fault
	    uint_t ex15        : 1;
	    uint_t mf          : 1; // (15) Floating point exception
	    uint_t ac          : 1; // (16) Alignment-check
	    uint_t mc          : 1; // (17) Machine Check
	    uint_t xf          : 1; // (18) SIMD floating-point
	    uint_t ex20        : 1;
	    uint_t ex21        : 1;
	    uint_t ex22        : 1;
	    uint_t ex23        : 1;
	    uint_t ex24        : 1;
	    uint_t ex25        : 1;
	    uint_t ex26        : 1;
	    uint_t ex27        : 1;
	    uint_t ex28        : 1;
	    uint_t ex29        : 1;
	    uint_t sx          : 1; // (30) Security Exception
	    uint_t ex31        : 1;
	} __attribute__ ((packed));
    } __attribute__ ((packed));
} __attribute__((packed));




/* Segment Selector Access Rights (32 bits) */
/* INTEL Manual: 20-4 vol 3B */
struct vmcs_segment {
    uint16_t selector;
    uint32_t limit;
    uint64_t base;

    union {
	uint32_t val;
	struct {
	    uint32_t    type        : 4;
	    uint32_t    desc_type   : 1; 
	    uint32_t    dpl         : 2;
	    uint32_t    present     : 1;
	    uint32_t    rsvd1       : 4;
	    uint32_t    avail       : 1;
	    uint32_t    long_mode   : 1; // CS only (64 bit active), reserved otherwise
	    uint32_t    db          : 1; 
	    uint32_t    granularity : 1;
	    uint32_t    unusable    : 1; 
	    uint32_t    rsvd2       : 15;
	} __attribute__((packed));
    } __attribute__((packed)) access;
};


struct vmcs_interrupt_state {
    union {
	uint32_t val;
	struct {
	    uint32_t    sti_blocking    : 1;
	    uint32_t    mov_ss_blocking : 1;
	    uint32_t    smi_blocking    : 1;
	    uint32_t    nmi_blocking    : 1;
	    uint32_t    rsvd1           : 28;
	} __attribute__((packed));
    } __attribute__((packed));
} __attribute__((packed));




struct vmcs_data {
    uint32_t revision ;
    uint32_t abort    ;
} __attribute__((packed));



int v3_vmcs_get_field_len(vmcs_field_t field);

const char * v3_vmcs_field_to_str(vmcs_field_t field);

void v3_print_vmcs();


int v3_vmx_save_vmcs(struct guest_info * info);
int v3_vmx_restore_vmcs(struct guest_info * info);


int v3_update_vmcs_host_state(struct guest_info * info);
int v3_update_vmcs_ctrl_fields(struct guest_info * info);


int v3_read_vmcs_segments(struct v3_segments * segs);
int v3_write_vmcs_segments(struct v3_segments * segs);
void v3_vmxseg_to_seg(struct vmcs_segment * vmcs_seg, struct v3_segment * seg);
void v3_seg_to_vmxseg(struct v3_segment * seg, struct vmcs_segment * vmcs_seg);

#endif // ! __V3VEE__


#endif 
