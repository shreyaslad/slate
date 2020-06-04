#include <stdint.h>
#include <boot/stivale.h>
#include <io.h>
#include <lib.h>
#include <alloc.h>
#include <mm/mem.h>
#include <sys/int.h>
#include <acpi/acpi.h>
#include <drivers/serial.h>
#include <drivers/vesa.h>

void kmain(struct stivale_info_t* info) {
	init_serial();
	//init_vesa(info);

	/*struct color_t color = {255, 0, 0};
	clear_screen(&color);*/

	isr_install();
	init_mem(info);

	init_acpi(info->rsdp + HIGH_VMA);

	while (1) {
		asm volatile("");
	}
}
