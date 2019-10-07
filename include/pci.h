#ifndef _PCI_H
#define _PCI_H
#include <stdint.h>

typedef struct {
	uint8_t bus, device, function;
	uint16_t vendorId, deviceId;
} pci_dev;

void pci_adjust(pci_dev *dev);
#endif
