#ifndef __GLUE_KIO_H__
#define __GLUE_KIO_H__

#include <libs/types.h>
#include <libs/stdarg.h>

#define kcons_putc (*kcons_putc_ptr)
#define kcons_getc (*kcons_getc_ptr)

extern void kcons_putc(int c);
extern int  kcons_getc(void);

#endif
