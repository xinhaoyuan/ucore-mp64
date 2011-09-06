/*
 * This file is part of the Palacios Virtual Machine Monitor developed
 * by the V3VEE Project with funding from the United States National 
 * Science Foundation and the Department of Energy.  
 *
 * The V3VEE Project is a joint project between Northwestern University
 * and the University of New Mexico.  You can find out more at 
 * http://www.v3vee.org
 *
 * Copyright (c) 2008, The V3VEE Project <http://www.v3vee.org> 
 * All rights reserved.
 *
 * Author: Automatically Generated File
 *
 * This is free software.  You are permitted to use,
 * redistribute, and modify it as specified in the file "V3VEE_LICENSE".
 */

#ifndef __VMCS_GEN__
#define __VMCS_GEN__

#ifdef __V3VEE__


#include <palacios/vmcs.h>
#include <palacios/vmm.h>

void    Set_VMCS_GUEST_ES_SELECTOR(uint_t val);
uint_t  Get_VMCS_GUEST_ES_SELECTOR();

void    PrintTrace_VMCS_GUEST_ES_SELECTOR();


void    Set_VMCS_GUEST_CS_SELECTOR(uint_t val);
uint_t  Get_VMCS_GUEST_CS_SELECTOR();

void    PrintTrace_VMCS_GUEST_CS_SELECTOR();


void    Set_VMCS_GUEST_SS_SELECTOR(uint_t val);
uint_t  Get_VMCS_GUEST_SS_SELECTOR();

void    PrintTrace_VMCS_GUEST_SS_SELECTOR();


void    Set_VMCS_GUEST_DS_SELECTOR(uint_t val);
uint_t  Get_VMCS_GUEST_DS_SELECTOR();

void    PrintTrace_VMCS_GUEST_DS_SELECTOR();


void    Set_VMCS_GUEST_FS_SELECTOR(uint_t val);
uint_t  Get_VMCS_GUEST_FS_SELECTOR();

void    PrintTrace_VMCS_GUEST_FS_SELECTOR();


void    Set_VMCS_GUEST_GS_SELECTOR(uint_t val);
uint_t  Get_VMCS_GUEST_GS_SELECTOR();

void    PrintTrace_VMCS_GUEST_GS_SELECTOR();


void    Set_VMCS_GUEST_LDTR_SELECTOR(uint_t val);
uint_t  Get_VMCS_GUEST_LDTR_SELECTOR();

void    PrintTrace_VMCS_GUEST_LDTR_SELECTOR();


void    Set_VMCS_GUEST_TR_SELECTOR(uint_t val);
uint_t  Get_VMCS_GUEST_TR_SELECTOR();

void    PrintTrace_VMCS_GUEST_TR_SELECTOR();


void    Set_VMCS_HOST_ES_SELECTOR(uint_t val);
uint_t  Get_VMCS_HOST_ES_SELECTOR();

void    PrintTrace_VMCS_HOST_ES_SELECTOR();


void    Set_VMCS_HOST_CS_SELECTOR(uint_t val);
uint_t  Get_VMCS_HOST_CS_SELECTOR();

void    PrintTrace_VMCS_HOST_CS_SELECTOR();


void    Set_VMCS_HOST_SS_SELECTOR(uint_t val);
uint_t  Get_VMCS_HOST_SS_SELECTOR();

void    PrintTrace_VMCS_HOST_SS_SELECTOR();


void    Set_VMCS_HOST_DS_SELECTOR(uint_t val);
uint_t  Get_VMCS_HOST_DS_SELECTOR();

void    PrintTrace_VMCS_HOST_DS_SELECTOR();


void    Set_VMCS_HOST_FS_SELECTOR(uint_t val);
uint_t  Get_VMCS_HOST_FS_SELECTOR();

void    PrintTrace_VMCS_HOST_FS_SELECTOR();


void    Set_VMCS_HOST_GS_SELECTOR(uint_t val);
uint_t  Get_VMCS_HOST_GS_SELECTOR();

void    PrintTrace_VMCS_HOST_GS_SELECTOR();


void    Set_VMCS_HOST_TR_SELECTOR(uint_t val);
uint_t  Get_VMCS_HOST_TR_SELECTOR();

void    PrintTrace_VMCS_HOST_TR_SELECTOR();


void    Set_IO_BITMAP_A_ADDR(uint_t val);
uint_t  Get_IO_BITMAP_A_ADDR();

