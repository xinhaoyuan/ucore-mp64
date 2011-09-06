/*
 * vmx_assist.h: Context definitions for the VMXASSIST world switch.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Leendert van Doorn, leendert@watson.ibm.com
 * Copyright (c) 2005, International Business Machines Corporation.
 */

#ifndef _VMX_ASSIST_H_
#define _VMX_ASSIST_H_

#ifdef __V3VEE__

#include <palacios/vm_guest.h>

#define VMXASSIST_BASE         0xD0000
#define VMXASSIST_MAGIC        0x17101966


struct vmx_assist_header {
    uint64_t rsvd; // 8 bytes of nothing
    uint32_t magic;
    uint32_t new_ctx_gpa;
    uint32_t old_ctx_gpa;
} __attribute__((packed));


union vmcs_arbytes {
    struct arbyte_fields {
        unsigned int seg_type : 4,
            s         : 1,
            dpl       : 2,
            p         : 1,
            reserved0 : 4,
            avl       : 1,
            reserved1 : 1,
            default_ops_size: 1,
            g         : 1,
            null_bit  : 1,
            reserved2 : 15;
    } __attribute__((packed)) fields;
    unsigned int bytes;
} __attribute__((packed));

struct vmx_assist_segment {
    uint32_t sel;
    uint32_t limit;
    uint32_t base;
    union vmcs_arbytes arbytes;
} __attribute__((packed));

/*
 * World switch state
 */
struct vmx_assist_context {
    uint32_t  eip;        /* execution pointer */
    uint32_t  esp;        /* stack pointer */
    uint32_t  eflags;     /* flags register */
    uint32_t  cr0;
    uint32_t  cr3;        /* page table directory */
    uint32_t  cr4;

    uint32_t  idtr_limit; /* idt */
    uint32_t  idtr_base;

    uint32_t  gdtr_limit; /* gdt */
    uint32_t  gdtr_base;

    struct vmx_assist_segment cs;
    struct vmx_assist_segment ds;
    struct vmx_assist_segment es;
    struct vmx_assist_segment ss;
    struct vmx_assist_segment fs;
    struct vmx_assist_segment gs;
    struct vmx_assist_segment tr;
    struct vmx_assist_segment ldtr;


    unsigned char rm_irqbase[2];
} __attribute__((packed));

typedef struct vmx_assist_context vmx_assist_context_t;

int v3_vmxassist_ctx_switch(struct guest_info * info);


#endif

#endif /* _VMX_ASSIST_H_ */

/*
 * Local variables:
 * mode: C
 * c-set-style: "BSD"
 * c-basic-offset: 4
 * tab-width: 4
 * indent-tabs-mode: nil
 * End:
 */
