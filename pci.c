#include <stdint.h>
#include <stdio.h>
#include <port.h>
#include <pci.h>

pci_dev pcidevs[20];
uint16_t numdevs=0;

uint16_t pciConfigReadWord (uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t address;
    uint32_t lbus  = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    uint16_t tmp = 0;
 
    /* create configuration address as per Figure 1 */
    address = (uint32_t)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
 
    /* write out the address */
    outportl(0xCF8, address);
    /* read in the data */
    /* (offset & 2) * 8) = 0 will choose the first word of the 32 bits register */
    tmp = (uint16_t)((inportl(0xCFC) >> ((offset & 2) * 8)) & 0xffff);
    return (tmp);
}

uint32_t pciConfigReadDword(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t address = (uint32_t)((bus<<16)|(slot<<11)|(func<<8)|
	(offset&0xfc)|((uint32_t)0x80000000));
    outportl(0xCF8, address);
    return inportl(0xCFC);
}

void pciConfigWriteDword(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint32_t conf) {
    uint32_t address = (uint32_t)((bus<<16)|(slot<<11)|(func<<8)|
	(offset&0xfc)|((uint32_t)0x80000000));
    outportl(0xCF8, address);
    outportl(0xCFC, conf);
}

void pci_adjust(pci_dev *dev)
{
	uint32_t conf=pciConfigReadDword(dev->bus,dev->device,dev->function,0x4);
	conf |= 7;
	pciConfigWriteDword(dev->bus,dev->device,dev->function,0x4,conf);
}

uint16_t getVendorId(uint8_t bus, uint8_t slot, uint8_t function)
{
	return pciConfigReadWord(bus,slot,function,0);
}

uint16_t getDeviceId(uint8_t bus, uint8_t slot, uint8_t function)
{
	return pciConfigReadWord(bus,slot,function,2);
}

uint8_t getHeaderType(uint8_t bus, uint8_t slot, uint8_t function)
{
	return pciConfigReadWord(bus,slot,function,0xE) & 0xFF;
}

uint8_t getBaseClass(uint8_t bus, uint8_t slot, uint8_t function)
{
	return pciConfigReadWord(bus,slot,function,0xA) >> 8;
}

uint8_t getSubClass(uint8_t bus, uint8_t slot, uint8_t function)
{
	return pciConfigReadWord(bus,slot,function,0xA) & 0xff;
}

uint8_t getSecondaryBus(uint8_t bus, uint8_t slot, uint8_t function)
{
	return pciConfigReadWord(bus,slot,function,0x18) >> 8;
}

uint8_t getProgIf(uint8_t bus, uint8_t slot, uint8_t function)
{
	return pciConfigReadWord(bus,slot,function,0x8) >> 8;
}

void checkFunction(uint8_t bus, uint8_t device, uint8_t function);

void checkDevice(uint8_t bus, uint8_t device)
{
	uint8_t function = 0;
	uint16_t vendorId = getVendorId(bus,device,function);
	if(vendorId==0xFFFF) return;
	checkFunction(bus,device,function);
	uint16_t headerType = getHeaderType(bus,device,function);
	if (headerType & 0x80) {
	  for(function=1;function<8;++function) {
	    if(getVendorId(bus,device,function) != 0xFFFF)
	       checkFunction(bus,device,function);
	  }
	}
	
}

void checkBus(uint8_t bus)
{
	uint8_t device;
	for(device=0;device<32;++device)
		checkDevice(bus,device);
}

void probe_pcnet32(pci_dev *dev,uint16_t ioaddr, uint8_t interrupt);
void printFunction(uint8_t bus, uint8_t device, uint8_t function)
{
	uint16_t vendorId=getVendorId(bus,device,function);
	uint16_t deviceId=getDeviceId(bus,device,function);
	uint8_t  baseClass=getBaseClass(bus,device,function);
	uint8_t  subClass=getSubClass(bus,device,function);
	uint8_t	 progIf=getProgIf(bus,device,function);
	uint16_t subsystemVendorId=pciConfigReadWord(bus,device,function,0x2C);
	uint16_t subsystemId=pciConfigReadWord(bus,device,function,0x2E);
	uint16_t interrupt=pciConfigReadWord(bus,device,function,0x3C);
	printf("PCI: (%d,%d,%d) %x:%x %x:%x %x-%x-%x %04x\n",
		bus,device,function,vendorId,deviceId,
		subsystemVendorId,subsystemId,
		(int)baseClass,(int)subClass,(int)progIf, interrupt);
}

void checkFunction(uint8_t bus, uint8_t device, uint8_t function)
{
	uint8_t baseClass, subClass, secondaryBus;
	baseClass = getBaseClass(bus,device,function);
	subClass = getSubClass(bus,device,function);
	uint16_t vendorId, deviceId;
	vendorId=getVendorId(bus,device,function);
	deviceId=getDeviceId(bus,device,function);
	pcidevs[numdevs].bus=bus; pcidevs[numdevs].device=device;
	pcidevs[numdevs].function=function; pcidevs[numdevs].vendorId=vendorId;
	pcidevs[numdevs].deviceId=deviceId;
	if(vendorId==0x8086 && baseClass==2) 
		printFunction(bus,device,function);
	if(vendorId==0x1022 && (deviceId==0x2000||deviceId==0x2001)) {
		printFunction(bus,device,function);
	  uint8_t interrupt=pciConfigReadWord(bus,device,function,0x3C)&0xff;
	  uint32_t ioaddr=pciConfigReadWord(bus,device,function,0x10)&0xFFFC;
		probe_pcnet32(&pcidevs[numdevs],ioaddr,interrupt);
	}
	++numdevs;
	//printFunction(bus,device,function);
	if((baseClass==0x06) && (subClass==0x04)) {
		secondaryBus = getSecondaryBus(bus,device,function);
		checkBus(secondaryBus);
	}
}

void checkAllBuses(void)
{
	uint8_t function, bus;
	if((getHeaderType(0,0,0)&0x80)==0) {
		checkBus(0);
	} else {
		for(function=0;function>8;++function) {
			if(getVendorId(0,0,function)!=0xFFFF) break;
			checkBus(function);
		}
	}
}

void pci_init()
{
	checkAllBuses();
}
