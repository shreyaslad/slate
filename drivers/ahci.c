#include <drivers/ahci.h>

#undef __MODULE__
#define __MODULE__ "ahci"

struct ahci_h2d_fis_t {
    uint8_t type;

    struct ahci_h2d_fis_flags {
        uint8_t pm_port: 4;
        uint8_t reserved0: 3;
        uint8_t c: 1;
    };

    uint8_t cmd;
    uint8_t feat_lo;
    uint8_t lba0;
    uint8_t lba1;
    uint8_t lba2;
    uint8_t dev_type;
    uint8_t lba3;
    uint8_t lba4;
    uint8_t lba5;
    uint8_t feat_hi;
    uint8_t count_lo;
    uint8_t count_hi;
    uint8_t icc;
    uint8_t ctrl;
    uint8_t reserved1[4];
} __attribute__((packed));

struct ahci_d2h_fis_t {
    uint8_t fis_type;

    struct ahci_d2h_fis_flags {
        uint8_t pm_port: 4;
        uint8_t reserved0: 2;
        uint8_t interrupt: 1;
        uint8_t reserved1: 1;
    };

    uint8_t stat;
    uint8_t err;
    uint8_t lba0;
    uint8_t lba1;
    uint8_t lba2;
    uint8_t device;
    uint8_t lba3;
    uint8_t lba4;
    uint8_t lba5;
    uint8_t reserved2;
    uint8_t count_lo;
    uint8_t count_hi;
    uint8_t reserved3[2];
    uint8_t reserved4[4];
} __attribute__((packed));

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

enum ahci_fis_t {
    FIS_TYPE_REG_H2D    = 0x27,
    FIS_TYPE_REG_D2H    = 0x34,
    FIS_TYPE_DMA_ACT    = 0x39,
    FIS_TYPE_DMA_SETUP  = 0x41,
    FIS_TYPE_DATA       = 0x46,
    FIS_TYPE_BIST       = 0x58,
    FIS_TYPE_PIO_SETUP  = 0x5f,
    FIS_TYPE_DEV_BITS   = 0xa1
};

enum sata_sig_t {
    ATA     = 0x00000101,   // SATA
    ATAPI   = 0xEB140101,   // SATAPI
    SEMB    = 0xC33C0101,   // Enclosure Management Bridge
    PM      = 0x96690101    // Port Multiplier
};

void init_ahci(struct pci_dev_t* device) {
    TRACE("Found AHCI controller at %u:%u.%u\n",
                    device->bus,
                    device->device,
                    device->function);
}
