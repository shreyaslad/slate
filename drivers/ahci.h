#ifndef __DRIVERS__AHCI_H__
#define __DRIVERS__AHCI_H__

#include <trace.h>
#include <drivers/pci.h>

void init_ahci(struct pci_dev_t* device);

#endif
