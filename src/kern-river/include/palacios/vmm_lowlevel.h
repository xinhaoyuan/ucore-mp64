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

#include <palacios/vmm_types.h>


#define CPUID_FEATURE_IDS 0x00000001
#define CPUID_EXT_FEATURE_IDS 0x80000001

struct seg_selector {
    union {
	uint16_t value;
	struct {
	    uint8_t rpl     : 2;
	    uint8_t ti      : 1;
	    uint16_t si     : 13;
	} __attribute__((packed));
    } __attribute__((packed));
} __attribute__((packed));

struct gen_segment {
    uint16_t limit_lo;
    uint32_t base_lo     : 24;
    uint8_t type         : 4;
    uint8_t system       : 1;
    uint8_t dpl          : 2;
    uint8_t present      : 1;
    uint8_t limit_hi     : 4;
    uint8_t avail        : 1;
    uint8_t long_mode    : 1;
    uint8_t db           : 1;
    uint8_t granularity  : 1;
    uint8_t base_hi      : 8;
} __attribute__((packed));

struct sys_segment64 {
    uint16_t limit_lo;
    uint32_t base_lo     : 24;
    uint8_t type         : 4;
    uint8_t rsvd0        : 1;
    uint8_t dpl          : 2;
    uint8_t present      : 1;
    uint8_t limit_hi     : 4;
    uint8_t avail        : 1;
    uint8_t rsvd1        : 3;
    uint8_t granularity  : 1;
    uint64_t base_hi     : 40;
    uint32_t rsvd2;
} __attribute__((packed));



static void __inline__ v3_cpuid(uint32_t target, 
				uint32_t * eax, uint32_t * ebx, 
				uint32_t * ecx, uint32_t * edx) {
#ifdef __V3_64BIT__
    __asm__ __volatile__ (
			  "cpuid\n\t"
			  : "=a" (*eax), "=b" (*ebx), "=c" (*ecx), "=d" (*edx)
			  : "0" (target), "2" (*ecx)
			  );
#elif __V3_32BIT__
    // 32 bit code compiled with -fPIC, cannot use ebx as an ouput reg. Fantastic.
    __asm__ __volatile__ (
			  "pushl %%ebx\n\t"
			  "movl %%edi, %%ebx\n\t"
			  "cpuid\n\t"
			  "movl %%ebx, %%edi\n\t"
			  "popl %%ebx\n\t"
			  : "=a" (*eax), "=D" (*ebx), "=c" (*ecx), "=d" (*edx)
			  : "0" (target), "2" (*ecx)
			  );
#endif
    return;
}


static void __inline__ v3_set_msr(uint_t msr, uint_t high_byte, uint_t low_byte) {
    __asm__ __volatile__ (
			  "wrmsr"
			  : 
			  : "c" (msr), "d" (high_byte), "a" (low_byte)
			  );


}



static void __inline__ v3_get_msr(uint_t msr, uint_t * high_byte, uint_t * low_byte) {
    __asm__ __volatile__ (
			  "rdmsr"
			  : "=d" (*high_byte), "=a" (*low_byte) 
			  : "c" (msr)
			  );
}



static void __inline__ v3_enable_ints() {
    __asm__ __volatile__ ("sti");
}

static void __inline__ v3_disable_ints() {
    __asm__ __volatile__ ("cli");
}




#ifdef __V3_32BIT__

static addr_t __inline__ v3_irq_save() {
    addr_t state;

    __asm__ __volatile__ ("pushf \n\t"
			  "popl %0 \n\t"
			  "cli \n\t"
			  :"=g" (state)
			  : 
			  :"memory"
			  ); 
    return state;
}

static void __inline__ v3_irq_restore(addr_t state) {
    __asm__ __volatile__("pushl %0 \n\t"
			 "popfl \n\t"
			 : 
			 :"g" (state)
			 :"memory", "cc"
			 );
}

#elif __V3_64BIT__

static addr_t __inline__ v3_irq_save() {
    addr_t state; 

    __asm__ __volatile__ ("pushfq \n\t"
			  "popq %0 \n\t"
			  "cli \n\t"
			  :"=g" (state)
			  : 
			  :"memory"
			  ); 

    return state;
}


static void __inline__ v3_irq_restore(addr_t state) {
    __asm__ __volatile__("pushq %0 \n\t"
			 "popfq \n\t"
			 : 
			 :"g" (state)
			 :"memory", "cc"
			 );
}

#endif
