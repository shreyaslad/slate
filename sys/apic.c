#include <sys/apic.h>

const uint64_t ia32_apic_base = 0x1b;

int init_apic() {
	wrmsr(ia32_apic_base, ~(1 << 10));
	wrmsr(ia32_apic_base, (1 << 11));

	//asm volatile("movq %0, %%cr8" :: "r"(0));
}