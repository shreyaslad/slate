#include <acpi/madt.h>

struct madt_t* madt;

struct madt_lapic_t** lapics;
int lapic_cnt;

struct madt_ioapic_t** ioapics;
int ioapic_cnt;

struct madt_iso_t** isos;
int iso_cnt;

struct madt_nmi_t** nmis;
int nmi_cnt;

void init_madt() {
	if ((madt = find_sdt("APIC", 0))) {
		serial_printf(KPRN_INFO, "ACPI", "Attempting to parse MADT\n");

		lapics = kmalloc(ACPI_MAX_TBL_CNT);
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

	init_apic();
}
