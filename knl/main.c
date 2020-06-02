#include <stdint.h>
#include <boot/stivale.h>
#include <drivers/serial.h>
#include <lib.h>
#include <mm/vmm.h>
#include <sys/int.h>

void kmain(struct stivale_info* info) {
	init_serial();
	serial_printf("[SLATE] Initialized Serial\n");

	init_mem(info);
	serial_printf("[MEM] Mapped all memory\n");
}
