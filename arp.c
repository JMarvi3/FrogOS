#include <net.h>
#include <ether.h>
#include <arp.h>
#include <ipv4.h>
#include <stdio.h>

void arp_process_packet(uint8_t *src, uint8_t *dest, arp_packet *packet)
{
if(ntohs(packet->htype)!=1||ntohs(packet->ptype)!=0x0800||packet->hlen!=6||packet->plen!=4) return;
printf(" .. %d %s %s -> ",ntohs(packet->oper),eth_ntoa(packet->sha),inet_ntoa(packet->spa));
printf("%s %s\n",eth_ntoa(packet->tha),inet_ntoa(packet->tpa));
}
