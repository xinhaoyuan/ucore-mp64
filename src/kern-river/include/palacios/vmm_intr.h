/*
 * This file is part of the Palacios Virtual Machine Monitor developed
 * by the V3VEE Project with funding from the United States National 
 * Science Foundation and the Department of Energy.  
 *
 * The V3VEE Project is a joint project between Northwestern University
 * and the University of New Mexico.  You can find out more at 
 * http://www.v3vee.org
 *
 * Copyright (c) 2008, Jack Lange <jarusl@cs.northwestern.edu> 
 * Copyright (c) 2008, The V3VEE Project <http://www.v3vee.org> 
 * All rights reserved.
 *
 * Author: Jack Lange <jarusl@cs.northwestern.edu>
 *
 * This is free software.  You are permitted to use,
 * redistribute, and modify it as specified in the file "V3VEE_LICENSE".
 */

#ifndef __VMM_INTR_H_
#define __VMM_INTR_H_


#ifdef __V3VEE__

#include <palacios/vmm_types.h>
#include <palacios/vmm_list.h>
#include <palacios/vmm_lock.h>


typedef enum {V3_INVALID_INTR, V3_EXTERNAL_IRQ, V3_VIRTUAL_IRQ, V3_NMI, V3_SOFTWARE_INTR} v3_intr_type_t;

struct guest_info;
struct v3_interrupt;



struct v3_irq_hook {
    int (*handler)(struct guest_info * info, struct v3_interrupt * intr, void * priv_data);
    void * priv_data;
};

#define MAX_IRQ 256




struct v3_intr_state {

    struct list_head controller_list;

    uint_t irq_pending;
    uint_t irq_started;
    uint_t irq_vector;

    uint8_t virq_map[MAX_IRQ / 8];

    v3_lock_t irq_lock;

    /* some way to get the [A]PIC intr */
    struct v3_irq_hook * hooks[256];
  
};



void v3_init_interrupt_state(struct guest_info * info);


int v3_raise_virq(struct guest_info * info, int irq);
int v3_lower_virq(struct guest_info * info, int irq);

int v3_raise_irq(struct guest_info * info, int irq);
int v3_lower_irq(struct guest_info * info, int irq);



struct intr_ctrl_ops {
    int (*intr_pending)(struct guest_info * info, void * private_data);
    int (*get_intr_number)(struct guest_info * info, void * private_data);
    int (*raise_intr)(struct guest_info * info, void * private_data, int irq);
    int (*lower_intr)(struct guest_info * info, void * private_data, int irq);
    int (*begin_irq)(struct guest_info * info, void * private_data, int irq);
};


void v3_clear_pending_intr(struct guest_info * core);

void v3_register_intr_controller(struct guest_info * info, struct intr_ctrl_ops * ops, void * state);

v3_intr_type_t v3_intr_pending(struct guest_info * info);
uint32_t v3_get_intr(struct guest_info * info);

//intr_type_t v3_get_intr_type(struct guest_info * info);

int v3_injecting_intr(struct guest_info * info, uint_t intr_num, v3_intr_type_t type);

/*
  int start_irq(struct vm_intr * intr);
  int end_irq(struct vm_intr * intr, int irq);
*/



int v3_hook_irq(struct guest_info * info, 
		uint_t irq,
		int (*handler)(struct guest_info * info, struct v3_interrupt * intr, void * priv_data),
		void  * priv_data);

int v3_hook_passthrough_irq(struct guest_info *info, uint_t irq);



#endif // !__V3VEE__



#endif
