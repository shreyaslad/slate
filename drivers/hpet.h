#ifndef __SYS__HPET_H__
#define __SYS__HPET_H__

#include <stdint.h>
#include <trace.h>
#include <mem.h>
#include <acpi/acpi.h>
#include <drivers/apic.h>

#undef __MODULE__
#define __MODULE__ "hpet"

void hpet_poll_and_sleep(uint64_t ms);
void init_hpet();

#endif
