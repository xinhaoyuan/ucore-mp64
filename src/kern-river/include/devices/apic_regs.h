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



struct lapic_id_reg {
    union {
	uint32_t val;
	struct {
	    uint_t rsvd          : 24;
	    uint_t apic_id       : 8;
	} __attribute__((packed));
    } __attribute__((packed));
} __attribute__((packed));

struct apic_ver_reg {
    union {
	uint32_t val;
	struct {
	    uint_t ver           : 8;
	    uint_t rsvd1         : 8;
	    uint_t max_lvts      : 8;
	    uint_t rsvd2         : 7;
	    uint_t ext_reg_present : 1;
	} __attribute__((packed));
    } __attribute__((packed));
} __attribute__((packed));


struct ext_apic_ctrl_reg {
    union {
	uint32_t val;
	struct {
	    uint_t ver           : 1;
	    uint_t seoi_enable   : 1;
	    uint_t ext_id_enable : 1;
	    uint_t rsvd2         : 29;
	} __attribute__((packed));
    } __attribute__((packed));
} __attribute__((packed));



struct local_vec_tbl_reg {
    union {
	uint32_t val;
	struct {
	    uint_t vec           : 8;
	    uint_t msg_type      : 3;
	    uint_t rsvd1         : 1;
	    uint_t del_status    : 1;
	    uint_t rsvd2         : 1;
	    uint_t rem_irr       : 1;
	    uint_t trig_mode     : 1;
	    uint_t mask          : 1;
	    uint_t tmr_mode      : 1;
	    uint_t rsvd3         : 14;
	} __attribute__((packed));
    } __attribute__((packed));
} __attribute__((packed));


struct tmr_vec_tbl_reg {
    union {
	uint32_t val;
	struct {
	    uint_t vec           : 8;
	    uint_t rsvd          : 4;
	    uint_t del_status    : 1;
	    uint_t rsvd2         : 3;
	    uint_t mask          : 1;
#define APIC_TMR_ONESHOT  0
#define APIC_TMR_PERIODIC 1
	    uint_t tmr_mode      : 1;
	    uint_t rsvd3         : 14;
	} __attribute__((packed));
    } __attribute__((packed));
} __attribute__((packed));


struct tmr_div_cfg_reg {
#define APIC_TMR_DIV1    0xb // 1011
#define APIC_TMR_DIV2    0x0 // 0000
#define APIC_TMR_DIV4    0x1 // 0001
#define APIC_TMR_DIV8    0x2 // 0010
#define APIC_TMR_DIV16   0x3 // 0011
#define APIC_TMR_DIV32   0x8 // 1000
#define APIC_TMR_DIV64   0x9 // 1001
#define APIC_TMR_DIV128  0xa // 1010
    union {
	uint32_t val;
	struct {
	    uint_t div_val       : 2;
	    uint_t rsvd1         : 1;
	    uint_t div_val2      : 1;
	    uint_t rsvd2         : 28;
	} __attribute__((packed));
    } __attribute__((packed));
} __attribute__((packed));


struct lint_vec_tbl_reg {
    union {
	uint32_t val;
	struct {
	    uint_t vec           : 8;
	    uint_t msg_type      : 3;
	    uint_t rsvd1         : 1;
	    uint_t del_status    : 1;
	    uint_t rsvd2         : 1;
	    uint_t rem_irr       : 1;
	    uint_t trig_mode     : 1;
	    uint_t mask          : 1;
	    uint_t rsvd3         : 15;
	} __attribute__((packed));
    } __attribute__((packed));
} __attribute__((packed));


struct perf_ctr_loc_vec_tbl_reg {
    union {
	uint32_t val;
	struct {
	    uint_t vec           : 8;
	    uint_t msg_type      : 3;
	    uint_t rsvd1         : 1;
	    uint_t del_status    : 1;
	    uint_t rsvd2         : 3;
	    uint_t mask          : 1;
	    uint_t rsvd3         : 15;
	} __attribute__((packed));
    } __attribute__((packed));
} __attribute__((packed));


struct therm_loc_vec_tbl_reg {
    union {
	uint32_t val;
	struct {
	    uint_t vec           : 8;
	    uint_t msg_type      : 3;
	    uint_t rsvd1         : 1;
	    uint_t del_status    : 1;
	    uint_t rsvd2         : 3;
	    uint_t mask          : 1;
	    uint_t rsvd3         : 15;
	} __attribute__((packed));
    } __attribute__((packed));
} __attribute__((packed));