void    PrintTrace_IO_BITMAP_A_ADDR();


void    Set_IO_BITMAP_A_ADDR_HIGH(uint_t val);
uint_t  Get_IO_BITMAP_A_ADDR_HIGH();

void    PrintTrace_IO_BITMAP_A_ADDR_HIGH();


void    Set_IO_BITMAP_B_ADDR(uint_t val);
uint_t  Get_IO_BITMAP_B_ADDR();

void    PrintTrace_IO_BITMAP_B_ADDR();


void    Set_IO_BITMAP_B_ADDR_HIGH(uint_t val);
uint_t  Get_IO_BITMAP_B_ADDR_HIGH();

void    PrintTrace_IO_BITMAP_B_ADDR_HIGH();


void    Set_MSR_BITMAPS(uint_t val);
uint_t  Get_MSR_BITMAPS();

void    PrintTrace_MSR_BITMAPS();


void    Set_MSR_BITMAPS_HIGH(uint_t val);
uint_t  Get_MSR_BITMAPS_HIGH();

void    PrintTrace_MSR_BITMAPS_HIGH();


void    Set_VM_EXIT_MSR_STORE_ADDR(uint_t val);
uint_t  Get_VM_EXIT_MSR_STORE_ADDR();

void    PrintTrace_VM_EXIT_MSR_STORE_ADDR();


void    Set_VM_EXIT_MSR_STORE_ADDR_HIGH(uint_t val);
uint_t  Get_VM_EXIT_MSR_STORE_ADDR_HIGH();

void    PrintTrace_VM_EXIT_MSR_STORE_ADDR_HIGH();


void    Set_VM_EXIT_MSR_LOAD_ADDR(uint_t val);
uint_t  Get_VM_EXIT_MSR_LOAD_ADDR();

void    PrintTrace_VM_EXIT_MSR_LOAD_ADDR();


void    Set_VM_EXIT_MSR_LOAD_ADDR_HIGH(uint_t val);
uint_t  Get_VM_EXIT_MSR_LOAD_ADDR_HIGH();

void    PrintTrace_VM_EXIT_MSR_LOAD_ADDR_HIGH();


void    Set_VM_ENTRY_MSR_LOAD_ADDR(uint_t val);
uint_t  Get_VM_ENTRY_MSR_LOAD_ADDR();

void    PrintTrace_VM_ENTRY_MSR_LOAD_ADDR();


void    Set_VM_ENTRY_MSR_LOAD_ADDR_HIGH(uint_t val);
uint_t  Get_VM_ENTRY_MSR_LOAD_ADDR_HIGH();

void    PrintTrace_VM_ENTRY_MSR_LOAD_ADDR_HIGH();


void    Set_VMCS_EXEC_PTR(uint_t val);
uint_t  Get_VMCS_EXEC_PTR();

void    PrintTrace_VMCS_EXEC_PTR();


void    Set_VMCS_EXEC_PTR_HIGH(uint_t val);
uint_t  Get_VMCS_EXEC_PTR_HIGH();

void    PrintTrace_VMCS_EXEC_PTR_HIGH();


void    Set_TSC_OFFSET(uint_t val);
uint_t  Get_TSC_OFFSET();

void    PrintTrace_TSC_OFFSET();


void    Set_TSC_OFFSET_HIGH(uint_t val);
uint_t  Get_TSC_OFFSET_HIGH();

void    PrintTrace_TSC_OFFSET_HIGH();


void    Set_VIRT_APIC_PAGE_ADDR(uint_t val);
uint_t  Get_VIRT_APIC_PAGE_ADDR();

void    PrintTrace_VIRT_APIC_PAGE_ADDR();


void    Set_VIRT_APIC_PAGE_ADDR_HIGH(uint_t val);
uint_t  Get_VIRT_APIC_PAGE_ADDR_HIGH();

void    PrintTrace_VIRT_APIC_PAGE_ADDR_HIGH();


void    Set_VMCS_LINK_PTR(uint_t val);
uint_t  Get_VMCS_LINK_PTR();

void    PrintTrace_VMCS_LINK_PTR();


void    Set_VMCS_LINK_PTR_HIGH(uint_t val);
uint_t  Get_VMCS_LINK_PTR_HIGH();

void    PrintTrace_VMCS_LINK_PTR_HIGH();


