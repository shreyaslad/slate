#ifndef __DRIVERS__TIMER_H__
#define __DRIVERS__TIMER_H__

#include <stdint.h>
#include <io.h>
#include <proc/regs.h>
#include <sys/ports.h>
#include <sys/int.h>

void init_timer(uint32_t freq);

#endif
