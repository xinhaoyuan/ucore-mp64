#ifndef __GLUE_KIO_H__
#define __GLUE_KIO_H__

#include <types.h>
#include <stdarg.h>

extern int (*vkprintf)(const char *fmt, va_list ap);
int          kprintf(const char *fmt, ...);

#endif