void    Set_GUEST_IA32_DEBUGCTL(uint_t val);
uint_t  Get_GUEST_IA32_DEBUGCTL();

void    PrintTrace_GUEST_IA32_DEBUGCTL();


void    Set_GUEST_IA32_DEBUGCTL_HIGH(uint_t val);
uint_t  Get_GUEST_IA32_DEBUGCTL_HIGH();

void    PrintTrace_GUEST_IA32_DEBUGCTL_HIGH();


void    Set_PIN_VM_EXEC_CTRLS(uint_t val);
uint_t  Get_PIN_VM_EXEC_CTRLS();

void    PrintTrace_PIN_VM_EXEC_CTRLS();


void    Set_PROC_VM_EXEC_CTRLS(uint_t val);
uint_t  Get_PROC_VM_EXEC_CTRLS();

void    PrintTrace_PROC_VM_EXEC_CTRLS();


void    Set_EXCEPTION_BITMAP(uint_t val);
uint_t  Get_EXCEPTION_BITMAP();

void    PrintTrace_EXCEPTION_BITMAP();


void    Set_PAGE_FAULT_ERROR_MASK(uint_t val);
uint_t  Get_PAGE_FAULT_ERROR_MASK();

void    PrintTrace_PAGE_FAULT_ERROR_MASK();


void    Set_PAGE_FAULT_ERROR_MATCH(uint_t val);
uint_t  Get_PAGE_FAULT_ERROR_MATCH();

void    PrintTrace_PAGE_FAULT_ERROR_MATCH();


void    Set_CR3_TARGET_COUNT(uint_t val);
uint_t  Get_CR3_TARGET_COUNT();

void    PrintTrace_CR3_TARGET_COUNT();


void    Set_VM_EXIT_CTRLS(uint_t val);
uint_t  Get_VM_EXIT_CTRLS();

void    PrintTrace_VM_EXIT_CTRLS();


void    Set_VM_EXIT_MSR_STORE_COUNT(uint_t val);
uint_t  Get_VM_EXIT_MSR_STORE_COUNT();

void    PrintTrace_VM_EXIT_MSR_STORE_COUNT();


void    Set_VM_EXIT_MSR_LOAD_COUNT(uint_t val);
uint_t  Get_VM_EXIT_MSR_LOAD_COUNT();

void    PrintTrace_VM_EXIT_MSR_LOAD_COUNT();


void    Set_VM_ENTRY_CTRLS(uint_t val);
uint_t  Get_VM_ENTRY_CTRLS();

void    PrintTrace_VM_ENTRY_CTRLS();


void    Set_VM_ENTRY_MSR_LOAD_COUNT(uint_t val);
uint_t  Get_VM_ENTRY_MSR_LOAD_COUNT();

void    PrintTrace_VM_ENTRY_MSR_LOAD_COUNT();


void    Set_VM_ENTRY_INT_INFO_FIELD(uint_t val);
uint_t  Get_VM_ENTRY_INT_INFO_FIELD();

void    PrintTrace_VM_ENTRY_INT_INFO_FIELD();


void    Set_VM_ENTRY_EXCEPTION_ERROR(uint_t val);
uint_t  Get_VM_ENTRY_EXCEPTION_ERROR();

void    PrintTrace_VM_ENTRY_EXCEPTION_ERROR();


void    Set_VM_ENTRY_INSTR_LENGTH(uint_t val);
uint_t  Get_VM_ENTRY_INSTR_LENGTH();

void    PrintTrace_VM_ENTRY_INSTR_LENGTH();


void    Set_TPR_THRESHOLD(uint_t val);
uint_t  Get_TPR_THRESHOLD();

void    PrintTrace_TPR_THRESHOLD();


void    Set_VM_INSTR_ERROR(uint_t val);
uint_t  Get_VM_INSTR_ERROR();

void    PrintTrace_VM_INSTR_ERROR();


void    Set_EXIT_REASON(uint_t val);
uint_t  Get_EXIT_REASON();

void    PrintTrace_EXIT_REASON();


void    Set_VM_EXIT_INT_INFO(uint_t val);
uint_t  Get_VM_EXIT_INT_INFO();

void    PrintTrace_VM_EXIT_INT_INFO();


void    Set_VM_EXIT_INT_ERROR(uint_t val);
uint_t  Get_VM_EXIT_INT_ERROR();

void    PrintTrace_VM_EXIT_INT_ERROR();


