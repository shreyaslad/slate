#include <stdint.h>
#include <boot/stivale.h>
#include <io.h>
#include <lib.h>
#include <lib/mem.h>
#include <sys/int.h>
#include <acpi/acpi.h>
#include <drivers/serial.h>
#include <drivers/vesa.h>
#include <drivers/apic.h>
#include <drivers/hpet.h>
#include <sys/smp.h>

void kmain(struct stivale_info_t* info) {
	init_serial();
	init_vesa(info);
	init_isrs();
	init_mem(info);
	init_acpi(info->rsdp + HIGH_VMA);
	//init_hpet();
	//init_smp();

	asm volatile("sti\nud2");

	struct color_t bg = {38, 38, 38};
	clear_screen(&bg);

	while (1) {
		asm volatile("");
	}
}
