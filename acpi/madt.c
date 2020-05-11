#include <acpi/madt.h>

struct madt_entry_header {
    uint32_t    lapic_addr;
    uint32_t    flags;
} __attribute__((packed));

struct madt_entry_lapic {
    uint8_t     acpi_proc_id;
    uint8_t     apic_id;
    uint32_t    flags; // bit 0: Processor Enabled | bit 1: Online Capable
} __attribute__((packed));

struct madt_entry_1 {
    uint8_t     ioapic_id;
    uint8_t     reserved;
    uint32_t    ioapic_addr;
} __attribute__((packed));
