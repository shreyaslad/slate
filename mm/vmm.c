#include <mm/vmm.h>

static spinlock_t vmm_lock;

size_t* get_pml4() {
    size_t* addr;
    asm volatile("movq %%cr3, %0;" : "=r"(addr));
    return (size_t*)addr;
}

void set_pml4(size_t PML4) {
    asm volatile("movq %0, %%cr3;" ::"r"(PML4) : "memory");
}

void invlpg(size_t* vaddr) {
    asm volatile("invlpg (%0);" ::"r"(vaddr) : "memory");
}

void tlbflush() {
    set_pml4(get_pml4());
}

// virtual address to offset
void vtoof(offset_t* offset, size_t* vaddr) {
    size_t addr = (size_t)vaddr;

    offset->pml4off = (addr & ((size_t)0x1ff << 39)) >> 39;
    offset->pml3off = (addr & ((size_t)0x1ff << 30)) >> 30;
    offset->pml2off = (addr & ((size_t)0x1ff << 30)) >> 30;
    offset->pml1off = (addr & ((size_t)0x1ff << 12)) >> 12;

    return;
}

void vmm_map(size_t* vaddr, size_t* paddr, size_t* pml4ptr, size_t flags) {
    spinlock_lock(&vmm_lock);
    
    offset_t offset;
    vtoof(&offset, vaddr);

    size_t  pml3phys = NULL;
    size_t* pml3virt = NULL;
    size_t  pml2phys = NULL;
    size_t* pml2virt = NULL;
    size_t  pml1phys = NULL;
    size_t* pml1virt = NULL;

    if (pml4ptr[offset.pml4off] & TABLEPRESENT) {
        pml3phys = (size_t)(pml4ptr[offset.pml4off] & RMFLAGS);
        pml3virt = (size_t)(pml3phys + HIGH_VMA);
    } else {
        pml3phys = pmm_alloc(1);
        pml3virt = (size_t)(pml3phys + HIGH_VMA);
        pml4ptr[offset.pml4off] = (size_t)pml3phys | flags;
    }

    if (pml3virt[offset.pml3off] & TABLEPRESENT) {
        pml2phys = (size_t)(pml3virt[offset.pml3off] & RMFLAGS);
        pml2virt = (size_t)(pml2phys + HIGH_VMA);
    } else {
        pml2phys = pmm_alloc(1);
        pml2virt = (size_t)(pml2phys + HIGH_VMA);
        pml3virt[offset.pml3off] = (size_t)pml2phys | flags;
    }

    if (pml2virt[offset.pml2off] & TABLEPRESENT) {
        pml1phys = (size_t)(pml2virt[offset.pml2off] & RMFLAGS);
        pml1virt = (size_t)(pml1phys + HIGH_VMA);
    } else {
        pml1phys = pmm_alloc(1);
        pml1virt = (size_t)(pml1phys + HIGH_VMA);
        pml2virt[offset.pml2off] = (size_t)pml1phys | flags;
    }

    pml1virt[offset.pml1off] = (size_t)paddr | flags;

    invlpg(vaddr);

    //asm volatile("sti");
    spinlock_release(&vmm_lock);

    return;
}

void vmm_free(size_t* vaddr, size_t pages) {
    spinlock_lock(&vmm_lock);
    asm volatile("cli");
    
    offset_t offset;
    vtoof(&offset, vaddr);
    size_t* pml4ptr = get_pml4();
    size_t* pml3ptr = (size_t*)(pml4ptr[offset.pml4off] & RMFLAGS);
    size_t* pml2ptr = (size_t*)(pml3ptr[offset.pml3off] & RMFLAGS);
    
    for (size_t i = offset.pml2off; i < pages + 1; i++) {
        pml2ptr[i] = 0; // TODO: free page table if necessary
    }
    
    tlbflush(vaddr);
    spinlock_release(&vmm_lock);

    return;
}
