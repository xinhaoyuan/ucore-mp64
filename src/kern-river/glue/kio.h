#ifndef __GLUE_KIO_H__
#define __GLUE_KIO_H__

#include <libs/types.h>
#include <libs/stdarg.h>

#define kputchar   (*kputchar_ptr)
#define kcons_getc (*kcons_getc_ptr)

extern void kputchar(int c);
extern int  kcons_getc(void);

#endif
