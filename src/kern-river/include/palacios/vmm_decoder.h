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

#ifndef __VMM_DECODER_H
#define __VMM_DECODER_H

#ifdef __V3VEE__

#include <palacios/vm_guest.h>
#include <palacios/vmm.h>


typedef enum { V3_INVALID_OP,
	       V3_OP_MOVCR2, V3_OP_MOV2CR, V3_OP_SMSW, V3_OP_LMSW, V3_OP_CLTS,
	       V3_OP_INVLPG,
 	       V3_OP_ADC, V3_OP_ADD, V3_OP_AND, V3_OP_OR, V3_OP_XOR, V3_OP_SUB,
	       V3_OP_INC, V3_OP_DEC, V3_OP_NEG, V3_OP_MOV, V3_OP_NOT, V3_OP_XCHG, 
	       V3_OP_SETB, V3_OP_SETBE, V3_OP_SETL, V3_OP_SETLE, V3_OP_SETNB, 
	       V3_OP_SETNBE, V3_OP_SETNL, V3_OP_SETNLE, V3_OP_SETNO, V3_OP_SETNP,
	       V3_OP_SETNS, V3_OP_SETNZ, V3_OP_SETO, V3_OP_SETP, V3_OP_SETS, 
	       V3_OP_SETZ, V3_OP_MOVS, V3_OP_STOS, V3_OP_MOVZX, V3_OP_MOVSX} v3_op_type_t;


typedef enum {INVALID_OPERAND, REG_OPERAND, MEM_OPERAND, IMM_OPERAND} v3_operand_type_t;

struct x86_operand {
    addr_t operand;
    uint_t size;
    v3_operand_type_t type;
};

struct x86_prefixes {
    uint_t lock   : 1;  // 0xF0
    uint_t repne  : 1;  // 0xF2
    uint_t repnz  : 1;  // 0xF2
    uint_t rep    : 1;  // 0xF3
    uint_t repe   : 1;  // 0xF3
    uint_t repz   : 1;  // 0xF3
    uint_t cs_override : 1;  // 0x2E
    uint_t ss_override : 1;  // 0x36
    uint_t ds_override : 1;  // 0x3E
    uint_t es_override : 1;  // 0x26
    uint_t fs_override : 1;  // 0x64
    uint_t gs_override : 1;  // 0x65
    uint_t br_not_taken : 1;  // 0x2E
    uint_t br_taken   : 1;  // 0x3E
    uint_t op_size     : 1;  // 0x66
    uint_t addr_size   : 1;  // 0x67
};


struct x86_instr {
    struct x86_prefixes prefixes;
    uint_t instr_length;
    v3_op_type_t op_type;
    uint_t num_operands;
    struct x86_operand dst_operand;
    struct x86_operand src_operand;
    struct x86_operand third_operand;
    addr_t str_op_length;
    addr_t is_str_op;
    void * decoder_data;
};


struct basic_instr_info {
    uint_t instr_length;
    uint_t op_size;
    uint_t str_op    : 1;
    uint_t has_rep : 1;
};



/************************/
/* EXTERNAL DECODER API */
/************************/
/* 
   This is an External API definition that must be implemented by a decoder
*/


/* 
 * Initializes a decoder
 */
int v3_init_decoder();

/* 
 * Decodes an instruction 
 * All addresses in arguments are in the host address space
 * instr_ptr is the host address of the instruction 
 * IMPORTANT: make sure the instr_ptr is in contiguous host memory
 *   ie. Copy it to a buffer before the call
 */
int v3_decode(struct guest_info * info, addr_t instr_ptr, struct x86_instr * instr);

/* 
 * Encodes an instruction
 * All addresses in arguments are in the host address space
 * The instruction is encoded from the struct, and copied into a 15 byte host buffer
 * referenced by instr_buf
 * any unused bytes at the end of instr_buf will be filled with nops
 * IMPORTANT: instr_buf must be allocated and 15 bytes long
 */
int v3_encode(struct guest_info * info, struct x86_instr * instr, char * instr_buf);


/*
 * Gets the operand size for a memory operation
 *
 */
int v3_basic_mem_decode(struct guest_info * info, addr_t instr_ptr, struct basic_instr_info * instr_info);



/* Removes a rep prefix in place */
void v3_strip_rep_prefix(uchar_t * instr, int length);
void v3_get_prefixes(uchar_t * instr, struct x86_prefixes * prefixes);


/* 
 * JRL: Some of this was taken from the Xen sources... 
 */

#define PACKED __attribute__((packed))

#define MODRM_MOD(x) ((x >> 6) & 0x3)
#define MODRM_REG(x) ((x >> 3) & 0x7)
#define MODRM_RM(x)  (x & 0x7)

struct modrm_byte {
    uint_t rm   :   3 PACKED;
    uint_t reg  :   3 PACKED;
    uint_t mod  :   2 PACKED;
};


