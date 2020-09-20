#ifndef __SYS__INT_H__
#define __SYS__INT_H__

#include <stdint.h>
#include <stddef.h>
#include <alloc.h>
#include <trace.h>
#include <proc/regs.h>
#include <drivers/apic.h>
#include <drivers/vesa.h>

#undef __MODULE__
#define __MODULE__ "int"

void register_handler(uint8_t int_no, void (*handler)(struct regs_t*));
void init_isrs();

#endif
