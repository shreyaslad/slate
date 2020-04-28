#include <kstdlib.h>

void init_mem(stivale_info_t* info) {
    e820_entry_t* entry = (e820_entry_t*)info->memory_map_addr;

    for (int i = 0; i < info->memory_map_entries; i++) {
        serial_printf("Entry: %x | Len: %d | Type: %d\n", entry[i].addr, entry[i].len, entry[i].type);
        totalmem += entry[i].len;
    }

    serial_printf("Total Memory: %x\n", totalmem);

    /*for (int i = 0; i < totalmem / PAGESIZE; i += PAGESIZE) {
        vmap(i, i + HIGH_VMA, get_pml4(), TABLEPRESENT | TABLEWRITE);
    }

    memset(bitmap, 0, totalmem);*/

    return;
}
