#ifndef _NET_H
#define _NET_H

#include <stdint.h>
#include <ipv4.h>

typedef struct net_dev {
	uint16_t dev_no;
	uint8_t hw_addr[6];
	in_addr ipv4_addr, ipv4_netmask;
	uint8_t needs_work;
	void (*do_work)(struct net_dev *dev);
	int (*tx_packet)(struct net_dev *dev, uint8_t *packet, uint16_t len);
	void *priv;
	uint32_t rx_packets;
	uint32_t tx_packets;
} net_dev;

net_dev *alloc_netdev(uint32_t n);
void	init_net();
void	net_process();
void	net_print_stats();
void	net_ifconfig(uint16_t dev_no, in_addr ipv4_addr, in_addr netmask);
uint32_t htonl(uint32_t hl);
uint32_t ntohl(uint32_t nl);
uint16_t htons(uint16_t hs);
uint16_t ntohs(uint16_t ns);

void *net_alloc();
void net_free(void *buff);
#endif
