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



#define MAKE_1OP_8FLAGS_INST(iname) static inline void iname##8(addr_t * dst,  addr_t * flags) { \
	/* Some of the flags values are not copied out in a pushf, we save them here */ \
	addr_t flags_rsvd = *flags & ~0xfffe7fff;			\
									\
	asm volatile (							\
		      "pushf; "						\
		      "push %2; "					\
		      "popf; "						\
		      #iname"b %0; "					\
		      "pushf; "						\
		      "pop %1; "					\
		      "popf; "						\
		      : "=q"(*(uint8_t *)dst),"=q"(*flags)		\
		      : "q"(*flags), "0"(*(uint8_t *)dst)		\
		      );						\
	*flags |= flags_rsvd;						\
									\
    }

#define MAKE_1OP_16FLAGS_INST(iname) static inline void iname##16(addr_t * dst,  addr_t * flags) { \
	/* Some of the flags values are not copied out in a pushf, we save them here */ \
	addr_t flags_rsvd = *flags & ~0xfffe7fff;			\
									\
	asm volatile (							\
		      "pushf; "						\
		      "push %2; "					\
		      "popf; "						\
		      #iname"w %0; "					\
		      "pushf; "						\
		      "pop %1; "					\
		      "popf; "						\
		      : "=q"(*(uint16_t *)dst),"=q"(*flags)		\
		      : "q"(*flags), "0"(*(uint16_t *)dst)		\
		      );						\
	*flags |= flags_rsvd;						\
									\
    }

#define MAKE_1OP_32FLAGS_INST(iname) static inline void iname##32(addr_t * dst,  addr_t * flags) { \
	/* Some of the flags values are not copied out in a pushf, we save them here */ \
	addr_t flags_rsvd = *flags & ~0xfffe7fff;			\
									\
	asm volatile (							\
		      "pushf; "						\
		      "push %2; "					\
		      "popf; "						\
		      #iname"l %0; "					\
		      "pushf; "						\
		      "pop %1; "					\
		      "popf; "						\
		      : "=q"(*(uint32_t *)dst),"=q"(*flags)		\
		      : "q"(*flags), "0"(*(uint32_t *)dst)		\
		      );						\
	*flags |= flags_rsvd;						\
									\
    }

#define MAKE_1OP_64FLAGS_INST(iname) static inline void iname##64(addr_t * dst,  addr_t * flags) { \
	/* Some of the flags values are not copied out in a pushf, we save them here */ \
	addr_t flags_rsvd = *flags & ~0xfffe7fff;			\
									\
	asm volatile (							\
		      "pushfq; "					\
		      "push %2; "					\
		      "popfq; "						\
		      #iname"q %0; "					\
		      "pushfq; "					\
		      "pop %1; "					\
		      "popfq; "						\
		      : "=q"(*(uint64_t *)dst),"=q"(*flags)		\
		      : "q"(*flags), "0"(*(uint64_t *)dst)		\
		      );						\
	*flags |= flags_rsvd;						\
									\
    }



#define MAKE_1OP_8_INST(iname) static inline void iname##8(addr_t * dst) { \
	asm volatile (							\
		      #iname"b %0; "					\
		      : "=q"(*(uint8_t *)dst)				\
		      : "0"(*(uint8_t *)dst)				\
		      );						\
    }

#define MAKE_1OP_16_INST(iname) static inline void iname##16(addr_t * dst) { \
	asm volatile (							\
		      #iname"w %0; "					\
		      : "=q"(*(uint16_t *)dst)				\
		      :  "0"(*(uint16_t *)dst)				\
		      );						\
    }

#define MAKE_1OP_32_INST(iname) static inline void iname##32(addr_t * dst) { \
	asm volatile (							\
		      #iname"l %0; "					\
		      : "=q"(*(uint32_t *)dst)				\
		      : "0"(*(uint32_t *)dst)				\
		      );						\
    }

#define MAKE_1OP_64_INST(iname) static inline void iname##64(addr_t * dst) { \
	asm volatile (							\
		      #iname"q %0; "					\
		      : "=q"(*(uint64_t *)dst)				\
		      : "0"(*(uint64_t *)dst)				\
		      );						\
    }


#define MAKE_2OP_64FLAGS_INST(iname) static inline void iname##64(addr_t * dst, addr_t * src, addr_t * flags) { \
	/* Some of the flags values are not copied out in a pushf, we save them here */ \
	addr_t flags_rsvd = *flags & ~0xfffe7fff;			\
									\
	asm volatile (							\
		      "pushfq\r\n"					\
		      "push %3\r\n"					\
		      "popfq\r\n"					\
		      #iname"q %2, %0\r\n"				\
		      "pushfq\r\n"					\
		      "pop %1\r\n"					\
		      "popfq\r\n"					\
		      : "=q"(*(uint64_t *)dst),"=q"(*flags)		\
		      : "q"(*(uint64_t *)src),"q"(*flags), "0"(*(uint64_t *)dst) \
		      );						\
	*flags |= flags_rsvd;						\
									\
    }




