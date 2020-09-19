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
        printf(KPRN_INFO, "madt: APIC Configuration:\n");

        lapics = kmalloc(ACPI_MAX_TBL_CNT);
        ioapics = kmalloc(ACPI_MAX_TBL_CNT) + HIGH_VMA;
        isos = kmalloc(ACPI_MAX_TBL_CNT) + HIGH_VMA;
        nmis = kmalloc(ACPI_MAX_TBL_CNT) + HIGH_VMA;

        for (uint8_t* madt_ptr = (uint8_t *)(&madt->madt_entries_begin);
            (size_t)madt_ptr < (size_t)madt + madt->sdt.len;
            madt_ptr += *(madt_ptr + 1)) {
                switch (*(madt_ptr)) {
                    case 0:
                        printf(KPRN_INFO, "madt:\tLAPIC #%u\n", lapic_cnt);
                        lapics[lapic_cnt++] = (struct madt_lapic_t *)madt_ptr;
                        break;
                    case 1:
                        printf(KPRN_INFO, "madt:\tIOAPIC #%u\n", ioapic_cnt);
                        ioapics[ioapic_cnt++] = (struct madt_ioapic_t *)madt_ptr;
                        break;
                    case 2:
                        printf(KPRN_INFO, "madt:\tISO #%u\n", iso_cnt);
                        isos[iso_cnt++] = (struct madt_iso_t *)madt_ptr;
                        break;
                    case 4:
                        printf(KPRN_INFO, "madt:\tNMI #%u\n", nmi_cnt);
                        nmis[nmi_cnt++] = (struct madt_nmi_t *)madt_ptr;
                        break;
                    default:
                        printf(KPRN_WARN, "madt:\tNothing found\n");
                        break;
                }
            }
    }
}
