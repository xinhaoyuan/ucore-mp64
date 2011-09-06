/* * String library
 * Copyright (c) 2001,2004 David H. Hovemeyer <daveho@cs.umd.edu>
 * Copyright (c) 2003, Jeffrey K. Hollingsworth <hollings@cs.umd.edu>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
 * ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
 * TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
 * SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
 * THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/* Modifications:
 * 2008, Jack Lange <jarusl@cs.northwestern.edu> 
 * 2008, Lei Xia <xiaxlei@gmail.com> 
 */

#ifndef __VMM_STRING_H__
#define __VMM_STRING_H__

#ifdef __V3VEE__

#include <palacios/vmm_types.h>


void * memset(void* s, int c, size_t n);
void * memcpy(void *dst, const void* src, size_t n);
void * memmove(void *dst, const void *src, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);
size_t strlen(const char* s);
size_t strnlen(const char *s, size_t maxlen);
int strcmp(const char* s1, const char* s2);
int strcasecmp(const char* s1, const char* s2);
int strncmp(const char* s1, const char* s2, size_t limit);
int strncasecmp(const char* s1, const char* s2, size_t limit);
char *strcat(char *s1, const char *s2);
char *strncat(char *s1, const char *s2, size_t limit);
char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, size_t limit);
char *strdup(const char *s1);
int atoi(const char * buf);
uint64_t atox(const char * buf);
int strtoi(const char * nptr, char ** endptr);
uint64_t strtox(const char * nptr, char ** endptr);
char *strchr(const char *s, int c);
char *strrchr(const char *s, int c);
char *strpbrk(const char *s, const char *accept);

size_t strspn(const char * s, const char * accept);
size_t strcspn(const char * s, const char * reject);
char * strstr(const char * haystack, const char * needle);

void str_tolower(char * s);
void str_toupper(char * s);



#define isspace(c)      (c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v')
#define isascii(c)	(((c) & ~0x7f) == 0)
#define isupper(c)	((c) >= 'A' && (c) <= 'Z')
#define islower(c)	((c) >= 'a' && (c) <= 'z')
#define isalpha(c)	(isupper(c) || islower(c))
#define isdigit(c)	((c) >= '0' && (c) <= '9')
#define isxdigit(c)	(isdigit(c) \
			  || ((c) >= 'A' && (c) <= 'F') \
			  || ((c) >= 'a' && (c) <= 'f'))
#define isprint(c)	((c) >= ' ' && (c) <= '~')

#define toupper(c)	((c) - 0x20 * (((c) >= 'a') && ((c) <= 'z')))
#define tolower(c)	((c) + 0x20 * (((c) >= 'A') && ((c) <= 'Z')))


#endif // !__V3VEE__

#endif  /* STRING_H */
