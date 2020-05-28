#include <stdint.h>
#include <boot/stivale.h>
#include <drivers/serial.h>
#include <lib.h>
#include <mm/vmm.h>
#include <sys/int.h>

void kmain(stivale_info_t* info) {
	init_serial();
	init_mem(info);

	while (1) {
		asm volatile("");
	}
}
