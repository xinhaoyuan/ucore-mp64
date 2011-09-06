/*BEGIN_LEGAL 
Intel Open Source License 

Copyright (c) 2002-2007 Intel Corporation 
All rights reserved. 
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.  Redistributions
in binary form must reproduce the above copyright notice, this list of
conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.  Neither the name of
the Intel Corporation nor the names of its contributors may be used to
endorse or promote products derived from this software without
specific prior written permission.
 
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE INTEL OR
ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
END_LEGAL */
/// @file xed-types.h
/// @author Mark Charney   <mark.charney@intel.com> 


#ifndef _XED_TYPES_H_
# define _XED_TYPES_H_

////////////////////////////////////////////////////////////////////////////

#include <xed/xed-common-hdrs.h>


   //typedef unsigned char		uint8_t;
//typedef unsigned short int	uint16_t;
//typedef unsigned int		uint32_t;
//typedef unsigned long int	uint64_t;

typedef signed char		int8_t;
typedef short int		int16_t;
typedef int			int32_t;
typedef long int		int64_t;


#if defined(__GNUC__) || defined(__ICC)
//#  include <stdint.h>
#  define xed_uint8_t   uint8_t 
#  define xed_uint16_t  uint16_t
#  define xed_uint32_t  uint32_t
#  define xed_uint64_t  uint64_t
#  define xed_int8_t     int8_t 
#  define xed_int16_t 	 int16_t
#  define xed_int32_t    int32_t
#  define xed_int64_t    int64_t
#elif defined(_WIN32)
#  define xed_uint8_t  unsigned __int8
#  define xed_uint16_t unsigned __int16
#  define xed_uint32_t unsigned __int32
#  define xed_uint64_t unsigned __int64
#  define xed_int8_t   __int8
#  define xed_int16_t  __int16
#  define xed_int32_t  __int32
#  define xed_int64_t  __int64
#else
#  error "XED types unsupported platform? Need windows, gcc, or icc."
#endif

typedef unsigned int  xed_uint_t;
typedef unsigned int  xed_bits_t;
typedef unsigned int  xed_bool_t;


////////////////////////////////////////////////////////////////////////////
#endif