#define SIB_BASE(x) ((x >> 6) & 0x3)
#define SIB_INDEX(x) ((x >> 3) & 0x7)
#define SIB_SCALE(x) (x & 0x7)

struct sib_byte {
    uint_t base     :   3 PACKED;
    uint_t index    :   3 PACKED;
    uint_t scale    :   2 PACKED;
};



#define MAKE_INSTR(nm, ...) static  const uchar_t V3_OPCODE_##nm[] = { __VA_ARGS__ }

/* 
 * Here's how it works:
 * First byte: Length. 
 * Following bytes: Opcode bytes. 
 * Special case: Last byte, if zero, doesn't need to match. 
 */
MAKE_INSTR(INVD,   2, 0x0f, 0x08);
MAKE_INSTR(CPUID,  2, 0x0f, 0xa2);
MAKE_INSTR(RDMSR,  2, 0x0f, 0x32);
MAKE_INSTR(WRMSR,  2, 0x0f, 0x30);
MAKE_INSTR(RDTSC,  2, 0x0f, 0x31);
MAKE_INSTR(RDTSCP, 3, 0x0f, 0x01, 0xf9);
MAKE_INSTR(CLI,    1, 0xfa);
MAKE_INSTR(STI,    1, 0xfb);
MAKE_INSTR(RDPMC,  2, 0x0f, 0x33);
MAKE_INSTR(CLGI,   3, 0x0f, 0x01, 0xdd);
MAKE_INSTR(STGI,   3, 0x0f, 0x01, 0xdc);
MAKE_INSTR(VMRUN,  3, 0x0f, 0x01, 0xd8);
MAKE_INSTR(VMLOAD, 3, 0x0f, 0x01, 0xda);
MAKE_INSTR(VMSAVE, 3, 0x0f, 0x01, 0xdb);
MAKE_INSTR(VMCALL, 3, 0x0f, 0x01, 0xd9);
MAKE_INSTR(PAUSE,  2, 0xf3, 0x90);
MAKE_INSTR(SKINIT, 3, 0x0f, 0x01, 0xde);
MAKE_INSTR(MOV2CR, 3, 0x0f, 0x22, 0x00);
MAKE_INSTR(MOVCR2, 3, 0x0f, 0x20, 0x00);
MAKE_INSTR(MOV2DR, 3, 0x0f, 0x23, 0x00);
MAKE_INSTR(MOVDR2, 3, 0x0f, 0x21, 0x00);
MAKE_INSTR(PUSHF,  1, 0x9c);
MAKE_INSTR(POPF,   1, 0x9d);
MAKE_INSTR(RSM,    2, 0x0f, 0xaa);
MAKE_INSTR(INVLPG, 3, 0x0f, 0x01, 0x00);
MAKE_INSTR(INVLPGA,3, 0x0f, 0x01, 0xdf);
MAKE_INSTR(HLT,    1, 0xf4);
MAKE_INSTR(CLTS,   2, 0x0f, 0x06);
MAKE_INSTR(LMSW,   3, 0x0f, 0x01, 0x00);
MAKE_INSTR(SMSW,   3, 0x0f, 0x01, 0x00);


#define PREFIX_LOCK         0xF0
#define PREFIX_REPNE        0xF2
#define PREFIX_REPNZ        0xF2
#define PREFIX_REP          0xF3
#define PREFIX_REPE         0xF3
#define PREFIX_REPZ         0xF3
#define PREFIX_CS_OVERRIDE  0x2E
#define PREFIX_SS_OVERRIDE  0x36
#define PREFIX_DS_OVERRIDE  0x3E
#define PREFIX_ES_OVERRIDE  0x26
#define PREFIX_FS_OVERRIDE  0x64
#define PREFIX_GS_OVERRIDE  0x65
#define PREFIX_BR_NOT_TAKEN 0x2E
#define PREFIX_BR_TAKEN     0x3E
#define PREFIX_OP_SIZE      0x66
#define PREFIX_ADDR_SIZE    0x67

int v3_opcode_cmp(const uchar_t * op1, const uchar_t * op2);


static inline int is_prefix_byte(uchar_t byte) {
    switch (byte) {
	case 0xF0:      // lock
	case 0xF2:      // REPNE/REPNZ
	case 0xF3:      // REP or REPE/REPZ
	case 0x2E:      // CS override or Branch hint not taken (with Jcc instrs)
	case 0x36:      // SS override
	case 0x3E:      // DS override or Branch hint taken (with Jcc instrs)
	case 0x26:      // ES override
	case 0x64:      // FS override
	case 0x65:      // GS override
	    //case 0x2E:      // branch not taken hint
	    //  case 0x3E:      // branch taken hint
	case 0x66:      // operand size override
	case 0x67:      // address size override
	    return 1;
	    break;
	default:
	    return 0;
	    break;
    }
}


