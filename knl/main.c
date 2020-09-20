#include <stdint.h>
#include <boot/stivale2.h>
#include <trace.h>
#include <mem.h>
#include <sys/interrupts.h>
#include <acpi/acpi.h>
#include <drivers/serial.h>
#include <drivers/vesa.h>
#include <drivers/apic.h>
#include <drivers/hpet.h>
#include <drivers/pci.h>
#include <proc/task.h>
#include <fs/vfs.h>
#include <fs/fd.h>

#undef __MODULE__
#define __MODULE__ "slate"

__attribute__((noreturn))
void kmain(struct stivale2_struct* info) {
    init_serial();
    init_isrs();

    struct stivale2_struct_tag_memmap*      memmap;
    struct stivale2_struct_tag_framebuffer* fb;
    struct stivale2_struct_tag_rsdp*        rsdp;
    struct stivale2_struct_tag_smp*         smp;

    for (struct stivale2_tag* cur = (struct stivale2_tag *)info->tags; cur; cur = cur->next) {
        switch (cur->identifier) {
            case STIVALE2_STRUCT_TAG_MEMMAP_ID:
                memmap = (struct stivale2_struct_tag_memmap *)cur;
                break;
            case STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID:
                fb = (struct stivale2_struct_tag_framebuffer *)cur;
                break;
            case STIVALE2_STRUCT_TAG_RSDP_ID:
                rsdp = (struct stivale2_struct_tag_rsdp *)cur;
                break;
            case STIVALE2_STRUCT_TAG_SMP_ID:
                smp = (struct stivale2_struct_tag_smp *)cur;
                break;

            default:
                WARN("Found unknown identifier: %lx\n",
                                  cur->identifier);
        }
    }

    init_vesa(fb);
    init_mem(memmap);

    init_acpi(rsdp->rsdp + HIGH_VMA);
    init_apic();
    init_hpet();
    init_lapic_timer();
    init_smp(smp);

    init_pci();

    init_vfs();
    init_fds();

    clear_screen(NULL);
    
    init_scheduler();

    printf("\n");
    printf("-------------------------------\n");
    printf("|            Slate            |\n");
    printf("|            -----            |\n");
    printf("|        acpi is meme.        |\n");
    printf("-------------------------------\n");

    printf("Built %s %s\n\n", __DATE__, __TIME__);

    while (1) {
        asm volatile("");
    }
}