void    Set_IDT_VECTOR_INFO(uint_t val);
uint_t  Get_IDT_VECTOR_INFO();

void    PrintTrace_IDT_VECTOR_INFO();


void    Set_IDT_VECTOR_ERROR(uint_t val);
uint_t  Get_IDT_VECTOR_ERROR();

void    PrintTrace_IDT_VECTOR_ERROR();


void    Set_VM_EXIT_INSTR_LENGTH(uint_t val);
uint_t  Get_VM_EXIT_INSTR_LENGTH();

void    PrintTrace_VM_EXIT_INSTR_LENGTH();


void    Set_VMX_INSTR_INFO(uint_t val);
uint_t  Get_VMX_INSTR_INFO();

void    PrintTrace_VMX_INSTR_INFO();


void    Set_GUEST_ES_LIMIT(uint_t val);
uint_t  Get_GUEST_ES_LIMIT();

void    PrintTrace_GUEST_ES_LIMIT();


void    Set_GUEST_CS_LIMIT(uint_t val);
uint_t  Get_GUEST_CS_LIMIT();

void    PrintTrace_GUEST_CS_LIMIT();


void    Set_GUEST_SS_LIMIT(uint_t val);
uint_t  Get_GUEST_SS_LIMIT();

void    PrintTrace_GUEST_SS_LIMIT();


void    Set_GUEST_DS_LIMIT(uint_t val);
uint_t  Get_GUEST_DS_LIMIT();

void    PrintTrace_GUEST_DS_LIMIT();


void    Set_GUEST_FS_LIMIT(uint_t val);
uint_t  Get_GUEST_FS_LIMIT();

void    PrintTrace_GUEST_FS_LIMIT();


void    Set_GUEST_GS_LIMIT(uint_t val);
uint_t  Get_GUEST_GS_LIMIT();

void    PrintTrace_GUEST_GS_LIMIT();


void    Set_GUEST_LDTR_LIMIT(uint_t val);
uint_t  Get_GUEST_LDTR_LIMIT();

void    PrintTrace_GUEST_LDTR_LIMIT();


void    Set_GUEST_TR_LIMIT(uint_t val);
uint_t  Get_GUEST_TR_LIMIT();

void    PrintTrace_GUEST_TR_LIMIT();


void    Set_GUEST_GDTR_LIMIT(uint_t val);
uint_t  Get_GUEST_GDTR_LIMIT();

void    PrintTrace_GUEST_GDTR_LIMIT();


void    Set_GUEST_IDTR_LIMIT(uint_t val);
uint_t  Get_GUEST_IDTR_LIMIT();

void    PrintTrace_GUEST_IDTR_LIMIT();


void    Set_GUEST_ES_ACCESS(uint_t val);
uint_t  Get_GUEST_ES_ACCESS();

void    PrintTrace_GUEST_ES_ACCESS();


void    Set_GUEST_CS_ACCESS(uint_t val);
uint_t  Get_GUEST_CS_ACCESS();

void    PrintTrace_GUEST_CS_ACCESS();


void    Set_GUEST_SS_ACCESS(uint_t val);
uint_t  Get_GUEST_SS_ACCESS();

void    PrintTrace_GUEST_SS_ACCESS();


void    Set_GUEST_DS_ACCESS(uint_t val);
uint_t  Get_GUEST_DS_ACCESS();

void    PrintTrace_GUEST_DS_ACCESS();


void    Set_GUEST_FS_ACCESS(uint_t val);
uint_t  Get_GUEST_FS_ACCESS();

void    PrintTrace_GUEST_FS_ACCESS();


void    Set_GUEST_GS_ACCESS(uint_t val);
uint_t  Get_GUEST_GS_ACCESS();

void    PrintTrace_GUEST_GS_ACCESS();


void    Set_GUEST_LDTR_ACCESS(uint_t val);
uint_t  Get_GUEST_LDTR_ACCESS();

void    PrintTrace_GUEST_LDTR_ACCESS();


void    Set_GUEST_TR_ACCESS(uint_t val);
uint_t  Get_GUEST_TR_ACCESS();

void    PrintTrace_GUEST_TR_ACCESS();


void    Set_GUEST_INT_STATE(uint_t val);
uint_t  Get_GUEST_INT_STATE();

void    PrintTrace_GUEST_INT_STATE();


