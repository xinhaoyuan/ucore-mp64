#ifndef __GLUE_KIO_H__
#define __GLUE_KIO_H__

#include <types.h>
#include <stdarg.h>

#define vkprintf   (*vkprintf_ptr)
int                  kprintf(const char *fmt, ...);
#define kputchar   (*kputchar_ptr)
#define kreadline  (*kreadline_ptr)
#define kcons_getc (*kcons_getc_ptr)

extern int   vkprintf(const char *fmt, va_list ap);
extern int   kputchar(int c);
extern char *kreadline(const char *prompt);
extern int   kcons_getc(void);

#define KRES_COUNT 1024

#define kacquire     (*kacquire_ptr)
#define kacquire_try (*kacquire_try_ptr)
#define krelease     (*krelease_ptr)

#endif
