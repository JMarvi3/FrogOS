#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <ipv4.h>

const char *inet_ntoa(in_addr in_addr)
{
	static char buff[16];
	const uint8_t *addr=(const uint8_t *)&in_addr.s_addr;
	sprintf(buff,"%d.%d.%d.%d",addr[0],addr[1],addr[2],addr[3]);
	return buff;
}

uint16_t ip_cksum(uint16_t prev, void *buff, size_t len)
{
	uint32_t sum=prev;
	while(len>=2) {
		sum+=*(uint16_t *)buff;
		buff+=2; len-=2;
	}
	if(len) sum+=*(uint8_t *)buff;
	while(sum>>16) {
		sum=(sum&0xffff)+(sum>>16);
	}
	return (~sum)&0xffff;
}

void dump(void *,size_t);

void ipv4_process_packet(net_dev *dev, ether_frame *frame, ip_packet *packet, size_t len)
{
if(packet->version!=4 || packet->ihl<5 || packet->ihl*4>len || ip_cksum(0,packet,4*packet->ihl)!=0) return;
switch(packet->protocol) {
	case 1: //ICMP
		
		break;
	default:
		break;
}
dump(packet,len);
}
