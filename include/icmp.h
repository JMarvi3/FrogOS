#ifndef _ICMP_H
#define _ICMP_H

#include <stdint.h>
#include <stddef.h>
#include <net.h>
#include <ether.h>
#include <ipv4.h>

typedef struct __attribute__ ((packed)) {
	uint8_t type, code;
	uint16_t cksum;
	uint32_t data;
	uint8_t payload[1];
} icmp_packet;

void icmp_process_packet(net_dev *dev, ether_frame *frame, ip_packet *ip_packet, icmp_packet *packet, size_t len);

#endif
