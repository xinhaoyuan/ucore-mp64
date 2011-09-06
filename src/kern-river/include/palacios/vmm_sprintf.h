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

#ifndef __VMM_SPRINTF_H__
#define __VMM_SPRINTF_H__

#ifdef __V3VEE__
#include <palacios/vmm_types.h>
#include <stdarg.h>

int sprintf(char *buf, const char *cfmt, ...);
//        __attribute__ ((format (printf, 1, 2)));
//int vsprintf(char *buf, const char * cfmt, va_list ap);
int snprintf(char *str, size_t size, const char * fmt, ...);
int vsnprintf(char *str, size_t size, const char * fmt, va_list ap);
//int vsnrprintf(char *str, size_t size, int radix, const char * fmt, va_list ap);

#define HD_COLUMN_MASK  0xff
#define HD_DELIM_MASK   0xff00
#define HD_OMIT_COUNT   (1 << 16)
#define HD_OMIT_HEX     (1 << 17)
#define HD_OMIT_CHARS   (1 << 18)


void v3_hexdump(const void * ptr, int length, const char * hdr, int flags);

#endif



#endif
