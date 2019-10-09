#include <net.h>
#include <memory.h>
#include <string.h>
#include <stdio.h>
#define NET_NUM_DEVS 16
net_dev net_devs[NET_NUM_DEVS];
uint16_t net_num_devs=0;

void init_net() {
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

void net_ifconfig(uint16_t dev_no, uint8_t *ipv4_addr) {
	memcpy(net_devs[dev_no].ipv4_addr,ipv4_addr,4);
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
