#include <acpi/madt.h>

struct madt_t {
    struct sdt_t sdt;
    uint32_t local_controller_paddr;
    uint32_t flags;
    uint8_t madt_entries_begin;
} __attribute__((packed));

struct madt_header_t {
    uint8_t type;
    uint8_t len;
} __attribute__((packed));

struct madt__lapic_t {
    struct madt_header_t header;
    uint8_t acpi_proc_id;
    uint8_t apic_id;
    uint32_t flags; // bit 0: Processor Enabled | bit 1: Online Capable
} __attribute__((packed));

struct madt_ioapic_t {
    struct madt_header_t header;
    uint8_t ioapic_id;
    uint8_t reserved;
    uint32_t ioapic_addr;
} __attribute__((packed));

struct madt_iso {
    struct madt_header_t header;
    uint8_t bus_src;
    uint8_t irq_src;
    uint32_t gsi;
    uint16_t flags;
} __attribute__((packed));

struct madt_nmi {
    struct madt_header_t header;
    uint8_t acpi_proc_id;
    uint16_t flags;
    uint8_t lint;
} __attribute__((packed));

static struct madt_t* madt = NULL;

void init_madt() {
	if (madt = find_sdt("APIC", 0)) {
		;
	}
}
