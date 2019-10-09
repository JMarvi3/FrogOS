#ifndef _ETHER_H
#define _ETHER_H

#include <stdint.h>
#include <net.h>

typedef struct __attribute__ ((packed)) {
	uint8_t dest[6];
	uint8_t src[6];
	uint16_t type;
	uint8_t	payload[1];
} ether_frame;


void ether_process_frame(net_dev *dev, ether_frame *frame, uint16_t len);
const char *eth_ntoa(const void *hw_addr);

#endif
