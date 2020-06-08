#ifndef __SYS__APIC_H__
#define __SYS__APIC_H__

#include <stdint.h>
#include <io.h>
#include <sys/ports.h>
#include <sys/msrs.h>
#include <acpi/madt.h>

uint32_t lapic_read(uint16_t offset);
void lapic_write(uint16_t offset, uint32_t val);

void init_apic();

#endif
