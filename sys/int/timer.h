#ifndef __SYS_INT__TIMER_H__
#define __SYS_INT__TIMER_H__

#include <stdint.h>
#include <sys/int/isr.h>
#include <drivers/serial.h>
#include <proc/regs.h>

void init_timer(uint32_t freq);

#endif
