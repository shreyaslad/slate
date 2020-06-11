#include <lib.h>

void init_mem(struct stivale_info_t* info) {
    struct mmap_entry_t* entry = (struct mmap_entry_t*)info->memory_map_addr;

    for (uint64_t i = 0; i < info->memory_map_entries; i++) {
        totalmem += entry[i].len;
    }

    serial_printf(KPRN_INFO, "MEM", "Total Memory: %X\n", totalmem);

   for (uint64_t i = 0; i < totalmem / PAGESIZE; i += PAGESIZE) {
        vmm_map(i + HIGH_VMA, i, get_pml4(), TABLEPRESENT | TABLEWRITE);
    }

	serial_printf(KPRN_INFO, "MEM", "Mapped all memory\n");
	
	bitmap_clear(totalmem / PAGESIZE / 8);

    return;
}
