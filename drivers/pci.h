#ifndef __DRIVERS__PCI_H__
#define __DRIVERS__PCI_H__

#include <stdint.h>
#include <alloc.h>
#include <locks.h>
#include <mem.h>
#include <io.h>
#include <vec.h>
#include <sys/ports.h>
#include <acpi/acpi.h>

struct pci_dev_t {
	uint8_t bus;
	uint8_t device;
	uint8_t function;
} __attribute__((packed));

struct pci_id_t {
	uint8_t class;
	uint8_t subclass;
	uint8_t prog_if;
	uint16_t dev_id;
	uint16_t ven_id;
} __attribute((packed));

void init_pci();

#endif