static inline v3_reg_t get_gpr_mask(struct guest_info * info) {
    switch (info->cpu_mode) {
	case REAL: 
	case LONG_16_COMPAT:
	    return 0xffff;
	    break;
	case PROTECTED:
	case LONG_32_COMPAT:
	case PROTECTED_PAE:
	    return 0xffffffff;
	case LONG:
	    return 0xffffffffffffffffLL;
	default:
	    PrintError("Unsupported Address Mode\n");
	    return -1;
    }
}


static inline addr_t get_addr_linear(struct guest_info * info, addr_t addr, struct v3_segment * seg) {
    switch (info->cpu_mode) {
	case REAL:
	    // It appears that the segment values are computed and cached in the vmcb structure 
	    // We Need to check this for Intel
	    /*   return addr + (seg->selector << 4);
		 break;*/

	case PROTECTED:
	case PROTECTED_PAE:
	case LONG_32_COMPAT:
	    return addr + seg->base;
	    break;

	case LONG:
	    // In long mode the segment bases are disregarded (forced to 0), unless using 
	    // FS or GS, then the base addresses are added
	    return addr + seg->base;

	case LONG_16_COMPAT:
	default:
	    PrintError("Unsupported CPU Mode: %d\n", info->cpu_mode);
	    return -1;
    }
}


typedef enum {INVALID_ADDR_TYPE, REG, DISP0, DISP8, DISP16, DISP32} modrm_mode_t;
typedef enum {INVALID_REG_SIZE, REG64, REG32, REG16, REG8} reg_size_t;






struct v3_gprs;

static inline addr_t decode_register(struct v3_gprs * gprs, char reg_code, reg_size_t reg_size) {
    addr_t reg_addr;

    switch (reg_code) {
	case 0:
	    reg_addr = (addr_t)&(gprs->rax);
	    break;
	case 1:
	    reg_addr = (addr_t)&(gprs->rcx);
	    break;
	case 2:
	    reg_addr = (addr_t)&(gprs->rdx);
	    break;
	case 3:
	    reg_addr = (addr_t)&(gprs->rbx);
	    break;
	case 4:
	    if (reg_size == REG8) {
		reg_addr = (addr_t)&(gprs->rax) + 1;
	    } else {
		reg_addr = (addr_t)&(gprs->rsp);
	    }
	    break;
	case 5:
	    if (reg_size == REG8) {
		reg_addr = (addr_t)&(gprs->rcx) + 1;
	    } else {
		reg_addr = (addr_t)&(gprs->rbp);
	    }
	    break;
	case 6:
	    if (reg_size == REG8) {
		reg_addr = (addr_t)&(gprs->rdx) + 1;
	    } else {
		reg_addr = (addr_t)&(gprs->rsi);
	    }
	    break;
	case 7:
	    if (reg_size == REG8) {
		reg_addr = (addr_t)&(gprs->rbx) + 1;
	    } else {
		reg_addr = (addr_t)&(gprs->rdi);
	    }
	    break;
	default:
	    reg_addr = 0;
	    break;
    }

    return reg_addr;
}



static inline v3_operand_type_t decode_operands16(struct v3_gprs * gprs, // input/output
						  char * modrm_instr,       // input
						  int * offset,             // output
						  addr_t * first_operand,   // output
						  addr_t * second_operand,  // output
						  reg_size_t reg_size) {    // input
  
    struct modrm_byte * modrm = (struct modrm_byte *)modrm_instr;
    addr_t base_addr = 0;
    modrm_mode_t mod_mode = 0;
    v3_operand_type_t addr_type = INVALID_OPERAND;
    char * instr_cursor = modrm_instr;

    //  PrintDebug("ModRM mod=%d\n", modrm->mod);

    instr_cursor += 1;

    if (modrm->mod == 3) {
	mod_mode = REG;
	addr_type = REG_OPERAND;
	//PrintDebug("first operand = Register (RM=%d)\n",modrm->rm);

	*first_operand = decode_register(gprs, modrm->rm, reg_size);

    } else {

	addr_type = MEM_OPERAND;

	if (modrm->mod == 0) {
	    mod_mode = DISP0;
	} else if (modrm->mod == 1) {
	    mod_mode = DISP8;
	} else if (modrm->mod == 2) {
	    mod_mode = DISP16;
	}

	switch (modrm->rm) {
	    case 0:
		base_addr = gprs->rbx + gprs->rsi;
		break;
	    case 1:
		base_addr = gprs->rbx + gprs->rdi;
		break;
	    case 2:
		base_addr = gprs->rbp + gprs->rsi;
		break;
	    case 3:
		base_addr = gprs->rbp + gprs->rdi;
		break;
	    case 4:
		base_addr = gprs->rsi;
		break;
	    case 5:
		base_addr = gprs->rdi;
		break;
	    case 6:
		if (modrm->mod == 0) {
		    base_addr = 0;
		    mod_mode = DISP16;
		} else {
		    base_addr = gprs->rbp;
		}
		break;
	    case 7:
		base_addr = gprs->rbx;
		break;
	}



	if (mod_mode == DISP8) {
	    base_addr += (uchar_t)*(instr_cursor);
	    instr_cursor += 1;
	} else if (mod_mode == DISP16) {
	    base_addr += (ushort_t)*(instr_cursor);
	    instr_cursor += 2;
	}
    
	*first_operand = base_addr;
    }

    *offset +=  (instr_cursor - modrm_instr);
    *second_operand = decode_register(gprs, modrm->reg, reg_size);

    return addr_type;
}



