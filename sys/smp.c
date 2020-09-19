#include <sys/smp.h>

void send_ipi(uint8_t ap, uint32_t ipi) {
    lapic_write(0x310, (ap << 24));
    lapic_write(0x300, ipi);
}

void init_smp(struct stivale2_struct_tag_smp* smp) {
    ;
}
