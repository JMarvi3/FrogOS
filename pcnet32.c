#include <stdint.h>
#include <stdio.h>
#include <port.h>
#include <pci.h>
#include <memory.h>
#include <string.h>
#include <system.h>
#include <conio.h>
#include <string.h>
#include <net.h>
#include <ether.h>

typedef struct __attribute__ ((packed)){
	uint16_t mode;
	uint8_t rlen;
	uint8_t tlen;
	uint8_t hw_addr[6];
	uint16_t reserved;
	uint8_t ladr[8];
	uint32_t rde;
	uint32_t tde;
} pcnet32_card_reg;

typedef struct {
	pci_dev *pcidev;
	uint16_t ioaddr;
	uint8_t interrupt;
	irq_handler_t handler;
	pcnet32_card_reg card_reg __attribute__((aligned(32)));
	uint16_t rx_buffer_ptr, tx_buffer_ptr;
	uint16_t  rx_buffer_count, tx_buffer_count;
	uint32_t rx_buffers, tx_buffers;
	uint8_t *rdes, *tdes;
	uint8_t mii, fdx;
} pcnet32_dev;

#define PCNET32_WIO_RDP 0x10
#define PCNET32_WIO_RAP 0x12
#define PCNET32_WIO_RESET 0x14
#define PCNET32_WIO_BDP 0x16

#define PCNET32_DWIO_RDP 0x10
#define PCNET32_DWIO_RAP 0x14
#define PCNET32_DWIO_RESET 0x18
#define PCNET32_DWIO_BDP 0x1C

#define PCNET32_BUFFER_SIZE 1600
#define PCNET32_DE_SIZE	16

uint32_t dwio_read_csr(uint16_t ioaddr, uint16_t index)
{
	outportl(ioaddr+PCNET32_DWIO_RAP, index);
	return inportl(ioaddr+PCNET32_DWIO_RDP);
}

void dwio_write_csr(uint16_t ioaddr, uint16_t index, uint32_t conf)
{
	outportl(ioaddr+PCNET32_DWIO_RAP, index);
	outportl(ioaddr+PCNET32_DWIO_RDP, conf);
}

uint16_t wio_read_csr(uint16_t ioaddr, uint16_t index)
{
	outportw(ioaddr+PCNET32_WIO_RAP, index);
	return inportw(ioaddr+PCNET32_WIO_RDP);
}

void dwio_write_bcr(uint16_t ioaddr, uint16_t index, uint32_t conf)
{
	outportl(ioaddr+PCNET32_DWIO_RAP, index);
	outportl(ioaddr+PCNET32_DWIO_BDP, conf);
}

uint32_t dwio_read_bcr(uint16_t ioaddr, uint16_t index)
{
	outportl(ioaddr+PCNET32_DWIO_RAP, index);
	return inportl(ioaddr+PCNET32_DWIO_BDP);
}


extern unsigned long long pit_counter;
void pcnet32_reset(uint16_t ioaddr)
{
	inportw(ioaddr+PCNET32_DWIO_RESET);
	inportw(ioaddr+PCNET32_WIO_RESET);
	__asm__ ("mov %0, %%eax; 1: cmp %%eax, %0; je 1b"::"m"(pit_counter):"%eax","cc");
	outportl(ioaddr+0x10,0);
}

uint8_t pcnet32_driverowns(uint8_t *des, uint16_t idx)
{
	return (des[16*idx+7]&0x80)==0;
}

void pcnet32_inc_txidx(pcnet32_dev *dev) {
	if (++(dev->tx_buffer_ptr)==dev->tx_buffer_count)
		dev->tx_buffer_ptr=0;
}

void pcnet32_inc_rxidx(pcnet32_dev *dev) {
	if (++(dev->rx_buffer_ptr)==dev->rx_buffer_count)
		dev->rx_buffer_ptr=0;
}

