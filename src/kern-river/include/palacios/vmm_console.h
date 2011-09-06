/*
 * This file is part of the Palacios Virtual Machine Monitor developed
 * by the V3VEE Project with funding from the United States National 
 * Science Foundation and the Department of Energy.  
 *
 * The V3VEE Project is a joint project between Northwestern University
 * and the University of New Mexico.  You can find out more at 
 * http://www.v3vee.org
 *
 * Copyright (c) 2010, Jack Lange <jarusl@cs.northwestern.edu> 
 * Copyright (c) 2010, Erik van der Kouwe <vdkouwe@cs.vu.nl>
 * Copyright (c) 2010, The V3VEE Project <http://www.v3vee.org> 
 * All rights reserved.
 *
 * Author: Jack Lange <jarusl@cs.northwestern.edu>
 * Author: Erik van der Kouwe <vdkouwe@cs.vu.nl>
 *
 * This is free software.  You are permitted to use,
 * redistribute, and modify it as specified in the file "V3VEE_LICENSE".
 */


#ifndef __VMM_CONSOLE_H__
#define __VMM_CONSOLE_H__

#include <palacios/vmm.h>


#ifdef __V3VEE__

#define V3_TtyOpen(path, mode)						\
    ({									\
	extern struct v3_console_hooks *console_hooks;				\
	((console_hooks) && (console_hooks)->tty_open) ?				\
	    (console_hooks)->tty_open((path), (mode)) : NULL;		\
    })

#define V3_TtyCursorSet(tty, x, y)					\
    ({									\
	extern struct v3_console_hooks *console_hooks;				\
	((console_hooks) && (console_hooks)->tty_cursor_set) ?			\
	    (console_hooks)->tty_cursor_set((tty), (x), (y)) : -1;		\
    })

#define V3_TtyCharacterSet(tty, x, y, c, style)				\
    ({									\
	extern struct v3_console_hooks *console_hooks;				\
	((console_hooks) && (console_hooks)->tty_character_set) ?			\
	    (console_hooks)->tty_character_set((tty), (x), (y), (c), (style)) : -1; \
    })

#define V3_TtyScroll(tty, lines)					\
    ({									\
	extern struct v3_console_hooks *console_hooks;				\
	((console_hooks) && (console_hooks)->tty_scroll) ?			\
	    (console_hooks)->tty_scroll((tty), (lines)) : -1;		\
    })

#define V3_TtyUpdate(tty)						\
    ({									\
	extern struct v3_console_hooks *console_hooks;				\
	((console_hooks) && (console_hooks)->tty_update) ?			\
	    (console_hooks)->tty_update((tty)) : -1;				\
    })

#endif

#define TTY_OPEN_MODE_READ	(1 << 0)
#define TTY_OPEN_MODE_WRITE	(1 << 1)

struct v3_console_hooks {
    /* open console device, mode is a combination of TTY_OPEN_MODE_* flags */
    void *(*tty_open)(const char *path, int mode);

    /* set cursor position */
    int (*tty_cursor_set)(void *tty, int x, int y);

    /* output character c with specified style at (x, y) */
    int (*tty_character_set)(void *tty, int x, int y, char c, unsigned char style);

    /* scroll the console down the specified number of lines */
    int (*tty_scroll)(void *tty, int lines);

    /* force update of console display; all updates by above functions
     * may be defferred until the next tty_update call 
     */
    int (*tty_update)(void *tty);
};


extern void V3_Init_Console(struct v3_console_hooks * hooks);

#endif
