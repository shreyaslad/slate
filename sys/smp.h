#ifndef __SYS__SMP_H__
#define __SYS__SMP_H__

#include <stdint.h>
#include <io.h>
#include <drivers/apic.h>

void send_ipi(uint8_t ap, uint32_t ipi);

void init_smp();

#endif
