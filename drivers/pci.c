#include <drivers/pci.h>

static struct vector_t* devices;
static struct vector_t* handlers;

uint32_t pci_pio_read_dword(uint8_t bus, uint8_t device, uint8_t function, uint8_t reg) {
	uint32_t pci_config_address = (1 << 31) 					| 
						((uint32_t)bus << 16) 					| 
						(((uint32_t)device & 0b11111) << 11) 	| 
						(((uint32_t)function & 0b111) << 8) 	| 
						(reg & ~(0b11));

	outd(0xcf8, pci_config_address);
	return ind(0xcfc);
}

void pci_pio_write_dword(uint8_t bus, uint8_t device, uint8_t function, uint8_t reg, uint32_t data) {
	uint32_t pci_config_address = (1 << 31) 					| 
						((uint32_t)bus << 16) 					| 
						(((uint32_t)device & 0b11111) << 11) 	| 
						(((uint32_t)function & 0b111) << 8) 	| 
						(reg & ~(0b11));

	outd(0xcf8, pci_config_address);
	outd(0xcfc, data);
}

static uint8_t pci_get_header_type(uint8_t bus, uint8_t device, uint8_t function) {
	uint8_t header_type = (uint8_t)(pci_pio_read_dword(bus, device, 0, 0xc) >> 16);

	return header_type & ~(1 << 7);
}

static uint8_t pci_get_class(uint8_t bus, uint8_t device, uint8_t function) {
	uint8_t class = (uint8_t)(pci_pio_read_dword(bus, device, function, 0x8) >> 24);

	return class;
}

static uint8_t pci_get_subclass(uint8_t bus, uint8_t device, uint8_t function) {
	uint8_t subclass = (uint8_t)(pci_pio_read_dword(bus, device, function, 0x8) >> 16);

	return subclass;
}

static uint8_t pci_get_prog_if(uint8_t bus, uint8_t device, uint8_t function) {
	return (uint8_t)(pci_pio_read_dword(bus, device, function, 0x8) >> 8);
}

static uint16_t pci_get_vendor_id(uint8_t bus, uint8_t device, uint8_t function) {
	return (uint16_t)pci_pio_read_dword(bus, device, function, 0);
}

static uint16_t pci_get_dev_id(uint8_t bus, uint8_t device, uint8_t function) {
	return (uint16_t)(pci_pio_read_dword(bus, device, function, 0) >> 16);
}

static uint8_t pci_check_function(uint8_t bus, uint8_t device, uint8_t function) {
	return (pci_get_vendor_id(bus, device, function) != 0xFFFF);
}

static uint8_t pci_get_secondary_bus(uint8_t bus, uint8_t device, uint8_t function) {
	return (uint8_t)(pci_pio_read_dword(bus, device, function, 0x18) >> 8);
}

static void pci_get_ids(struct pci_id_t* ret, struct pci_dev_t* target) {
	uint8_t bus = target->bus;
	uint8_t device = target->device;
	uint8_t function = target->function;

	ret->class = pci_get_class(bus, device, function);
	ret->subclass = pci_get_subclass(bus, device, function);
	ret->prog_if = pci_get_prog_if(bus, device, function);
	ret->dev_id = pci_get_dev_id(bus, device, function);
	ret->ven_id = pci_get_vendor_id(bus, device, function);
}

