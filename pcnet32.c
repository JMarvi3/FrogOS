#include <stdint.h>
#include <stdio.h>
#include <port.h>
#include <pci.h>

#define PCNET32_WIO_RDP 0x10
#define PCNET32_WIO_RAP 0x12
#define PCNET32_WIO_RESET 0x14
#define PCNET32_WIO_BDP 0x16

#define PCNET32_DWIO_RDP 0x10
#define PCNET32_DWIO_RAP 0x14
#define PCNET32_DWIO_RESET 0x18
#define PCNET32_DWIO_BDP 0x1C

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

void probe_pcnet32(pci_dev *dev, uint16_t ioaddr, uint8_t interrupt)
{
	printf("PCNET32 (%x,%x,%x):\n",dev->bus,dev->device,dev->function);
	pcnet32_reset(ioaddr);
	uint32_t chip_version=dwio_read_csr(ioaddr,88) | dwio_read_csr(ioaddr,89)<<16;
	printf("version: %08lx\n",chip_version);
	if (chip_version != 0x2621003) return;
	pci_adjust(dev);
	unsigned int i;
	uint8_t hw_addr[6];
	for(i=0;i<6;++i) hw_addr[i]=inportb(ioaddr+i);
	print_hwaddr(hw_addr);

	//set SWSTYLE to 2
	uint32_t csr58 = dwio_read_csr(ioaddr,58);
	csr58 = csr58&0xffff0 | 0x2;
	dwio_write_csr(ioaddr,58,csr58);
	uint32_t bcr18=dwio_read_bcr(ioaddr, 18);
	printf("bcr18: %lx\n",bcr18);
	printf("pcnet32 done.\n");
}
