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

struct madt_lapic_t {
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

static struct madt_lapic_t** lapics;
static int lapic_cnt;

static struct madt_ioapic_t** ioapics;
static int ioapic_cnt;

static struct madt_iso_t** isos;
static int iso_cnt;

static struct madt_nmi_t** nmis;
static int nmi_cnt;

void init_madt() {
	if ((madt = find_sdt("APIC", 0))) {
		serial_printf(KPRN_INFO, "ACPI", "Attempting to parse MADT\n");

		lapics = kmalloc(ACPI_MAX_TBL_CNT) + HIGH_VMA;
		ioapics = kmalloc(ACPI_MAX_TBL_CNT) + HIGH_VMA;
		isos = kmalloc(ACPI_MAX_TBL_CNT) + HIGH_VMA;
		nmis = kmalloc(ACPI_MAX_TBL_CNT) + HIGH_VMA;

		for (uint8_t* madt_ptr = (uint8_t *)(&madt->madt_entries_begin);
        	(size_t)madt_ptr < (size_t)madt + madt->sdt.len;
        	madt_ptr += *(madt_ptr + 1)) {
				switch (*(madt_ptr)) {
					case 0:
						serial_printf(KPRN_INFO, "MADT", "Found LAPIC #%u\n", lapic_cnt);
						lapics[lapic_cnt++] = (struct madt_lapic_t *)madt_ptr;
						break;
					case 1:
						serial_printf(KPRN_INFO, "MADT", "Found IOAPIC #%u\n", ioapic_cnt);
						ioapics[ioapic_cnt++] = (struct madt_ioapic_t *)madt_ptr;
						break;
					case 2:
						serial_printf(KPRN_INFO, "MADT", "Found ISO #%u\n", iso_cnt);
						isos[iso_cnt++] = (struct madt_iso_t *)madt_ptr;
						break;
					case 4:
						serial_printf(KPRN_INFO, "MADT", "Found NMI #%u\n", nmi_cnt);
						nmis[nmi_cnt++] = (struct madt_nmi_t *)madt_ptr;
						break;
					default:
						serial_printf(KPRN_WARN, "MADT", "Nothing found in current MADT entry\n");
						break;
				}
			}
	}
}
