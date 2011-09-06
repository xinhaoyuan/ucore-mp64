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

#ifndef __VMM_TYPES_H
#define __VMM_TYPES_H

#ifdef __V3VEE__
/* #include <palacios/vmm_stddef.h> */




typedef enum {SHADOW_PAGING, NESTED_PAGING} v3_paging_mode_t;
typedef enum {VM_RUNNING, VM_STOPPED, VM_SUSPENDED, VM_ERROR, VM_EMULATING} v3_vm_operating_mode_t;


typedef enum {REAL, /*UNREAL,*/ PROTECTED, PROTECTED_PAE, LONG, LONG_32_COMPAT, LONG_16_COMPAT} v3_cpu_mode_t;
typedef enum {PHYSICAL_MEM, VIRTUAL_MEM} v3_mem_mode_t;


#define NULL ((void *)0)


typedef signed char schar_t;
typedef unsigned char uchar_t;

typedef signed short sshort_t;
typedef unsigned short ushort_t;

typedef signed int sint_t;
typedef unsigned int uint_t;

typedef signed long long sllong_t;
typedef unsigned long long ullong_t;

typedef signed long slong_t;
typedef unsigned long ulong_t;

typedef unsigned long size_t;
typedef long ssize_t;       

#define false 0
#define true 1
typedef uchar_t bool;



typedef unsigned long long uint64_t;
typedef long long sint64_t;

typedef unsigned int uint32_t;
typedef int sint32_t;


typedef unsigned short uint16_t;
typedef short sint16_t;

typedef unsigned char uint8_t;
typedef char sint8_t;

typedef ulong_t addr_t;
typedef ullong_t v3_reg_t;
#endif /* ! __V3VEE__ */

#endif
