#include <net.h>
#include <memory.h>
#include <string.h>
#include <stdio.h>

#define NET_NUM_DEVS 16
net_dev net_devs[NET_NUM_DEVS];
uint16_t net_num_devs=0;
void *buffers;

void init_net() {
}

void *net_alloc() {
	if(!buffers) {
		buffers=alloc(32*2000,1);
		if(buffers) {
			memset(buffers,0,32*2000);
			uint16_t i;
			for(i=0;i<31*2000;i+=2000)
			*(void **)(buffers+i)=buffers+i+2000;
		} else {
			return 0;
		}
	}
	void *ret=buffers;
	buffers=*(void **)(ret);
	return ret;
}

void net_free(void *buff) {
	memset(buff,0,2000);
	*(void **)buff=buffers;
	buffers=buff;
}

net_dev *alloc_netdev(uint32_t n) {
	if (NET_NUM_DEVS==net_num_devs) return 0;
	net_dev *dev;
	void *priv=alloc(n,1);
	if(!priv) return 0;
	uint16_t dev_no = net_num_devs++;
	dev=&net_devs[dev_no];
	memset(dev,0,sizeof(net_dev));
	dev->priv = priv;
	dev->dev_no = dev_no;
	return dev;
}

void net_ifconfig(uint16_t dev_no, in_addr ipv4_addr, in_addr netmask) {
	if (net_num_devs>dev_no) {
		net_devs[dev_no].ipv4_addr.s_addr=ipv4_addr.s_addr;
		net_devs[dev_no].ipv4_netmask.s_addr=netmask.s_addr;
	}
}

void net_process() {
	uint16_t i;
	for(i=0;i<net_num_devs;++i) {
		if(net_devs[i].needs_work) {
			net_devs[i].do_work(&net_devs[i]);
		}
	}
}

void net_print_stats() {
	uint16_t i;
	for(i=0;i<net_num_devs;++i) {
		printf("%d rx:%ld tx:%ld\n",i,net_devs[i].rx_packets,net_devs[i].tx_packets);
	}
}

uint32_t htonl(uint32_t hl)
{
	register uint32_t ret;
	__asm__ ("bswap %%eax":"=a"(ret):"a"(hl));
	return ret;
}
uint32_t ntohl(uint32_t nl)
{
	return htonl(nl);
}
uint16_t htons(uint16_t hs)
{
	register uint16_t ret;
	__asm__ ("xchg %%al,%%ah":"=a"(ret):"a"(hs));
	return ret;
}
uint16_t ntohs(uint16_t ns) 
{
	return htons(ns);
}
