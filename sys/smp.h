#ifndef __SYS__SMP_H__
#define __SYS__SMP_H__

#include <stdint.h>
#include <trace.h>
#include <alloc.h>
#include <mem.h>
#include <boot/stivale2.h>
#include <drivers/apic.h>

#undef __MODULE__
#define __MODULE__ "smp"

void send_ipi(uint8_t ap, uint32_t ipi);
void init_smp();

#endif
