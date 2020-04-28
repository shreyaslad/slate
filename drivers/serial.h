#ifndef __DRIVERS__SERIAL_H__
#define __DRIVERS__SERIAL_H__

#include <kstring.h>
#include <sys/ports.h>
#include <sys/locks.h>

#define PORT 0x3F8 /* COM1 */

void init_serial();
void serial_print(char* message);
void serial_print_int(int num);

#endif
