#include <net.h>
#include <ether.h>
#include <stdio.h>
#include <stddef.h>
#include <arp.h>

void dump(void *,size_t);

const char *eth_ntoa(const void *hw_addr)
{
	static char buff[18];
	const unsigned char *addr=hw_addr;
	sprintf(buff, "%02x:%02x:%02x:%02x:%02x:%02x",
		addr[0], addr[1], addr[2],
		addr[3], addr[4], addr[5]);
	return buff;
	
}

void ether_process_frame(net_dev *dev, ether_frame *frame, uint16_t len)
{
uint16_t type=ntohs(frame->type);
if(type<1536) return;
switch(type) {
	case 0x0800: //IPv4
		printf("IPv4: %d %s ",len,eth_ntoa(frame->src));
		printf("-> %s\n",eth_ntoa(frame->dest));
		dump(frame->payload,len-14-4);
		break;
	case 0x0806: //ARP
		printf("ARP: %d %s ",len,eth_ntoa(frame->src));
		printf("-> %s\n",eth_ntoa(frame->dest));
		arp_process_packet(dev,frame->src,frame->dest,(arp_packet *)frame->payload);
		break;
	case 0x0842: //Wake-On-Lan
		printf("WOL: %d %s ",len,eth_ntoa(frame->src));
		printf("-> %s\n",eth_ntoa(frame->dest));
		break;
	case 0x86dd: //IPv6
		break;
	default:
		break;
}

}

int ether_sendframe(net_dev *dev, void *frame, size_t len)
{
	return dev->tx_packet(dev,frame,len);
}
