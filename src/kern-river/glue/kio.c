#include <glue/kio.h>

int
kprintf(const char *fmt, ...)
{
	va_list ap;
    int cnt;
    va_start(ap, fmt);
    cnt = vkprintf(fmt, ap);
    va_end(ap);
    return cnt;
}
