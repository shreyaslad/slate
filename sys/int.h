#ifndef __SYS__INT_H__
#define __SYS__INT_H__

#include <stdint.h>
#include <stddef.h>
#include <proc/regs.h>
#include <drivers/apic.h>

void irq_handler(registers_t* regs);
void err_handler(registers_t* regs);

typedef void (*isr_t)(registers_t*);
void register_interrupt_handler(uint8_t n, isr_t handler);

void init_isrs();

#endif