static inline v3_operand_type_t decode_operands32(struct v3_gprs * gprs, // input/output
						  uchar_t * modrm_instr,       // input
						  int * offset,             // output
						  addr_t * first_operand,   // output
						  addr_t * second_operand,  // output
						  reg_size_t reg_size) {    // input
  
    uchar_t * instr_cursor = modrm_instr;
    struct modrm_byte * modrm = (struct modrm_byte *)modrm_instr;
    addr_t base_addr = 0;
    modrm_mode_t mod_mode = 0;
    uint_t has_sib_byte = 0;
    v3_operand_type_t addr_type = INVALID_OPERAND;



    instr_cursor += 1;

    if (modrm->mod == 3) {
	mod_mode = REG;
	addr_type = REG_OPERAND;
    
	//    PrintDebug("first operand = Register (RM=%d)\n",modrm->rm);

	*first_operand = decode_register(gprs, modrm->rm, reg_size);

    } else {

	addr_type = MEM_OPERAND;

	if (modrm->mod == 0) {
	    mod_mode = DISP0;
	} else if (modrm->mod == 1) {
	    mod_mode = DISP8;
	} else if (modrm->mod == 2) {
	    mod_mode = DISP32;
	}
    
	switch (modrm->rm) {
	    case 0:
		base_addr = gprs->rax;
		break;
	    case 1:
		base_addr = gprs->rcx;
		break;
	    case 2:
		base_addr = gprs->rdx;
		break;
	    case 3:
		base_addr = gprs->rbx;
		break;
	    case 4:
		has_sib_byte = 1;
		break;
	    case 5:
		if (modrm->mod == 0) {
		    base_addr = 0;
		    mod_mode = DISP32;
		} else {
		    base_addr = gprs->rbp;
		}
		break;
	    case 6:
		base_addr = gprs->rsi;
		break;
	    case 7:
		base_addr = gprs->rdi;
		break;
	}

	if (has_sib_byte) {
	    instr_cursor += 1;
	    struct sib_byte * sib = (struct sib_byte *)(instr_cursor);
	    int scale = 1;

	    instr_cursor += 1;


	    if (sib->scale == 1) {
		scale = 2;
	    } else if (sib->scale == 2) {
		scale = 4;
	    } else if (sib->scale == 3) {
		scale = 8;
	    }


	    switch (sib->index) {
		case 0:
		    base_addr = gprs->rax;
		    break;
		case 1:
		    base_addr = gprs->rcx;
		    break;
		case 2:
		    base_addr = gprs->rdx;
		    break;
		case 3:
		    base_addr = gprs->rbx;
		    break;
		case 4:
		    base_addr = 0;
		    break;
		case 5:
		    base_addr = gprs->rbp;
		    break;
		case 6:
		    base_addr = gprs->rsi;
		    break;
		case 7:
		    base_addr = gprs->rdi;
		    break;
	    }

	    base_addr *= scale;


	    switch (sib->base) {
		case 0:
		    base_addr += gprs->rax;
		    break;
		case 1:
		    base_addr += gprs->rcx;
		    break;
		case 2:
		    base_addr += gprs->rdx;
		    break;
		case 3:
		    base_addr += gprs->rbx;
		    break;
		case 4:
		    base_addr += gprs->rsp;
		    break;
		case 5:
		    if (modrm->mod != 0) {
			base_addr += gprs->rbp;
		    }
		    break;
		case 6:
		    base_addr += gprs->rsi;
		    break;
		case 7:
		    base_addr += gprs->rdi;
		    break;
	    }

	} 


	if (mod_mode == DISP8) {
	    base_addr += (uchar_t)*(instr_cursor);
	    instr_cursor += 1;
	} else if (mod_mode == DISP32) {
	    base_addr += (uint_t)*(instr_cursor);
	    instr_cursor += 4;
	}
    

	*first_operand = base_addr;
    }

    *offset += (instr_cursor - modrm_instr);

    *second_operand = decode_register(gprs, modrm->reg, reg_size);

    return addr_type;
}



#endif // !__V3VEE__


#endif
