#include <stdint.h>
#include <boot/stivale.h>
#include <mem.h>
#include <assert.h>
#include <bitmap.h>
#include <sys/int.h>
#include <acpi/acpi.h>
#include <drivers/serial.h>
#include <drivers/vesa.h>
#include <drivers/apic.h>
#include <drivers/hpet.h>
#include <sys/smp.h>
#include <proc/scheduler.h>

void kmain(struct stivale_info_t* info) {
	init_serial();
	init_isrs();
	init_mem(info);
	init_vesa(info);
	init_acpi(info->rsdp + HIGH_VMA);
	init_apic();
	init_hpet();
	init_lapic_timer();
	init_scheduler();
	//init_smp();

	struct color_t bg = {38, 38, 38};
	clear_screen(&bg);

	asm volatile("sti");

	while (1) {
		asm volatile("");
	}
}