void pcnet32_initialize_de(pcnet32_dev *dev, uint16_t idx, uint8_t is_tx)
{
	uint8_t *des; uint32_t buf_addr;
	if(is_tx) {
		des=dev->tdes; buf_addr=dev->tx_buffers;
	} else {
		des=dev->rdes; buf_addr=dev->rx_buffers;
	}
	memset(&des[idx*16], 0, 16);
	*(uint32_t*)&des[idx*16] =(uint32_t) get_physaddr((void*)(buf_addr+idx*PCNET32_BUFFER_SIZE));
	uint16_t bcnt = ((uint16_t)(-PCNET32_BUFFER_SIZE))&0x0fff|0xf000;
	*(uint16_t *)&des[idx*16+4]=bcnt;
	if(!is_tx) des[idx*16+7]=0x80;
}

extern unsigned long long pit_counter;
void pcnet32_irq_handler(struct regs *r, void *data) {
	net_dev *netdev=(net_dev *)data;
	pcnet32_dev *dev=(pcnet32_dev *)netdev->priv;;
	uint32_t csr0=dwio_read_csr(dev->ioaddr,0);
	if((csr0&0x600)!=0) {
		dwio_write_csr(dev->ioaddr,0,csr0|(1<<9)|(1<<10));
		netdev->needs_work=1;
	} else {
		printf("hmm... %lld csr0: %x\n",pit_counter,csr0);
	}
}

int pcnet32_tx_packet(net_dev *netdev, uint8_t *packet, uint16_t len)
{
	pcnet32_dev *dev=(pcnet32_dev *)netdev->priv;
	if(!pcnet32_driverowns(dev->tdes, dev->tx_buffer_ptr)) {
		return 0;
	}
	memcpy((void *)(dev->tx_buffers+dev->tx_buffer_ptr*PCNET32_BUFFER_SIZE),packet, len);
	net_free(packet);
	dev->tdes[dev->tx_buffer_ptr*16+7] |= 0x3;
	uint16_t bcnt = (uint16_t)(-len)&0xfff | 0xf000;
	*(uint16_t *)(dev->tdes+dev->tx_buffer_ptr*16+4) = bcnt;
	++netdev->tx_packets;
	dev->tdes[dev->tx_buffer_ptr*16+7] |= 0x80;
	if((++dev->tx_buffer_ptr)>=dev->tx_buffer_count)
		dev->tx_buffer_ptr=0;
	return 1;
}

void pcnet32_do_irq_work(net_dev *netdev)
{
	pcnet32_dev *dev=(pcnet32_dev *)netdev->priv;
	netdev->needs_work=0;
	uint16_t i, de_ptr=dev->rx_buffer_ptr;
	for(i=0;i<dev->rx_buffer_count;++i) {
	    if(!pcnet32_driverowns(dev->rdes, de_ptr))
		break;
	    uint32_t rx_buff=dev->rx_buffers+de_ptr*PCNET32_BUFFER_SIZE;
	    ++netdev->rx_packets;
	    uint16_t mcnt=dev->rdes[16*de_ptr+8]|dev->rdes[16*de_ptr+9]<<8;
	    ether_process_frame(netdev,(ether_frame *)rx_buff,mcnt);
	    memset((void *)rx_buff,0,mcnt);
	    dev->rdes[16*de_ptr+7]|=0x80;
	    ++de_ptr;
	    if(de_ptr>=dev->rx_buffer_count) de_ptr=0;
	}
	dev->rx_buffer_ptr=de_ptr;
}

