#include <lib.h>

void init_mem(struct stivale_info* info) {
	struct mmap_entry* entry = (struct mmap_entry *)info->memory_map_addr;

	for (int i = 0; i < info->memory_map_entries; i++) {
		totalmem += entry[i].length;
	}
	
	/*uint64_t* new_pml4 = pmm_alloc(1);

	serial_printf("New PML4: %x\n", (uint64_t)new_pml4 + KERNEL_HIGH_VMA);

	for (int i = 0; i < totalmem; i += PAGESIZE) {
		vmm_map(i + HIGH_VMA, i, new_pml4 + KERNEL_HIGH_VMA, TABLEWRITE | TABLEPRESENT);
	}*/

	bitmap_clear(totalmem / PAGESIZE / 8);
	
    return;
}
