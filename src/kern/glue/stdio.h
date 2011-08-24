#ifndef __GLUE_STDIO_H__
#define __GLUE_STDIO_H__

#include <types.h>
#include <stdarg.h>

extern int (*vkprintf)(const char *fmt, va_list ap);
int          kprintf(const char *fmt, ...);

#endif
