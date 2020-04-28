#include <stdint.h>
#include <boot/stivale.h>
#include <kstdio.h>
#include <kstdlib.h>

void kmain(stivale_info_t* info) {
    init_serial();

    init_mem(info);

    for(;;) ;
}
