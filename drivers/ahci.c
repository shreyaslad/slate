#include <drivers/ahci.h>

#define FIS_TYPE_REG_H2D    0x27
#define FIS_TYPE_REG_D2H    0x34
#define FIS_TYPE_DMA_ACT    0x39
#define FIS_TYPE_DMA_SETUP  0x41
#define FIS_TYPE_DATA       0x46
#define FIS_TYPE_BIST       0x58
#define FIS_TYPE_PIO_SETUP  0x5f
#define FIS_TYPE_DEV_BITS   0xa1

struct ahci_port_t {
	uint32_t command_list_base_lo;
	uint32_t command_list_base_hi;
	uint32_t fis_base_addr_lo;
	uint32_t fis_base_addr_hi;
	uint32_t int_stat;
	uint32_t int_enable;
	uint32_t cmd;
	uint32_t reserved0;
	uint32_t task_file;
	uint32_t sig;
	uint32_t sata_stat;
	uint32_t sata_ctrl;
	uint32_t sata_err;
	uint32_t sata_active;
	uint32_t cmd_issue;
	uint32_t sata_notif;
	uint32_t fis_switch_ctrl;
	uint32_t reserved1[11];
	uint32_t vendor[4];
} __attribute__((packed));

struct ahci_mem_t {
	uint32_t capabilities;
	uint32_t global_host_ctrl;
	uint32_t int_stat;
	uint32_t port_impld;
	uint32_t version;
	uint32_t ccc_ctrl;
	uint32_t ccc_ports;
	uint32_t em_loc;
	uint32_t em_ctrl;
	uint32_t ext_capabilities;
	uint32_t bohc;
	uint8_t rsv[0xa0 - 0x2c];
	uint8_t vendor[0x100 - 0xa0];
	struct ahci_port_t ports[];
} __attribute__((packed));

void init_ahci(struct pci_dev_t* device) {
	printf(KPRN_INFO, "achi: Found AHCI controller at %u:%u.%u\n",
					device->bus,
					device->device,
					device->function);

	
}
