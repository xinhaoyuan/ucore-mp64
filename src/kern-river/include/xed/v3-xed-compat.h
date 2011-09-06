#ifndef __V3_XED_COMPAT_H__
#define __V3_XED_COMPAT_H__


#include <palacios/vmm_types.h>

/* Definition of the control structure for streams
*/
typedef struct file_struct {
        short           level;          /* fill/empty level of buffer */
        unsigned        flags;          /* File status flags    */
        char            fd;             /* File descriptor      */
        unsigned char   hold;           /* Ungetc char if no buffer */
        short           bsize;          /* Buffer size          */
        unsigned char   *buffer;        /* Data transfer buffer */
        unsigned char   *curp;          /* Current active pointer */
        unsigned        istemp;         /* Temporary file indicator */
        short           token;          /* Used for validity checking */
}       FILE;    



int fprintf(FILE *file, char *fmt, ...);
int printf(char *fmt, ...);
int fflush(FILE *stream);
void abort(void);


#endif
