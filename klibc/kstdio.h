#ifndef __LIBC__KSTDIO_H__
#define __LIBC__KSTDIO_H__

#include <stdint.h>
#include <stdarg.h>
#include <kstring.h>
#include <mm/mem.h>
#include <sys/locks.h>
#include <drivers/serial.h>

void serial_printf(char* message, ...);

#endif
