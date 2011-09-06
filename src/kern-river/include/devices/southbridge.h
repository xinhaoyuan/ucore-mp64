/* 
 * This file is part of the Palacios Virtual Machine Monitor developed
 * by the V3VEE Project with funding from the United States National 
 * Science Foundation and the Department of Energy.  
 *
 * The V3VEE Project is a joint project between Northwestern University
 * and the University of New Mexico.  You can find out more at 
 * http://www.v3vee.org
 *
 * Copyright (c) 2009, Lei Xia <lxia@northwestern.edu>
 * Copyright (c) 2009, Chang Seok Bae <jhuell@gmail.com>
 * Copyright (c) 2009, The V3VEE Project <http://www.v3vee.org> 
 * All rights reserved.
 *
 * Author:  Lei Xia <lxia@northwestern.edu>
 *             Chang Seok Bae <jhuell@gmail.com>
 *
 * This is free software.  You are permitted to use,
 * redistribute, and modify it as specified in the file "V3VEE_LICENSE".
 */

#ifndef __DEVICES_SOUTHBRIDGE_H__
#define __DEVICES_SOUTHBRIDGE_H__


#ifdef __V3VEE__


typedef enum {V3_SB_INVALID, V3_SB_PIIX3, V3_SB_PIIX4, V3_SB_CMD646} v3_southbridge_type_t;

struct v3_southbridge {
    struct vm_device * pci_bus;
    struct pci_device * southbridge_pci;

    v3_southbridge_type_t type;
};


#endif // ! __V3VEE__

#endif
