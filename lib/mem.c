#include <lib/mem.h>

void* memset(void* bufptr, int value, size_t size) {
    unsigned char* buf = (unsigned char*)bufptr;
    for (uint64_t i = 0; i < size; i++) {
        buf[i] = value;
    }

    return bufptr;
}

void* memcpy(void* dest, const void* src, size_t size) {
    const unsigned char* src2 = (const unsigned char*)src;
    unsigned char* dst = (unsigned char*)dest;
    for (uint64_t i = 0; i < size; i++) {
        dst[i] = src2[i];
    }
    return dst;
}

int memcmp(const void* s1, const void* s2, size_t n) {
    const uint8_t* p1 = s1;
    const uint8_t* p2 = s2;

    for (size_t i = 0; i < n; i++) {
    if (p1[i] != p2[i])
        return p1[i] < p2[i] ? -1 : 1;
    }

    return 0;
}

void init_mem(struct stivale_info_t* info) {
	printf(KPRN_INFO, "entry: Bootstrapping Memory:\n");

	struct mmap_entry_t* entry = (struct mmap_entry_t*)info->memory_map_addr;

	for (uint64_t i = 0; i < info->memory_map_entries; i++)
		totalmem += entry[i].len;

	for (uint64_t i = 0; i < totalmem / PAGESIZE; i += PAGESIZE) {
		vmm_map((uint64_t *)(i + HIGH_VMA), (uint64_t *)i, get_pml4(), TABLEPRESENT | TABLEWRITE);
		vmm_map((uint64_t *)(i + KERNEL_HIGH_VMA), (uint64_t *)i, get_pml4(), TABLEPRESENT | TABLEWRITE);
		vmm_map((uint64_t *)i, (uint64_t *)i, get_pml4(), TABLEPRESENT | TABLEWRITE);
	}

	memset(pmm_bitmap, 0, totalmem / PAGESIZE / 8);

	for (uint64_t i = 0; i < info->memory_map_entries; i++) {
		printf(KPRN_INFO, "entry:\t%#016x - %#016x: ", entry[i].addr, entry[i].addr + entry[i].len);

		switch (entry[i].type) {
			case STIVALE_MEMORY_AVAILABLE:
				printf(KPRN_NONE, "Usable\n");
				break;
			case STIVALE_MEMORY_RESERVED:
				printf(KPRN_NONE, "Reserved\n");
				break;
			case STIVALE_MEMORY_ACPI_RECLAIMABLE:
				printf(KPRN_NONE, "ACPI Reclaimable\n");
				break;
			case STIVALE_MEMORY_NVS:
				printf(KPRN_NONE, "NVS\n");
				break;
			case STIVALE_MEMORY_BADRAM:
				printf(KPRN_NONE, "Bad RAM\n");
				break;
			case STIVALE_MEMORY_KRNL_N_MODS:
				printf(KPRN_NONE, "Kernel/Modules\n");
				break;
		}

		if (entry[i].type != STIVALE_MEMORY_AVAILABLE) {
			set_abs_bit(pmm_bitmap, entry[i].addr / PAGESIZE);
		}
	}

	printf(KPRN_INFO, "entry: Available Memory: %uGiB\n", totalmem / 1073741824);

	return;
}