static char* pci_get_dev_type(uint8_t class, uint8_t subclass, uint8_t prog_if) {
	switch (class) {
		case 0: return "Undefined";
		case 1:
			switch (subclass) {
				case 0: return "SCSI Bus Controller";
				case 1: return "IDE Controller";
				case 2: return "Floppy Disk Controller";
				case 3: return "IPI Bus Controller";
				case 4: return "RAID Controller";
				case 5: return "ATA Controller";
				case 6:
					switch (prog_if) {
						case 0: return "Vendor specific SATA Controller";
						case 1: return "AHCI SATA Controller";
						case 2: return "Serial Storage Bus SATA Controller";
					}

					break;
				case 7: return "Serial Attached SCSI Controller";
				case 8:
					switch (prog_if) {
						case 1: return "NVMHCI Controller";
						case 2: return "NVMe Controller";
					}

					break;
		}

		return "Mass Storage Controller";

		case 2:
			switch (subclass) {
				case 0: return "Ethernet Controller";
				case 1: return "Token Ring Controller";
				case 2: return "FDDI Controller";
				case 3: return "ATM Controller";
				case 4: return "ISDN Controller";
				case 5: return "WorldFip Controller";
				case 6: return "PICMG 2.14 Controller";
				case 7: return "InfiniBand Controller";
				case 8: return "Fabric Controller";
			}

			return "Network Controller";
		case 3:
			switch (subclass) {
				case 0: return "VGA Compatible Controller";
				case 1: return "XGA Controller";
				case 2: return "3D Controller";
			}

			return "Display Controller";
		case 4: return "Multimedia controller";
		case 5: return "Memory Controller";
		case 6:
			switch (subclass) {
				case 0: return "Host Bridge";
				case 1: return "ISA Bridge";
				case 2: return "EISA Bridge";
				case 3: return "MCA Bridge";
				case 4: return "PCI-to-PCI Bridge";
				case 5: return "PCMCIA Bridge";
				case 6: return "NuBus Bridge";
				case 7: return "CardBus Bridge";
				case 8: return "RACEway Bridge";
				case 9: return "Semi-Transparent PCI-to-PCI Bridge";
				case 10: return "InfiniBand-to-PCI Host Bridge";
			}
			return "Bridge Device";
		case 8:
			switch (subclass) {
				case 0:
					switch (prog_if) {
						case 0: return "8259-Compatible PIC";
						case 1: return "ISA-Compatible PIC";
						case 2: return "EISA-Compatible PIC";
						case 0x10: return "I/O APIC IRQ Controller";
						case 0x20: return "I/O xAPIC IRQ Controller";
					}

					break;
				case 1:
					switch (prog_if) {
						case 0: return "8237-Compatible DMA Controller";
						case 1: return "ISA-Compatible DMA Controller";
						case 2: return "EISA-Compatible DMA Controller";
					}

					break;
				case 2:
					switch (prog_if) {
						case 0: return "8254-Compatible PIT";
						case 1: return "ISA-Compatible PIT";
						case 2: return "EISA-Compatible PIT";
						case 3: return "HPET";
					}

					break;
				case 3: return "Real Time Clock";
				case 4: return "PCI Hot-Plug Controller";
				case 5: return "SDHCI";
				case 6: return "IOMMU";
			}

			return "Base System Peripheral";
		case 0xC:
			switch (subclass) {
				case 0:
					switch (prog_if) {
						case 0: return "Generic FireWire (IEEE 1394) Controller";
						case 0x10: return "OHCI FireWire (IEEE 1394) Controller";
					}

					break;
				case 1: return "ACCESS Bus Controller";
				case 2: return "SSA Controller";
				case 3:
					switch (prog_if) {
						case 0: return "uHCI USB1 Controller";
						case 0x10: return "oHCI USB1 Controller";
						case 0x20: return "eHCI USB2 Controller";
						case 0x30: return "xHCI USB3 Controller";
						case 0xFE: return "USB Device";
					}
					break;
				case 4: return "Fibre Channel Controller";
				case 5: return "SMBus";
				case 6: return "InfiniBand Controller";
				case 7: return "IPMI Interface Controller";
			}

			return "Serial Bus Controller";
		default:
			return "Unknown";
			break;
	}
}

static uint8_t is_multifunction(uint8_t bus, uint8_t device) {
	uint8_t header_type = (uint8_t)(pci_pio_read_dword(bus, device, 0, 0xc) >> 16);

	return header_type & (1 << 7);
}

static uint8_t is_pci_bridge(uint8_t bus, uint8_t device, uint8_t function) {
	if (pci_get_header_type(bus, device, function) != 0x1)
		return 0;

	if (pci_get_class(bus, device, function) != 0x6)
		return 0;

	if (pci_get_subclass(bus, device, function) != 0x4)
		return 0;

	return 1;
}

static void pci_scan_bus(uint8_t bus) {
	serial_printf(KPRN_INFO, "PCI", "Scanning bus %u\n", bus);

	for (uint8_t device = 0; device < 32; device++) {
		if (pci_check_function(bus, device, 0)) {
			if (is_pci_bridge(bus, device, 0)) {
				pci_scan_bus(pci_get_secondary_bus(bus, device, 0));
			} else {
				struct pci_dev_t* dev = kmalloc(sizeof(struct pci_dev_t)) + HIGH_VMA;
				dev->bus = bus;
				dev->device = device;
				dev->function = 0;
				vec_a(devices, dev);
			}

			if (is_multifunction(bus, device)) {
				for (uint8_t function = 1; function < 8; function++) {
					if (pci_check_function(bus, device, function)) {
						if (is_pci_bridge(bus, device, function))
							pci_scan_bus(pci_get_secondary_bus(bus, device, function));
						else {
							struct pci_dev_t* dev = kmalloc(sizeof(struct pci_dev_t)) + HIGH_VMA;
							dev->bus = bus;
							dev->device = device;
							dev->function = function;
							vec_a(devices, dev);
						}
					}
				}
			}
		}
	}
}

void init_pci() {
	devices = kmalloc(sizeof(struct vector_t)) + HIGH_VMA;
	handlers = kmalloc(sizeof(struct vector_t)) + HIGH_VMA;
	vec_n(devices);

	pci_scan_bus(0);

	for (size_t i = 0; i < devices->n; i++) {
		struct pci_dev_t* cur_dev = devices->items[i];

		struct pci_id_t cur_id;
		pci_get_ids(&cur_id, cur_dev);

		serial_printf(KPRN_INFO, "PCI", "Found \"%s\" at %u:%u.%u\n", 
					pci_get_dev_type(cur_id.class, cur_id.subclass, cur_id.prog_if),
					cur_dev->bus, cur_dev->device, cur_dev->function);
	}
}