#define MAKE_2OP_32FLAGS_INST(iname) static inline void iname##32(addr_t * dst, addr_t * src, addr_t * flags) { \
	/* Some of the flags values are not copied out in a pushf, we save them here */ \
	addr_t flags_rsvd = *flags & ~0xfffe7fff;			\
									\
	asm volatile (							\
		      "pushf; "						\
		      "push %3; "					\
		      "popf; "						\
		      #iname"l %2, %0; "				\
		      "pushf; "						\
		      "pop %1; "					\
		      "popf; "						\
		      : "=q"(*(uint32_t *)dst),"=q"(*flags)		\
		      : "q"(*(uint32_t *)src),"q"(*flags), "0"(*(uint32_t *)dst) \
		      );						\
	*flags |= flags_rsvd;						\
    }


#define MAKE_2OP_16FLAGS_INST(iname) static inline void iname##16(addr_t * dst, addr_t * src, addr_t * flags) { \
	/* Some of the flags values are not copied out in a pushf, we save them here */ \
	addr_t flags_rsvd = *flags & ~0xfffe7fff;			\
									\
	asm volatile (							\
		      "pushf; "						\
		      "push %3; "					\
		      "popf; "						\
		      #iname"w %2, %0; "				\
		      "pushf; "						\
		      "pop %1; "					\
		      "popf; "						\
		      : "=q"(*(uint16_t *)dst),"=q"(*flags)		\
		      : "q"(*(uint16_t *)src),"q"(*flags), "0"(*(uint16_t *)dst) \
		      );						\
	*flags |= flags_rsvd;						\
    }

#define MAKE_2OP_8FLAGS_INST(iname) static inline void iname##8(addr_t * dst, addr_t * src, addr_t * flags) { \
	/* Some of the flags values are not copied out in a pushf, we save them here */ \
	addr_t flags_rsvd = *flags & ~0xfffe7fff;			\
									\
	asm volatile (							\
		      "pushf; "						\
		      "push %3; "					\
		      "popf; "						\
		      #iname"b %2, %0; "				\
		      "pushf; "						\
		      "pop %1; "					\
		      "popf; "						\
		      : "=q"(*(uint8_t *)dst),"=q"(*flags)		\
		      : "q"(*(uint8_t *)src),"q"(*flags), "0"(*(uint8_t *)dst) \
		      );						\
	*flags |= flags_rsvd;						\
    }





#define MAKE_2OP_64STR_INST(iname) static inline void iname##64(addr_t * dst, \
								addr_t * src, \
								addr_t * ecx, addr_t * flags) { \
	/* Some of the flags values are not copied out in a pushf, we save them here */ \
	addr_t flags_rsvd = *flags & ~0xfffe7fff;			\
									\
	asm volatile (							\
		      "pushfq; "					\
		      "pushq %4; "					\
		      "popfq; "						\
		      "rep; "						\
		      #iname"q; "					\
		      "pushfq; "					\
		      "popq %0; "					\
		      "popfq; "						\
		      : "=q"(*flags)					\
		      : "D"(*dst),"S"(*src),"c"(*ecx),"q"(*flags)	\
		      );						\
	*flags |= flags_rsvd;						\
    }


#define MAKE_2OP_32STR_INST(iname) static inline void iname##32(addr_t * dst, \
								addr_t * src, \
								addr_t * ecx, addr_t * flags) { \
	/* Some of the flags values are not copied out in a pushf, we save them here */ \
	addr_t flags_rsvd = *flags & ~0xfffe7fff;			\
									\
	asm volatile (							\
		      "pushf; "						\
		      "push %4; "					\
		      "popf; "						\
		      "rep; "						\
		      #iname"l; "					\
		      "pushf; "						\
		      "pop %0; "					\
		      "popf; "						\
		      : "=q"(*flags)					\
		      : "D"(*dst),"S"(*src),"c"(*ecx),"q"(*flags)	\
		      );						\
	*flags |= flags_rsvd;						\
    }

