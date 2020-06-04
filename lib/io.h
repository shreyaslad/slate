#ifndef __LIB__IO_H__
#define __LIB__IO_H__

#include <stdint.h>
#include <stdarg.h>
#include <str.h>
#include <alloc.h>
#include <mm/mem.h>
#include <sys/locks.h>
#include <drivers/serial.h>

#define KPRN_INFO	0
#define KPRN_WARN	1
#define KPRN_ERR	2

void serial_printf(int kprn, const char* sub, const char* fmt, ...);

#endif
