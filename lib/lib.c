#include <lib.h>

void init_mem(stivale_info_t* info) {
    mmap_entry_t* entry = (mmap_entry_t*)info->memory_map_addr;

    for (int i = 0; i < info->memory_map_entries; i++) {
        totalmem += entry[i].len;
    }

    serial_printf("[MEM] Total Memory: %d\n", totalmem);

    for (int i = 0; i < totalmem / PAGESIZE; i += PAGESIZE) {
        vmm_map(i, i + HIGH_VMA, get_pml4(), TABLEPRESENT | TABLEWRITE);
    }
	
	bitmap_clear(totalmem / PAGESIZE / 8);

    return;
}