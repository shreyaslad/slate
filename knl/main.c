#include <stdint.h>
#include <boot/stivale.h>
#include <kstdio.h>
#include <kstdlib.h>
#include <sys/int/isr.h>

void kmain(stivale_info_t* info) {
    init_serial();
    init_mem(info);

    isr_install();
    irq_install();

    for(;;) ;
}
