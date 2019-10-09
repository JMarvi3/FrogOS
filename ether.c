#include <net.h>
#include <ether.h>
#include <stdio.h>
#include <crc32.h>

void ether_process_frame(net_dev *dev, ether_frame *frame, uint16_t len, uint8_t broadcast)
{
uint16_t type=ntohs(frame->type);
if(type<1536) return;
switch(type) {
	case 0x0800: //IPv4
		break;
	case 0x0806: //ARP
		break;
	case 0x0842: //Wake-On-Lan
		printf("(WOL:%d,",broadcast);
		print_hwaddr(frame->src);
		printf(",");
		print_hwaddr(frame->dest);
		printf(")\n");
		break;
	case 0x86dd: //IPv6
		break;
	default:
		break;
}

}
