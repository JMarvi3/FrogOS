#include <stdint.h>
#include <stdio.h>
#include <port.h>
#include <pci.h>
#include <memory.h>
#include <string.h>
#include <system.h>

typedef struct {
	pci_dev *pcidev;
	uint16_t ioaddr;
	uint8_t interrupt;
	uint8_t hw_addr[6];
	uint16_t rx_buffer_ptr, tx_buffer_ptr;
	uint16_t  rx_buffer_count, tx_buffer_count;
	uint32_t rx_buffers, tx_buffers;
	uint8_t rdes[16], tdes[16];
} pcnet32_dev;

typedef struct __attribute__ ((__packed__))  {
	uint16_t mode;
	uint8_t rlen;
	uint8_t tlen;
	uint8_t hw_addr[6];
	uint16_t reserved;
	uint8_t ladr[8];
	uint32_t rde;
	uint32_t tde;
} pcnet32_card_reg;

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


void print_hwaddr(uint8_t *hw_addr) {
	printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
		hw_addr[0], hw_addr[1], hw_addr[2],
		hw_addr[3], hw_addr[4], hw_addr[5]);
}

unsigned long long pit_counter;
void pcnet32_reset(uint16_t ioaddr)
{
	inportw(ioaddr+PCNET32_DWIO_RESET);
	inportw(ioaddr+PCNET32_WIO_RESET);
	__asm__ ("mov %0, %%eax; 1: cmp %%eax, %0; je 1b"::"m"(pit_counter):"%eax","cc");
	outportl(ioaddr+0x10,0);
}

uint8_t pcnet32_driverowns(uint8_t *des, uint16_t idx)
{
	return (des[16*idx+7]&0-0x80)==0;
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
	*(uint32_t*)&des[idx*16] =(uint32_t) get_physaddr((void*)(buf_addr+idx*PCNET32_BUFFER_SIZE));
	memset(&des[idx*16], 0, 16);
	uint16_t bcnt = ((uint16_t)(-PCNET32_BUFFER_SIZE))&0x0fff|0xf000;
	*(uint16_t *)&des[idx*16+4]=bcnt;
	if(!is_tx) des[idx*16+7]=0x80;
}


void probe_pcnet32(pci_dev *pcidev, uint16_t ioaddr, uint8_t interrupt)
{
	printf("%d\n",sizeof(pcnet32_card_reg));
	printf("PCNET32 (%x,%x,%x):\n",pcidev->bus,pcidev->device,pcidev->function);
	pcnet32_reset(ioaddr);
	uint32_t chip_version=dwio_read_csr(ioaddr,88) | dwio_read_csr(ioaddr,89)<<16;
	printf("version: %08lx\n",chip_version);
	if (chip_version != 0x2621003) return;
	pcnet32_dev *dev=alloc(sizeof(pcnet32_dev),1);
	dev->pcidev=pcidev; dev->ioaddr=ioaddr; dev->interrupt=interrupt;
	pci_adjust(pcidev);
	uint16_t i;
	for(i=0;i<6;++i) (dev->hw_addr)[i]=inportb(ioaddr+i);

	//set SWSTYLE to 2
	uint32_t csr58 = dwio_read_csr(ioaddr,58);
	csr58 = csr58&0xfffffff0 | 0x2;
	dwio_write_csr(ioaddr,58,csr58);

	uint32_t bcr20 = dwio_read_bcr(ioaddr,20);
	bcr20 = bcr20&0xffffff00 | 0x82;
	dwio_write_bcr(ioaddr,20,bcr20);

	dev->rx_buffer_count=64;
	dev->rx_buffer_ptr=0;
	dev->rx_buffers=(uint32_t)alloc(dev->rx_buffer_count*PCNET32_BUFFER_SIZE,1);
	dev->tx_buffer_count=32;
	dev->tx_buffer_ptr=0;
	dev->tx_buffers=(uint32_t)alloc(dev->tx_buffer_count*PCNET32_BUFFER_SIZE,1);
	for(i=0;i<dev->rx_buffer_count;++i) pcnet32_initialize_de(dev,i,0);
	for(i=0;i<dev->tx_buffer_count;++i) pcnet32_initialize_de(dev,i,1);
	pcnet32_card_reg *card_reg=alloc(sizeof(pcnet32_card_reg),4);
	if(card_reg) {
	 card_reg->mode=0; card_reg->rlen=6<<4; card_reg->tlen=5<<4;
	 memcpy(card_reg->hw_addr,dev->hw_addr,6);
	 card_reg->reserved=0;
	 memset(card_reg->ladr,0,8);
	 card_reg->rde=(uint32_t)get_physaddr(dev->rdes);
	 card_reg->tde=(uint32_t)get_physaddr(dev->tdes);
        }
	printf("pcnet32 done.\n");
}
