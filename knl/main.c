#include <stdint.h>
#include <boot/stivale.h>
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

__attribute__((noreturn))
void kmain(struct stivale_info_t* info) {
	init_serial();
	init_isrs();
	init_mem(info);
	init_vesa(info);

	init_acpi(info->rsdp + HIGH_VMA);
	init_apic();
	init_hpet();
	init_lapic_timer();

	init_pci();

	init_vfs();
	init_fds();

	struct color_t bg = {52, 84, 94};
	struct color_t fg = {186, 186, 186};
	clear_screen(&bg);
	
	init_scheduler();

	printf(KPRN_NONE, "\n");
	printf(KPRN_INFO, "-------------------------------\n");
	printf(KPRN_INFO, "|            Slate            |\n");
	printf(KPRN_INFO, "|            -----            |\n");
	printf(KPRN_INFO, "|        acpi is meme.        |\n");
	printf(KPRN_INFO, "-------------------------------\n");

	printf(KPRN_INFO, "Built %s %s\n\n", __DATE__, __TIME__);

	while (1)
		asm volatile("");
}
