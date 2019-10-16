#include <icmp.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <net.h>
#include <ipv4.h>

void dump(void*,size_t);
void icmp_process_packet(net_dev *dev, ether_frame *frame, ip_packet *ip, icmp_packet *packet, size_t len)
{
if(packet->type==8) {
ether_frame *new_frame=net_alloc();
if(new_frame) {
memcpy(new_frame->dest,frame->src,6);
memcpy(new_frame->src,frame->dest,6);
new_frame->type=htons(0x0800);
ip_packet *new_ip=(ip_packet *)new_frame->payload;
new_ip->ihl=5; new_ip->version=4;
new_ip->tot_len=htons(20+len); new_ip->id=ip->id;
new_ip->ttl=64; new_ip->protocol=1;
new_ip->src=ip->dest; new_ip->dest=ip->src;
new_ip->check=ip_cksum(0,new_ip,20);
icmp_packet *new_packet=(icmp_packet *)new_ip->payload;
memcpy(new_packet,packet,len);
new_packet->cksum=0; new_packet->type=0;
new_packet->cksum=ip_cksum(0,new_packet,len);
ether_sendframe(dev,new_frame,len+20+14);
}
}
}
