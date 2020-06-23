#ifndef __SYS__INT_H__
#define __SYS__INT_H__

#include <stdint.h>
#include <stddef.h>
#include <proc/regs.h>
#include <drivers/apic.h>

void isr_handler(registers_t* regs);

void init_isrs();

#endif
