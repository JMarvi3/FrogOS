#ifndef _IPV4_H
#define _IPV4_H

#include <stdint.h>
#include <stddef.h>
#include <net.h>
#include <ether.h>

typedef struct __attribute__ ((packed)) {
	unsigned int ihl:4;
	unsigned int version:4;
	uint8_t tos;
	uint16_t tot_len, id, frag_off;
	uint8_t ttl, protocol;
	uint16_t check;
	uint32_t src, dest;
	uint8_t payload[1];
} ip_packet;

const char *inet_ntoa(struct in_addr addr);
uint16_t ip_cksum(uint16_t prev, void *buff, size_t len);
void ipv4_process_packet(net_dev *dev, ether_frame *frame, ip_packet *packet, size_t len);

#endif
