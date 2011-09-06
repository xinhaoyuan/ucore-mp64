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

#ifndef __VMM_CONFIG_H__
#define __VMM_CONFIG_H__



#ifdef __V3VEE__

//#include <palacios/vm_guest.h>
#include <palacios/vmm.h>
#include <palacios/vmm_xml.h>
#include <palacios/vmm_list.h>
#include <palacios/vmm_hashtable.h>
//#include <palacios/svm.h>


struct guest_info;

int v3_config_guest(struct guest_info * info, void * cfg_blob);

struct v3_cfg_file {
    void * data;
    uint64_t size;

    char tag[256];

    struct list_head file_node;
};



typedef struct v3_xml v3_cfg_tree_t;

struct v3_config {
    v3_cfg_tree_t * cfg;

    struct list_head file_list;
    struct hashtable * file_table;

    void * blob;
};


struct v3_cfg_file * v3_cfg_get_file(struct guest_info * info, char * tag);

char * v3_cfg_val(v3_cfg_tree_t * tree, char * tag);
v3_cfg_tree_t * v3_cfg_subtree(v3_cfg_tree_t * tree, char * tag);
v3_cfg_tree_t * v3_cfg_next_branch(v3_cfg_tree_t * tree);

#endif // ! __V3VEE__

#endif
