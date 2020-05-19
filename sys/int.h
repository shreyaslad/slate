#ifndef __SYS__INT_H__
#define __SYS__INT_H__

#include <stdint.h>
#include <stddef.h>
#include <proc/regs.h>

void isr_install();
void err_handler(registers_t* regs);

#endif
