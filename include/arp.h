#ifndef _ARP_H
#define _ARP_H

#include <stdint.h>
#include <ipv4.h>
#include <net.h>

typedef struct __attribute__((packed)) {
 uint16_t htype, ptype;
 uint8_t hlen, plen;
 uint16_t oper;
 uint8_t sha[6];
 in_addr spa;
 uint8_t tha[6];
 in_addr tpa;
} arp_packet;

void arp_process_packet(net_dev *dev,uint8_t *src, uint8_t *dest, arp_packet *packet);

#endif
