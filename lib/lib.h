#ifndef __LIB__LIB_H__
#define __LIB__LIB_H__

#include <stdint.h>
#include <stddef.h>
#include <io.h>
#include <boot/stivale.h>
#include <mm/mem.h>
#include <mm/pmm.h>
#include <mm/vmm.h>
#include <drivers/serial.h>

#define UNUSED(x) (void)(x)
#define EXIT_SUCCESS 1
#define EXIT_FAILURE 0

extern uint64_t _kernel_start;
extern uint64_t _kernel_end;

void init_mem(stivale_info_t* info);

#endif
