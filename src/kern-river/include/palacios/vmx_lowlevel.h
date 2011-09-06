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

#ifndef __VMX_LOWLEVEL_H__
#define __VMX_LOWLEVEL_H__

#ifdef __V3VEE__

#include <palacios/vmcs.h>

#define VMX_SUCCESS         0 
#define VMX_FAIL_INVALID    1
#define VMX_FAIL_VALID      2

// vmfail macro
#define CHECK_VMXFAIL(ret_valid, ret_invalid)	\
    if (ret_valid) {				\
        return VMX_FAIL_VALID;			\
    } else if (ret_invalid) {			\
        return VMX_FAIL_INVALID;		\
    }

/* Opcode definitions for all the VM instructions */

#define VMCLEAR_OPCODE  ".byte 0x66,0xf,0xc7;" /* reg=/6 */
#define VMRESUME_OPCODE ".byte 0x0f,0x01,0xc3;"
#define VMPTRLD_OPCODE  ".byte 0x0f,0xc7;" /* reg=/6 */
#define VMPTRST_OPCODE  ".byte 0x0f,0xc7;" /* reg=/7 */
#define VMREAD_OPCODE   ".byte 0x0f,0x78;"
#define VMWRITE_OPCODE  ".byte 0x0f,0x79;"
#define VMXOFF_OPCODE   ".byte 0x0f,0x01,0xc4;"
#define VMXON_OPCODE    ".byte 0xf3,0x0f,0xc7;" /* reg=/6 */


/* Mod/rm definitions for intel registers/memory */
#define EAX_ECX_MODRM   ".byte 0xc1;"
// %eax with /6 reg
#define EAX_06_MODRM    ".byte 0x30;"
// %eax with /7 reg
#define EAX_07_MODRM    ".byte 0x38;"



static inline int v3_enable_vmx(addr_t vmxon_ptr) {
    uint64_t vmxon_ptr_64 __attribute__((aligned(8))) = (uint64_t)vmxon_ptr;
    uint8_t ret_invalid = 0;

    __asm__ __volatile__ (
                VMXON_OPCODE
                EAX_06_MODRM
                "setnaeb %0;" // fail invalid (CF=1)
                : "=q"(ret_invalid)
                : "a"(&vmxon_ptr_64),"0"(ret_invalid)
                : "memory");

    if (ret_invalid) {
        return VMX_FAIL_INVALID;
    } else {
        return VMX_SUCCESS;
    }
}

static inline int vmcs_clear(addr_t vmcs_ptr) {
    uint64_t vmcs_ptr_64 __attribute__ ((aligned(8))) = (uint64_t)vmcs_ptr;
    uint8_t ret_valid = 0;
    uint8_t ret_invalid = 0;

    __asm__ __volatile__ (
            VMCLEAR_OPCODE
            EAX_06_MODRM
            "seteb %0;" // fail valid (ZF=1)
            "setnaeb %1;" // fail invalid (CF=1)
            : "=q"(ret_valid), "=q"(ret_invalid)
            : "a"(&vmcs_ptr_64), "0"(ret_valid), "1"(ret_invalid)
            : "memory");

    CHECK_VMXFAIL(ret_valid, ret_invalid);
  
    return VMX_SUCCESS;
}

static inline int vmcs_load(addr_t vmcs_ptr) {
    uint64_t vmcs_ptr_64 = (uint64_t)vmcs_ptr;
    uint8_t ret_valid = 0;
    uint8_t ret_invalid = 0;
    
    __asm__ __volatile__ (
                VMPTRLD_OPCODE
                EAX_06_MODRM
                "seteb %0;" // fail valid (ZF=1)
                "setnaeb %1;"  // fail invalid (CF=1)
                : "=q"(ret_valid), "=q"(ret_invalid)
                : "a"(&vmcs_ptr_64), "0"(ret_valid), "1"(ret_invalid)
                : "memory");
    
    CHECK_VMXFAIL(ret_valid, ret_invalid);

    return VMX_SUCCESS;
}

static inline int vmcs_store(addr_t vmcs_ptr) {
    uint64_t vmcs_ptr_64 = (uint64_t)vmcs_ptr;

    __asm__ __volatile__ (
               VMPTRST_OPCODE
               EAX_07_MODRM
               :
               : "a"(&vmcs_ptr_64)
               : "memory");

    return VMX_SUCCESS;
}

static inline int vmcs_read(vmcs_field_t vmcs_field, void * dst) {
    addr_t val = 0;
    uint8_t ret_valid = 0;
    uint8_t ret_invalid = 0;

    __asm__ __volatile__ (  
                VMREAD_OPCODE
                EAX_ECX_MODRM
                "seteb %1;" // fail valid
                "setnaeb %1;" // fail invalid
                :  "=c"(val), "=d"(ret_valid) //, "=r"(ret_invalid) // Use ECX
                : "a" (vmcs_field), "0"(0), "1"(ret_valid)
                : "memory"
                );

    CHECK_VMXFAIL(ret_valid, ret_invalid);

    switch(v3_vmcs_get_field_len(vmcs_field))
    {
        case 2:
            *((uint16_t*)dst) = (uint16_t)val;
            break;
        case 4:
            *((uint32_t*)dst) = (uint32_t)val;
            break;
        case 8:
            *((uint64_t*)dst) = (uint64_t)val;
            break;
    }


    return VMX_SUCCESS;
}

static inline int vmcs_write(vmcs_field_t vmcs_field, addr_t value) {
    uint8_t ret_valid = 0;
    uint8_t ret_invalid = 0;

    __asm__ __volatile__ (
                VMWRITE_OPCODE
                EAX_ECX_MODRM
                "seteb %0;" // fail valid (ZF=1)
                "setnaeb %1;" // fail invalid (CF=1)
                : "=q" (ret_valid), "=q" (ret_invalid)
                : "a" (vmcs_field), "c"(value)
                : "memory");

    CHECK_VMXFAIL(ret_valid, ret_invalid);

    return VMX_SUCCESS;
}

static inline int vmx_off() {
    uint8_t ret_valid = 0;
    uint8_t ret_invalid = 0;

    __asm__ __volatile__ (
                VMXOFF_OPCODE
                "seteb %0;"
                "setnaeb %1;"
                : "=q"(ret_valid), "=q"(ret_invalid)
                : "0"(ret_valid), "1"(ret_invalid)
                : "memory");

    CHECK_VMXFAIL(ret_valid, ret_invalid);

    return VMX_SUCCESS;
}

#endif

#endif