void    Set_GUEST_ACTIVITY_STATE(uint_t val);
uint_t  Get_GUEST_ACTIVITY_STATE();

void    PrintTrace_GUEST_ACTIVITY_STATE();


void    Set_GUEST_SMBASE(uint_t val);
uint_t  Get_GUEST_SMBASE();

void    PrintTrace_GUEST_SMBASE();


void    Set_GUEST_IA32_SYSENTER_CS(uint_t val);
uint_t  Get_GUEST_IA32_SYSENTER_CS();

void    PrintTrace_GUEST_IA32_SYSENTER_CS();


void    Set_HOST_IA32_SYSENTER_CS(uint_t val);
uint_t  Get_HOST_IA32_SYSENTER_CS();

void    PrintTrace_HOST_IA32_SYSENTER_CS();


void    Set_CR0_GUEST_HOST_MASK(uint_t val);
uint_t  Get_CR0_GUEST_HOST_MASK();

void    PrintTrace_CR0_GUEST_HOST_MASK();


void    Set_CR4_GUEST_HOST_MASK(uint_t val);
uint_t  Get_CR4_GUEST_HOST_MASK();

void    PrintTrace_CR4_GUEST_HOST_MASK();


void    Set_CR0_READ_SHADOW(uint_t val);
uint_t  Get_CR0_READ_SHADOW();

void    PrintTrace_CR0_READ_SHADOW();


void    Set_CR4_READ_SHADOW(uint_t val);
uint_t  Get_CR4_READ_SHADOW();

void    PrintTrace_CR4_READ_SHADOW();


void    Set_CR3_TARGET_VALUE_0(uint_t val);
uint_t  Get_CR3_TARGET_VALUE_0();

void    PrintTrace_CR3_TARGET_VALUE_0();


void    Set_CR3_TARGET_VALUE_1(uint_t val);
uint_t  Get_CR3_TARGET_VALUE_1();

void    PrintTrace_CR3_TARGET_VALUE_1();


void    Set_CR3_TARGET_VALUE_2(uint_t val);
uint_t  Get_CR3_TARGET_VALUE_2();

void    PrintTrace_CR3_TARGET_VALUE_2();


void    Set_CR3_TARGET_VALUE_3(uint_t val);
uint_t  Get_CR3_TARGET_VALUE_3();

void    PrintTrace_CR3_TARGET_VALUE_3();


void    Set_EXIT_QUALIFICATION(uint_t val);
uint_t  Get_EXIT_QUALIFICATION();

void    PrintTrace_EXIT_QUALIFICATION();


void    Set_IO_RCX(uint_t val);
uint_t  Get_IO_RCX();

void    PrintTrace_IO_RCX();


void    Set_IO_RSI(uint_t val);
uint_t  Get_IO_RSI();

void    PrintTrace_IO_RSI();


void    Set_IO_RDI(uint_t val);
uint_t  Get_IO_RDI();

void    PrintTrace_IO_RDI();


void    Set_IO_RIP(uint_t val);
uint_t  Get_IO_RIP();

void    PrintTrace_IO_RIP();


void    Set_GUEST_LINEAR_ADDR(uint_t val);
uint_t  Get_GUEST_LINEAR_ADDR();

void    PrintTrace_GUEST_LINEAR_ADDR();


void    Set_GUEST_CR0(uint_t val);
uint_t  Get_GUEST_CR0();

void    PrintTrace_GUEST_CR0();


void    Set_GUEST_CR3(uint_t val);
uint_t  Get_GUEST_CR3();

void    PrintTrace_GUEST_CR3();


void    Set_GUEST_CR4(uint_t val);
uint_t  Get_GUEST_CR4();

void    PrintTrace_GUEST_CR4();


void    Set_GUEST_ES_BASE(uint_t val);
uint_t  Get_GUEST_ES_BASE();

void    PrintTrace_GUEST_ES_BASE();


void    Set_GUEST_CS_BASE(uint_t val);
uint_t  Get_GUEST_CS_BASE();

void    PrintTrace_GUEST_CS_BASE();


void    Set_GUEST_SS_BASE(uint_t val);
uint_t  Get_GUEST_SS_BASE();

void    PrintTrace_GUEST_SS_BASE();


void    Set_GUEST_DS_BASE(uint_t val);
uint_t  Get_GUEST_DS_BASE();

void    PrintTrace_GUEST_DS_BASE();


