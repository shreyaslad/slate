#ifndef __SYS__INT__HPET_H__
#define __SYS__INT__HPET_H__

#include <stdint.h>
#include <io.h>
#include <mem.h>
#include <acpi/acpi.h>
#include <drivers/apic.h>

void hpet_poll_and_sleep(uint64_t ms);
void init_hpet();

#endif
