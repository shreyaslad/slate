#include <sys/int/apic.h>

static volatile const uint64_t ia32_apic_base = 0x1b;

uint32_t lapic_read(uint16_t offset) {
	uint32_t* volatile lapic_addr = (uint32_t* volatile)(madt->l_paddr + offset);
	return *lapic_addr;
}

void lapic_write(uint16_t offset, uint32_t val) {
	uint32_t* volatile lapic_addr = (uint32_t* volatile)(madt->l_paddr + offset);
	*lapic_addr = val;
}

void init_apic() {
	
	// Remap the PIC
	outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);

	// Disable the PIC
    outb(0xA1, 0xFF);
    outb(0x21, 0xFF);

	wrmsr(ia32_apic_base, ~(1 << 10));
	wrmsr(ia32_apic_base, (1 << 11));

	asm volatile("mov %0, %%cr8" :: "r"(0ull));

	lapic_write(0xf0, lapic_read(0xf0) | 0x100);

	if (rdmsr(ia32_apic_base) & (1 << 11))
		serial_printf(KPRN_INFO, "APIC", "LAPIC Enabled\n");

	uint32_t* lapic_base = (volatile uint32_t *)madt->l_paddr;
	serial_printf(KPRN_INFO, "APIC", "LAPIC Base: %x\n", (uint32_t)lapic_base);
}
