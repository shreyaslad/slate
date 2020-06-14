#include <sys/smp.h>

void send_ipi(uint8_t ap, uint32_t ipi) {
	lapic_write(0x310, (ap << 24));
	lapic_write(0x300, ipi);
}

void init_smp() {
	for (int i = 1; i <= lapic_cnt; i++) {
		send_ipi(i, 0x500);

		serial_printf(KPRN_INFO, "SMP", "Sent Startup IPI sent to CPU #%d\n", i);
	}
}