struct err_vec_tbl_reg {
    union {
	uint32_t val;
	struct {
	    uint_t vec           : 8;
	    uint_t msg_type      : 3;
	    uint_t rsvd1         : 1;
	    uint_t del_status    : 1;
	    uint_t rsvd2         : 3;
	    uint_t mask          : 1;
	    uint_t rsvd3         : 15;
	} __attribute__((packed));
    } __attribute__((packed));
} __attribute__((packed));


struct err_status_reg {
    union {
	uint32_t val;
	struct {
	    uint_t rsvd1         : 2;
	    uint_t sent_acc_err  : 1;
	    uint_t recv_acc_err  : 1;
	    uint_t rsvd2         : 1;
	    uint_t sent_ill_err  : 1;
	    uint_t recv_ill_err  : 1;
	    uint_t ill_reg_addr  : 1;
	    uint_t rsvd3         : 24;
	} __attribute__((packed));
    } __attribute__((packed));
} __attribute__((packed));


struct spurious_int_reg {
    union {
	uint32_t val;
	struct {
	    uint_t vec           : 8;
	    uint_t apic_soft_en  : 1;
	    uint_t foc_cpu_chk   : 1;
	    uint_t rsvd1         : 22;
	} __attribute__((packed));
    } __attribute__((packed));
} __attribute__((packed));

struct int_cmd_reg {
    union {
	uint64_t val;

	struct {
	    uint32_t lo;
	    uint32_t hi;
	} __attribute__((packed));

	struct {
	    uint_t vec           : 8;
	    uint_t msg_type      : 3;
	    uint_t dst_mode      : 1;
	    uint_t del_status    : 1;
	    uint_t rsvd1         : 1;
	    uint_t lvl           : 1;
	    uint_t trig_mode     : 1;
	    uint_t rem_rd_status : 2;
	    uint_t dst_shorthand : 2;
	    uint64_t rsvd2       : 36;
	    uint32_t dst         : 8;
	} __attribute__((packed));
    } __attribute__((packed));
} __attribute__((packed));


struct log_dst_reg {
    union {
	uint32_t val;
	struct {
	    uint_t rsvd1         : 24;
	    uint_t dst_log_id    : 8;
	} __attribute__((packed));
    } __attribute__((packed));
} __attribute__((packed));


struct dst_fmt_reg {
    union {
	uint32_t val;
	struct {
	    uint_t rsvd1         : 28;
	    uint_t model         : 4;
	} __attribute__((packed));
    } __attribute__((packed));
} __attribute__((packed));


struct arb_prio_reg {
    union {
	uint32_t val;
	struct {
	    uint_t arb_prio_sub  : 4;
	    uint_t arb_prio      : 4;
	    uint_t rsvd1         : 24;
	} __attribute__((packed));
    } __attribute__((packed));
} __attribute__((packed));




struct task_prio_reg {
    union {
	uint32_t val;
	struct {
	    uint_t task_prio_sub  : 4;
	    uint_t task_prio      : 4;
	    uint_t rsvd1         : 24;
	} __attribute__((packed));
    } __attribute__((packed));
} __attribute__((packed));



struct proc_prio_reg {
    union {
	uint32_t val;
	struct {
	    uint_t proc_prio_sub  : 4;
	    uint_t proc_prio      : 4;
	    uint_t rsvd1         : 24;
	} __attribute__((packed));
    } __attribute__((packed));
} __attribute__((packed));

struct ext_apic_feature_reg {
    union {
	uint32_t val;
	struct {
	    uint_t int_en_reg_cap : 1;
	    uint_t spec_eoi_cap   : 1;
	    uint_t ext_apic_id_cap: 1;
	    uint_t rsvd1          : 13;
	    uint_t ext_lvt_cnt    : 8;
	    uint_t rsvd2          : 8;
	} __attribute__((packed));
    } __attribute__((packed));
} __attribute__((packed));


struct spec_eoi_reg {
    union {
	uint32_t val;
	struct {
	    uint_t vec           : 8;
	    uint_t rsvd1         : 24;
	} __attribute__((packed));
    } __attribute__((packed));
} __attribute__((packed));