#define MAKE_2OP_16STR_INST(iname) static inline void iname##16(addr_t * dst, \
								addr_t * src, \
								addr_t * ecx, addr_t * flags) { \
	/* Some of the flags values are not copied out in a pushf, we save them here */ \
	addr_t flags_rsvd = *flags & ~0xfffe7fff;			\
									\
	asm volatile (							\
		      "pushf; "						\
		      "push %4; "					\
		      "popf; "						\
		      "rep; "						\
		      #iname"w; "					\
		      "pushf; "						\
		      "pop %0; "					\
		      "popf; "						\
		      : "=q"(*flags)					\
		      : "D"(*dst),"S"(*src),"c"(*ecx),"q"(*flags)	\
		      );						\
	*flags |= flags_rsvd;						\
    }



#define MAKE_2OP_8STR_INST(iname) static inline void iname##8(addr_t * dst, \
							      addr_t * src, \
							      addr_t * ecx, addr_t * flags) { \
	/* Some of the flags values are not copied out in a pushf, we save them here */ \
	addr_t flags_rsvd = *flags & ~0xfffe7fff;			\
									\
	asm volatile (							\
		      "pushf; "						\
		      "push %4; "					\
		      "popf; "						\
		      "rep; "						\
		      #iname"b; "					\
		      "pushf; "						\
		      "pop %0; "					\
		      "popf; "						\
		      : "=q"(*flags)					\
		      : "D"(*dst),"S"(*src),"c"(*ecx),"q"(*flags)	\
		      );						\
	*flags |= flags_rsvd;						\
    }




#define MAKE_1OP_64STR_INST(iname) static inline void iname##64(addr_t * dst, \
								addr_t * src, \
								addr_t * ecx, addr_t * flags) { \
	/* Some of the flags values are not copied out in a pushf, we save them here */ \
	addr_t flags_rsvd = *flags & ~0xfffe7fff;			\
									\
	asm volatile (							\
		      "pushfq; "					\
		      "pushq %4; "					\
		      "popfq; "						\
		      "rep; "						\
		      #iname"q; "					\
		      "pushfq; "					\
		      "popq %0; "					\
		      "popfq; "						\
		      : "=q"(*flags)					\
		      : "D"(*dst),"a"(*src),"c"(*ecx),"q"(*flags)	\
		      );						\
									\
	*flags |= flags_rsvd;						\
    }


#define MAKE_1OP_32STR_INST(iname) static inline void iname##32(addr_t * dst, \
								addr_t * src, \
								addr_t * ecx, addr_t * flags) { \
	/* Some of the flags values are not copied out in a pushf, we save them here */ \
	addr_t flags_rsvd = *flags & ~0xfffe7fff;			\
									\
	asm volatile (							\
		      "pushf; "						\
		      "push %4; "					\
		      "popf; "						\
		      "rep; "						\
		      #iname"l; "					\
		      "pushf; "						\
		      "pop %0; "					\
		      "popf; "						\
		      : "=q"(*flags)					\
		      : "D"(*dst),"a"(*src),"c"(*ecx),"q"(*flags)	\
		      );						\
	*flags |= flags_rsvd;						\
    }

#define MAKE_1OP_16STR_INST(iname) static inline void iname##16(addr_t * dst, \
								addr_t * src, \
								addr_t * ecx, addr_t * flags) { \
	/* Some of the flags values are not copied out in a pushf, we save them here */ \
	addr_t flags_rsvd = *flags & ~0xfffe7fff;			\
									\
	asm volatile (							\
		      "pushf; "						\
		      "push %4; "					\
		      "popf; "						\
		      "rep; "						\
		      #iname"w; "					\
		      "pushf; "						\
		      "pop %0; "					\
		      "popf; "						\
		      : "=q"(*flags)					\
		      : "D"(*dst),"a"(*src),"c"(*ecx),"q"(*flags)	\
		      );						\
	*flags |= flags_rsvd;						\
    }



#define MAKE_1OP_8STR_INST(iname) static inline void iname##8(addr_t * dst, \
							      addr_t * src, \
							      addr_t * ecx, addr_t * flags) { \
	/* Some of the flags values are not copied out in a pushf, we save them here */ \
	addr_t flags_rsvd = *flags & ~0xfffe7fff;			\
									\
	asm volatile (							\
		      "pushf; "						\
		      "push %4; "					\
		      "popf; "						\
		      "rep; "						\
		      #iname"b; "					\
		      "pushf; "						\
		      "pop %0; "					\
		      "popf; "						\
		      : "=q"(*flags)					\
		      : "D"(*dst),"a"(*src),"c"(*ecx),"q"(*flags)	\
		      );						\
	*flags |= flags_rsvd;						\
    }




