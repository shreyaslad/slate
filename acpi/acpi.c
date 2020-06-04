#include <acpi/acpi.h>

static struct rsdp_t* rsdp = NULL;
static struct rsdt_t* rsdt = NULL;
static struct xsdt_t* xsdt = NULL;

void* find_sdt(const char* signature, int idx) {
	int cnt = 0;

	if (xsdt != NULL) {
		for (size_t i = 0; i < (xsdt->sdt.len - sizeof(struct sdt_t)) / 8; i++) {
			struct sdt_t* ptr = (struct sdt_t *)((size_t)xsdt->sdt_ptr[i] + HIGH_VMA);

			if (!strncmp(ptr->signature, signature, 4)) {
				if (cnt++ == idx) {
					serial_printf(KPRN_INFO, "ACPI", "Found \"%s\" at %X\n", signature, (size_t)ptr);
					return (void *)ptr;
				}
			}
		}
	} else {
		for (size_t i = 0; i < (rsdt->sdt.len - sizeof(struct sdt_t)) / 4; i++) {
			struct sdt_t* ptr = (struct sdt_t *)((size_t)rsdt->sdt_ptr[i] + HIGH_VMA);

			if (!strncmp(ptr->signature, signature, 4)) {
				if (cnt++ == idx) {
					serial_printf(KPRN_INFO, "ACPI", "Found \"%s\" at %X\n", signature, (size_t)ptr);
					return (void *)ptr;
				}
			}
		}
	}

	serial_printf(KPRN_ERR, "ACPI", "Table with signature \"%s\" not found\n", signature);
	return NULL;
}

void init_acpi(uint64_t rsdp_addr) {
	if (rsdp_addr) {
		rsdp = (struct rsdp_t *)rsdp_addr;
		goto found;
	} else {
		serial_printf(KPRN_ERR, "ACPI", "Non-compliant system!\n");
	}

	return;

found:
	if (rsdp->rev >= 2 && rsdp->xsdt_paddr) {
        serial_printf(KPRN_INFO, "ACPI", "Found XSDT at %X\n", ((size_t)rsdp->xsdt_paddr + HIGH_VMA));
        xsdt = (struct xsdt_t *)((size_t)rsdp->xsdt_paddr + HIGH_VMA);
    } else {
        serial_printf(KPRN_INFO, "ACPI", "Found RSDT at %X\n", ((size_t)rsdp->rsdt_paddr + HIGH_VMA));
        rsdt = (struct rsdt_t *)((size_t)rsdp->rsdt_paddr + HIGH_VMA);
    }

	init_madt();

	serial_printf(KPRN_INFO, "ACPI", "Finished setting up ACPI\n");
}