void probe_pcnet32(pci_dev *pcidev, uint16_t ioaddr, uint8_t interrupt)
{
	printf("PCNET32: ");
	pcnet32_reset(ioaddr);
	uint32_t chip_version=dwio_read_csr(ioaddr,88) | dwio_read_csr(ioaddr,89)<<16;
	if((chip_version&0xfff) != 0x003) goto err;

	net_dev *netdev=alloc_netdev(sizeof(pcnet32_dev));
	if(!netdev) goto err;
	pcnet32_dev *dev=(pcnet32_dev *)netdev->priv;
	netdev->do_work=pcnet32_do_irq_work;
	netdev->tx_packet=pcnet32_tx_packet;
	
	dev->pcidev=pcidev; dev->ioaddr=ioaddr; dev->interrupt=interrupt;

	dev->mii = dev->fdx = 0;
	switch(chip_version>>12) {
		case 0x2621:
			dev->fdx=1;
			break;
		case 0x2625:
			dev->fdx=1;
			dev->mii=1;
			break;
		default:
			goto err;
	}
	pci_adjust(pcidev);
	uint16_t i;
	for(i=0;i<6;++i) (netdev->hw_addr)[i]=inportb(ioaddr+i);
	printf(eth_ntoa(netdev->hw_addr));

	//set SWSTYLE to 2
	uint32_t csr58 = dwio_read_csr(ioaddr,58);
	csr58 = csr58&0xfffffff0 | 0x2;
	dwio_write_csr(ioaddr,58,csr58);


#define PCNET32_LOG_RX_BUFF 6
#define PCNET32_LOG_TX_BUFF 4
	dev->rx_buffer_count=1<<PCNET32_LOG_RX_BUFF;
	dev->rx_buffer_ptr=0;
	dev->rx_buffers=(uint32_t)alloc(dev->rx_buffer_count*PCNET32_BUFFER_SIZE,1);
	if(!dev->rx_buffers) goto err;
	memset((void *)dev->rx_buffers,0,dev->rx_buffer_count*PCNET32_BUFFER_SIZE);
	dev->tx_buffer_count=1<<PCNET32_LOG_TX_BUFF;
	dev->tx_buffer_ptr=0;
	dev->tx_buffers=(uint32_t)alloc(dev->tx_buffer_count*PCNET32_BUFFER_SIZE,1);
	if(!dev->tx_buffers) goto err;
	dev->rdes=alloc(16*dev->rx_buffer_count,1);
	if(!dev->rdes) goto err;
	dev->tdes=alloc(16*dev->tx_buffer_count,1);
	if(!dev->tdes) goto err;
	for(i=0;i<dev->rx_buffer_count;++i) pcnet32_initialize_de(dev,i,0);
	for(i=0;i<dev->tx_buffer_count;++i) pcnet32_initialize_de(dev,i,1);
	dev->card_reg.mode=0; // promiscuous: (1<<15);
	dev->card_reg.rlen=PCNET32_LOG_RX_BUFF<<4;
	dev->card_reg.tlen=PCNET32_LOG_TX_BUFF<<4;
	memcpy(dev->card_reg.hw_addr,netdev->hw_addr,6);
	dev->card_reg.reserved=0;
	memset(dev->card_reg.ladr,0xff,8);
	dev->card_reg.rde=(uint32_t)get_physaddr(dev->rdes);
	dev->card_reg.tde=(uint32_t)get_physaddr(dev->tdes);

	uint32_t init_block=(uint32_t)get_physaddr(&dev->card_reg);
	dwio_write_csr(ioaddr,1,init_block&0xffff);
	dwio_write_csr(ioaddr,2,init_block>>16);
	dwio_write_csr(ioaddr,4,0x915);

	// Initialize the card
//	dwio_write_csr(ioaddr,0,dwio_read_csr(ioaddr,0)|1);
	dwio_write_csr(ioaddr,0,1);
	while(dwio_read_csr(ioaddr,0)&0x100==0);

	dwio_write_csr(ioaddr,0,(1<<2));
	dev->handler.handler = pcnet32_irq_handler;
	dev->handler.data=(void *)netdev;
	dev->handler.next=0;
	install_irq_handler(interrupt,&(dev->handler));
        
	// Set start and enable interrupts
	dwio_write_csr(ioaddr,0,(1<<1)|(1<<6));
	printf(" running");
#ifdef DEBUG
	printf("0x%x",dwio_read_csr(ioaddr,0));
#endif

	err:
	printf(" done.\n");
}
