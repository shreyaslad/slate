#include <stdint.h>
#include <boot/stivale.h>
#include <drivers/serial.h>
#include <lib.h>
#include <mm/mem.h>
#include <sys/int.h>
#include <acpi/acpi.h>

void kmain(stivale_info_t* info) {
	init_serial();
	serial_printf("[SLATE] Initialized Serial\n");

	init_mem(info);
	serial_printf("[SLATE] Mapped all memory\n");

	init_acpi(info->rsdp + HIGH_VMA);
	serial_printf("[SLATE] Finished ACPI\n");

	while (1) {
		asm volatile("");
	}
}