#define MAKE_2OP_64_INST(iname) static inline void iname##64(addr_t * dst, addr_t * src) { \
	asm volatile (							\
		      #iname"q %1, %0; "				\
		      : "=q"(*(uint64_t *)dst)				\
		      : "q"(*(uint64_t *)src), "0"(*(uint64_t *)dst)	\
		      );						\
    }

#define MAKE_2OP_32_INST(iname) static inline void iname##32(addr_t * dst, addr_t * src) { \
	asm volatile (							\
		      #iname"l %1, %0; "				\
		      : "=q"(*(uint32_t *)dst)				\
		      : "q"(*(uint32_t *)src), "0"(*(uint32_t *)dst)	\
		      );						\
    }

#define MAKE_2OP_16_INST(iname) static inline void iname##16(addr_t * dst, addr_t * src) { \
	asm volatile (							\
		      #iname"w %1, %0; "				\
		      : "=q"(*(uint16_t *)dst)				\
		      : "q"(*(uint16_t *)src), "0"(*(uint16_t *)dst)	\
		      );						\
    }

#define MAKE_2OP_8_INST(iname) static inline void iname##8(addr_t * dst, addr_t * src) { \
	asm volatile (							\
		      #iname"b %1, %0; "				\
		      : "=q"(*(uint8_t *)dst)				\
		      : "q"(*(uint8_t *)src), "0"(*(uint8_t *)dst)	\
		      );						\
    }





#define MAKE_2OP_8EXT_INST(iname) static inline void iname##8(addr_t * dst, addr_t * src, uint_t dst_len) { \
	if (dst_len == 2) {						\
	    asm volatile (						\
			  #iname" %1, %0; "				\
			  : "=q"(*(uint16_t *)dst)			\
			  : "q"(*(uint8_t *)src), "0"(*(uint16_t *)dst)	\
			  );						\
	} else if (dst_len == 4) {					\
	    asm volatile (						\
			  #iname" %1, %0; "				\
			  : "=q"(*(uint32_t *)dst)			\
			  : "q"(*(uint8_t *)src), "0"(*(uint32_t *)dst)	\
			  );						\
	} else if (dst_len == 8) {					\
	    asm volatile (						\
			  #iname" %1, %0; "				\
			  : "=q"(*(uint64_t *)dst)			\
			  : "q"(*(uint8_t *)src), "0"(*(uint64_t *)dst)	\
			  );						\
	}								\
    }

#define MAKE_2OP_16EXT_INST(iname) static inline void iname##16(addr_t * dst, addr_t * src, uint_t dst_len) { \
	if (dst_len == 4) {						\
	    asm volatile (						\
			  #iname" %1, %0; "				\
			  : "=q"(*(uint32_t *)dst)			\
			  : "q"(*(uint16_t *)src), "0"(*(uint32_t *)dst) \
			  );						\
	} else if (dst_len == 8) {					\
	    asm volatile (						\
			  #iname" %1, %0; "				\
			  : "=q"(*(uint64_t *)dst)			\
			  : "q"(*(uint16_t *)src), "0"(*(uint64_t *)dst) \
			  );						\
	}								\
    }




#define MAKE_2OUT_64_INST(iname) static inline void iname##64(addr_t * dst, addr_t * src) { \
	asm volatile (							\
		      #iname"q %1, %0; "				\
		      : "=q"(*(uint64_t *)dst), "=q"(*(uint64_t *)src)	\
		      : "0"(*(uint64_t *)dst), "1"(*(uint64_t *)src)	\
		      );						\
    }

#define MAKE_2OUT_32_INST(iname) static inline void iname##32(addr_t * dst, addr_t * src) { \
	asm volatile (							\
		      #iname"l %1, %0; "				\
		      : "=q"(*(uint32_t *)dst), "=q"(*(uint32_t *)src)	\
		      :  "0"(*(uint32_t *)dst), "1"(*(uint32_t *)src)	\
		      );						\
    }

#define MAKE_2OUT_16_INST(iname) static inline void iname##16(addr_t * dst, addr_t * src) { \
	asm volatile (							\
		      #iname"w %1, %0; "				\
		      : "=q"(*(uint16_t *)dst), "=q"(*(uint16_t *)src)	\
		      : "0"(*(uint16_t *)dst), "1"(*(uint16_t *)src)	\
		      );						\
    }

#define MAKE_2OUT_8_INST(iname) static inline void iname##8(addr_t * dst, addr_t * src) { \
	asm volatile (							\
		      #iname"b %1, %0; "				\
		      : "=q"(*(uint8_t *)dst), "=q"(*(uint8_t *)src)	\
		      : "0"(*(uint8_t *)dst), "1"(*(uint8_t *)src)	\
		      );						\
    }






/****************************/
/* 8 Bit instruction forms  */
/****************************/

