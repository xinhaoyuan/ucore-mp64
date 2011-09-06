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
 * Copyright (c) 2009, Yuan Tang <ytang@northwestern.edu> 
 * Copyright (c) 2009, The V3VEE Project <http://www.v3vee.org> 
 * All rights reserved.
 *
 * Author: Lei Xia <lxia@northwestern.edu>
 *		  Yuan Tang <ytang@northwestern.edu>
 *
 * This is free software.  You are permitted to use,
 * redistribute, and modify it as specified in the file "V3VEE_LICENSE".
 */

#ifndef __VNET_H__
#define __VNET_H__

#include <palacios/vmm.h>
#include <palacios/vmm_string.h>
#include <palacios/vmm_types.h>
#include <palacios/vmm_queue.h>
#include <palacios/vmm_hashtable.h>
#include <palacios/vmm_sprintf.h>

#define ETHERNET_HEADER_LEN 14
#define ETHERNET_DATA_MIN   46
#define ETHERNET_DATA_MAX   1500
#define ETHERNET_PACKET_LEN (ETHERNET_HEADER_LEN + ETHERNET_DATA_MAX)

typedef enum {MAC_ANY, MAC_NOT, MAC_NONE} mac_type_t; //for 'src_mac_qual' and 'dst_mac_qual'
typedef enum {LINK_INTERFACE, LINK_EDGE, LINK_ANY} link_type_t; //for 'type' and 'src_type' in struct routing
typedef enum {TCP_TYPE, UDP_TYPE, NONE_TYPE} prot_type_t;

#define VNET_INITAB_HCALL 0xca00

//routing table entry
struct routing_entry{
    char src_mac[6];
    char dest_mac[6];

    int src_mac_qual;
    int dest_mac_qual;

    int link_idx; //link[dest] is the link to be used to send pkt
    link_type_t link_type; //EDGE|INTERFACE|ANY
 
    int src_link_idx;
    link_type_t src_type; //EDGE|INTERFACE|ANY
}__attribute__((packed));


struct eth_header {
    uchar_t dest[6];
    uchar_t src[6];
    uint16_t type; // indicates layer 3 protocol type
}__attribute__((packed));

struct ip_header {
    uint8_t version: 4;
    uint8_t hdr_len: 4;
    uchar_t tos;
    uint16_t total_len;
    uint16_t id;
    uint8_t flags:     3;
    uint16_t offset: 13;
    uchar_t ttl;
    uchar_t proto;
    uint16_t cksum;
    uint32_t src_addr;
    uint32_t dst_addr;
}__attribute__((packed));

struct udp_header {
    uint16_t src_port;
    uint16_t dst_port;
    uint16_t len;
    uint16_t csum;//set to zero, disable the xsum
}__attribute__((packed));

struct udp_link_header {
    struct eth_header eth_hdr;
    struct ip_header ip_hdr;
    struct udp_header udp_hdr;
}__attribute__((packed));

#define DEVICE_NAME_LEN 20
struct vnet_if_device {
    char name[DEVICE_NAME_LEN];
    uchar_t mac_addr[6];
    struct vm_device *dev;
    
    int (*input)(uchar_t *data, uint32_t len, void *private_data);
    
    void *private_data;
}__attribute__((packed));


struct vnet_if_link {
    prot_type_t pro_type; //transport layer protocal type of this link
    unsigned long dest_ip;
    uint16_t dest_port;

    struct udp_link_header vnet_header; //header applied to the packet in/out from this link

    int (*input)(uchar_t *data, uint32_t len, void *private_data);
    
    void *private_data;
}__attribute__((packed));


struct link_entry {
    link_type_t type;
  
    union {
	struct vnet_if_device *dst_dev;
	struct vnet_if_link *dst_link;
    } __attribute__((packed));

    int use;
}__attribute__((packed));

struct ethernet_pkt {
    uint32_t size; //size of data field
    uint16_t type;
    uint8_t use_header;
    struct udp_link_header ext_hdr;
    char data[ETHERNET_PACKET_LEN];
}__attribute__((packed));

int v3_vnet_send_rawpkt(uchar_t *buf, int len, void *private_data);
int v3_vnet_send_udppkt(uchar_t *buf, int len, void *private_data);
int vnet_register_device(struct vm_device *vdev, 
			 char *dev_name, 
			 uchar_t mac[6], 
			 int (*netif_input)(uchar_t * pkt, uint_t size, void *private_data), 
			 void *data);

int vnet_add_route_entry(char src_mac[6],
				char dest_mac[6],
				int src_mac_qual,
				int dest_mac_qual,
				int link_idx,
				link_type_t link_type,
				int src_link_idx,
				link_type_t src_link_type);

int v3_vnet_pkt_process(); 

void v3_init_vnet();

#endif


