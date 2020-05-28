#include <lib.h>

void init_mem(stivale_info_t* info) {
    e820_entry_t* entry = (e820_entry_t*)info->memory_map_addr;
	
	serial_printf("here");

    for (int i = 0; i < info->memory_map_entries; i++) {
        totalmem += entry[i].len;
    }
	
	serial_printf("here");

    for (int i = 0; i < totalmem / PAGESIZE; i += PAGESIZE) {
        vmm_map(i, i + HIGH_VMA, get_pml4(), TABLEPRESENT | TABLEWRITE);
    }
	
	serial_printf("here");
	
	bitmap_clear(totalmem / PAGESIZE / 8);
	
	serial_printf("here");

    return;
}