MAKE_2OP_8FLAGS_INST(adc);
MAKE_2OP_8FLAGS_INST(add);
MAKE_2OP_8FLAGS_INST(and);
MAKE_2OP_8FLAGS_INST(or);
MAKE_2OP_8FLAGS_INST(xor);
MAKE_2OP_8FLAGS_INST(sub);


MAKE_1OP_8FLAGS_INST(inc);
MAKE_1OP_8FLAGS_INST(dec);
MAKE_1OP_8FLAGS_INST(neg);
MAKE_1OP_8FLAGS_INST(setb);
MAKE_1OP_8FLAGS_INST(setbe);
MAKE_1OP_8FLAGS_INST(setl);
MAKE_1OP_8FLAGS_INST(setle);
MAKE_1OP_8FLAGS_INST(setnb);
MAKE_1OP_8FLAGS_INST(setnbe);
MAKE_1OP_8FLAGS_INST(setnl);
MAKE_1OP_8FLAGS_INST(setnle);
MAKE_1OP_8FLAGS_INST(setno);
MAKE_1OP_8FLAGS_INST(setnp);
MAKE_1OP_8FLAGS_INST(setns);
MAKE_1OP_8FLAGS_INST(setnz);
MAKE_1OP_8FLAGS_INST(seto);
MAKE_1OP_8FLAGS_INST(setp);
MAKE_1OP_8FLAGS_INST(sets);
MAKE_1OP_8FLAGS_INST(setz);


MAKE_1OP_8_INST(not);

MAKE_2OP_8_INST(mov);
MAKE_2OP_8EXT_INST(movzx);
MAKE_2OP_8EXT_INST(movsx);

MAKE_2OUT_8_INST(xchg);

MAKE_2OP_8STR_INST(movs);
MAKE_1OP_8STR_INST(stos);
MAKE_1OP_8STR_INST(scas);


/****************************/
/* 16 Bit instruction forms */
/****************************/
MAKE_2OP_16FLAGS_INST(adc);
MAKE_2OP_16FLAGS_INST(add);
MAKE_2OP_16FLAGS_INST(and);
MAKE_2OP_16FLAGS_INST(or);
MAKE_2OP_16FLAGS_INST(xor);
MAKE_2OP_16FLAGS_INST(sub);


MAKE_1OP_16FLAGS_INST(inc);
MAKE_1OP_16FLAGS_INST(dec);
MAKE_1OP_16FLAGS_INST(neg);

MAKE_1OP_16_INST(not);

MAKE_2OP_16_INST(mov);
MAKE_2OP_16EXT_INST(movzx);
MAKE_2OP_16EXT_INST(movsx);
MAKE_2OUT_16_INST(xchg);

MAKE_2OP_16STR_INST(movs);
MAKE_1OP_16STR_INST(stos);
MAKE_1OP_16STR_INST(scas);


/****************************/
/* 32 Bit instruction forms */
/****************************/
MAKE_2OP_32FLAGS_INST(adc);
MAKE_2OP_32FLAGS_INST(add);
MAKE_2OP_32FLAGS_INST(and);
MAKE_2OP_32FLAGS_INST(or);
MAKE_2OP_32FLAGS_INST(xor);
MAKE_2OP_32FLAGS_INST(sub);


MAKE_1OP_32FLAGS_INST(inc);
MAKE_1OP_32FLAGS_INST(dec);
MAKE_1OP_32FLAGS_INST(neg);

MAKE_1OP_32_INST(not);

MAKE_2OP_32_INST(mov);

MAKE_2OUT_32_INST(xchg);



MAKE_2OP_32STR_INST(movs);
MAKE_1OP_32STR_INST(stos);
MAKE_1OP_32STR_INST(scas);



#ifdef __V3_64BIT__

/****************************/
/* 64 Bit instruction forms */
/****************************/
MAKE_2OP_64FLAGS_INST(adc);
MAKE_2OP_64FLAGS_INST(add);
MAKE_2OP_64FLAGS_INST(and);
MAKE_2OP_64FLAGS_INST(or);
MAKE_2OP_64FLAGS_INST(xor);
MAKE_2OP_64FLAGS_INST(sub);

MAKE_1OP_64FLAGS_INST(inc);
MAKE_1OP_64FLAGS_INST(dec);
MAKE_1OP_64FLAGS_INST(neg);

MAKE_1OP_64_INST(not);


MAKE_2OP_64_INST(mov);
MAKE_2OP_64STR_INST(movs);
MAKE_1OP_64STR_INST(stos);
MAKE_1OP_64STR_INST(scas);

MAKE_2OUT_64_INST(xchg);


#endif
