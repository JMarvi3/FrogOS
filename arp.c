#include <net.h>
#include <ether.h>
#include <arp.h>
#include <ipv4.h>
#include <stdio.h>
#include <string.h>

void arp_process_packet(net_dev *dev, uint8_t *src, uint8_t *dest, arp_packet *packet)
{
if(ntohs(packet->htype)!=1||ntohs(packet->ptype)!=0x0800||packet->hlen!=6||packet->plen!=4) return;
printf(" .. %d %s %s -> ",ntohs(packet->oper),eth_ntoa(packet->sha),inet_ntoa(packet->spa));
printf("%s %s\n",eth_ntoa(packet->tha),inet_ntoa(packet->tpa));
if(ntohs(packet->oper)==1 && packet->tpa.s_addr==dev->ipv4_addr.s_addr) {
	ether_frame *frame=net_alloc();
	if(frame) {
		memcpy(frame->dest,src,6);
		memcpy(frame->src,dest,6);
		frame->type=htons(0x0806);
		arp_packet *arp=(arp_packet *)frame->payload;
		arp->htype=htons(1); arp->ptype=htons(0x0800);
		arp->hlen=6; arp->plen=4;
		arp->oper=htons(2);
		memcpy(arp->sha,dev->hw_addr,6);
		arp->spa.s_addr=packet->tpa.s_addr;
		memcpy(arp->tha,src,6);
		arp->tpa.s_addr=packet->spa.s_addr;
		ether_sendframe(dev,frame,14+sizeof(arp_packet));
	}	
}
}
