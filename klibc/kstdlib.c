#include <kstdlib.h>

void init_mem(stivale_info_t* info) {
    e820_entry_t* entry = (e820_entry_t*)info->memory_map_addr;

    for (int i = 0; i < info->memory_map_entries; i++) {
        totalmem += entry[i].len;
    }

    for (int i = 0; i < totalmem / PAGESIZE; i += PAGESIZE) {
        vmap(i, i + HIGH_VMA, get_pml4(), TABLEPRESENT | TABLEWRITE);
    }

    memset(bitmap, 0, totalmem);

    return;
}
