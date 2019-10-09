#include <net.h>
#include <ether.h>
#include <stdio.h>
#include <crc32.h>
#include <stddef.h>

void dump(void *,size_t);
void ether_process_frame(net_dev *dev, ether_frame *frame, uint16_t len, uint8_t broadcast)
{
uint16_t type=ntohs(frame->type);
if(type<1536) return;
switch(type) {
	case 0x0800: //IPv4
		break;
	case 0x0806: //ARP
		printf("ARP: %d ",len);
		print_hwaddr(frame->src);
		printf(" -> ");
		print_hwaddr(frame->dest);
		printf("\n");
		dump((void *)frame+14,28);// len-14-4
		printf("\n");
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
