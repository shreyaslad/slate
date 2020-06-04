#include <mm/vmm.h>

static spinlock_t vmm_lock;

uint64_t* get_pml4() {
	uint64_t* addr;
	asm volatile("movq %%cr3, %0;" : "=r"(addr));
	return (uint64_t*)addr;
}

void set_pml4(uint64_t PML4) {
	asm volatile("movq %0, %%cr3;" ::"r"(PML4) : "memory");
}

void invlpg(uint64_t* vaddr) {
	asm volatile("invlpg (%0);" ::"r"(vaddr) : "memory");
}

void tlbflush() {
	set_pml4(get_pml4());
}

// virtual address to offset
void vtoof(offset_t* offset, uint64_t* vaddr) {
    size_t addr = (size_t)vaddr;

    offset->pml4off = (addr & ((size_t)0x1ff << 39)) >> 39;
    offset->pml3off = (addr & ((size_t)0x1ff << 30)) >> 30;
    offset->pml2off = (addr & ((size_t)0x1ff << 30)) >> 30;
    offset->pml1off = (addr & ((size_t)0x1ff << 12)) >> 12;

    return;
}

// maps a virtual address to a physical address
void vmm_map(uint64_t* vaddr, uint64_t* paddr, uint64_t* pml4ptr, uint64_t flags) {
	//spinlock_lock(&vmm_lock);
	
	offset_t offset;
    vtoof(&offset, vaddr);

	uint64_t	pml3phys = NULL;
	uint64_t*	pml3virt = NULL;
	uint64_t	pml2phys = NULL;
	uint64_t*	pml2virt = NULL;
    uint64_t	pml1phys = NULL;
    uint64_t*	pml1virt = NULL;

	if (pml4ptr[offset.pml4off] & TABLEPRESENT) {
		pml3phys = (uint64_t)(pml4ptr[offset.pml4off] & RMFLAGS);
		pml3virt = (uint64_t)(pml3phys + HIGH_VMA);
	} else {
		pml3phys = pmm_alloc(1);
		pml3virt = (uint64_t)(pml3phys + HIGH_VMA);
		pml4ptr[offset.pml4off] = (uint64_t)pml3phys | flags;
	}

	if (pml3virt[offset.pml3off] & TABLEPRESENT) {
		pml2phys = (uint64_t)(pml3virt[offset.pml3off] & RMFLAGS);
		pml2virt = (uint64_t)(pml2phys + HIGH_VMA);
	} else {
		pml2phys = pmm_alloc(1);
		pml2virt = (uint64_t)(pml2phys + HIGH_VMA);
		pml3virt[offset.pml3off] = (uint64_t)pml2phys | flags;
	}

	if (pml2virt[offset.pml2off] & TABLEPRESENT) {
		pml1phys = (uint64_t)(pml2virt[offset.pml2off] & RMFLAGS);
		pml1virt = (uint64_t)(pml1phys + HIGH_VMA);
	} else {
		pml1phys = pmm_alloc(1);
		pml1virt = (uint64_t)(pml1phys + HIGH_VMA);
		pml2virt[offset.pml2off] = (uint64_t)pml1phys | flags;
	}

	pml1virt[offset.pml1off] = (uint64_t)paddr | flags;

	invlpg(vaddr);

    //asm volatile("sti");
	//spinlock_release(&vmm_lock);

	return;
}

void vmm_free(uint64_t* vaddr, size_t pages) {
	spinlock_lock(&vmm_lock);
	asm volatile("cli");
	
	offset_t offset;
	vtoof(&offset, vaddr);
	uint64_t* pml4ptr = get_pml4();
	uint64_t* pml3ptr = (uint64_t*)(pml4ptr[offset.pml4off] & RMFLAGS);
	uint64_t* pml2ptr = (uint64_t*)(pml3ptr[offset.pml3off] & RMFLAGS);
	for (uint64_t i = offset.pml2off; i < pages + 1; i++) {
		pml2ptr[i] = 0; // TODO: free page table if necessary
	}
	
	tlbflush(vaddr);
	spinlock_release(&vmm_lock);

	return;
}

void* vmm_fork(uint64_t* pml4) {
    //spinlock_lock(&vmm_lock);
    //asm volatile("cli");

    uint64_t* pml4_cpy = (uint64_t*)((uint64_t)pmm_alloc(1) + HIGH_VMA);
    memcpy(pml4_cpy, pml4, TABLESIZE);

    for (int i = 0; i < 512; i++) {
        if (pml4[i] & TABLEPRESENT) {
            uint64_t* pml3 = (uint64_t*)(pml4[i] & RMFLAGS);
            uint64_t* pml3_cpy = (uint64_t*)((uint64_t)pmm_alloc(1) + HIGH_VMA);
            memcpy(pml3_cpy, pml3, TABLESIZE);

            pml4_cpy[i] = (uint64_t)pml3_cpy | (pml4[i] & ~RMFLAGS);

            for (int j = 0; j < 512; j++) {
                if (pml3[j] & TABLEPRESENT) {
                    uint64_t* pml2 = (uint64_t *)(pml3[j] & RMFLAGS);
                    uint64_t* pml2_cpy = (uint64_t *)((uint64_t)pmm_alloc(1) + HIGH_VMA);
                    memcpy(pml2_cpy, pml2, TABLESIZE);

                    pml3_cpy[j] = (uint64_t)pml2_cpy | (pml3[j] & ~RMFLAGS);

                    for (int k = 0; k < 512; k++) {
                        if (pml2[k] & TABLEPRESENT) {
                            uint64_t* pml1 = (uint64_t *)(pml2[k] & RMFLAGS);
                            uint64_t* pml1_cpy = (uint64_t *)((uint64_t)pmm_alloc(1) + HIGH_VMA);
                            memcpy(pml1_cpy, pml1, TABLESIZE);

                            pml2_cpy[k] = (uint64_t)pml1_cpy | (pml2[k] & ~RMFLAGS);

                            memcpy(*(uint64_t *)(pml1_cpy[k] & RMFLAGS), *(uint64_t *)(pml1[k] & RMFLAGS), PAGESIZE);
                        }
                    }
                }
            }
        }
    }

    //asm volatile("sti");
    //spinlock_release(&vmm_lock);

    return (void *)pml4_cpy;
}