void    Set_GUEST_FS_BASE(uint_t val);
uint_t  Get_GUEST_FS_BASE();

void    PrintTrace_GUEST_FS_BASE();


void    Set_GUEST_GS_BASE(uint_t val);
uint_t  Get_GUEST_GS_BASE();

void    PrintTrace_GUEST_GS_BASE();


void    Set_GUEST_LDTR_BASE(uint_t val);
uint_t  Get_GUEST_LDTR_BASE();

void    PrintTrace_GUEST_LDTR_BASE();


void    Set_GUEST_TR_BASE(uint_t val);
uint_t  Get_GUEST_TR_BASE();

void    PrintTrace_GUEST_TR_BASE();


void    Set_GUEST_GDTR_BASE(uint_t val);
uint_t  Get_GUEST_GDTR_BASE();

void    PrintTrace_GUEST_GDTR_BASE();


void    Set_GUEST_IDTR_BASE(uint_t val);
uint_t  Get_GUEST_IDTR_BASE();

void    PrintTrace_GUEST_IDTR_BASE();


void    Set_GUEST_DR7(uint_t val);
uint_t  Get_GUEST_DR7();

void    PrintTrace_GUEST_DR7();


void    Set_GUEST_RSP(uint_t val);
uint_t  Get_GUEST_RSP();

void    PrintTrace_GUEST_RSP();


void    Set_GUEST_RIP(uint_t val);
uint_t  Get_GUEST_RIP();

void    PrintTrace_GUEST_RIP();


void    Set_GUEST_RFLAGS(uint_t val);
uint_t  Get_GUEST_RFLAGS();

void    PrintTrace_GUEST_RFLAGS();


void    Set_GUEST_PENDING_DEBUG_EXCS(uint_t val);
uint_t  Get_GUEST_PENDING_DEBUG_EXCS();

void    PrintTrace_GUEST_PENDING_DEBUG_EXCS();


void    Set_GUEST_IA32_SYSENTER_ESP(uint_t val);
uint_t  Get_GUEST_IA32_SYSENTER_ESP();

void    PrintTrace_GUEST_IA32_SYSENTER_ESP();


void    Set_GUEST_IA32_SYSENTER_EIP(uint_t val);
uint_t  Get_GUEST_IA32_SYSENTER_EIP();

void    PrintTrace_GUEST_IA32_SYSENTER_EIP();


void    Set_HOST_CR0(uint_t val);
uint_t  Get_HOST_CR0();

void    PrintTrace_HOST_CR0();


void    Set_HOST_CR3(uint_t val);
uint_t  Get_HOST_CR3();

void    PrintTrace_HOST_CR3();


void    Set_HOST_CR4(uint_t val);
uint_t  Get_HOST_CR4();

void    PrintTrace_HOST_CR4();


void    Set_HOST_FS_BASE(uint_t val);
uint_t  Get_HOST_FS_BASE();

void    PrintTrace_HOST_FS_BASE();


void    Set_HOST_GS_BASE(uint_t val);
uint_t  Get_HOST_GS_BASE();

void    PrintTrace_HOST_GS_BASE();


void    Set_HOST_TR_BASE(uint_t val);
uint_t  Get_HOST_TR_BASE();

void    PrintTrace_HOST_TR_BASE();


void    Set_HOST_GDTR_BASE(uint_t val);
uint_t  Get_HOST_GDTR_BASE();

void    PrintTrace_HOST_GDTR_BASE();


void    Set_HOST_IDTR_BASE(uint_t val);
uint_t  Get_HOST_IDTR_BASE();

void    PrintTrace_HOST_IDTR_BASE();


void    Set_HOST_IA32_SYSENTER_ESP(uint_t val);
uint_t  Get_HOST_IA32_SYSENTER_ESP();

void    PrintTrace_HOST_IA32_SYSENTER_ESP();


void    Set_HOST_IA32_SYSENTER_EIP(uint_t val);
uint_t  Get_HOST_IA32_SYSENTER_EIP();

void    PrintTrace_HOST_IA32_SYSENTER_EIP();


void    Set_HOST_RSP(uint_t val);
uint_t  Get_HOST_RSP();

void    PrintTrace_HOST_RSP();


void    Set_HOST_RIP(uint_t val);
uint_t  Get_HOST_RIP();

void    PrintTrace_HOST_RIP();

void PrintTrace_VMCS_ALL();


#endif // !__V3VEE

#endif



