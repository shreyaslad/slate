#ifndef __DRIVERS__SERIAL_H__
#define __DRIVERS__SERIAL_H__

#include <str.h>
#include <sys/ports.h>
#include <locks.h>

#define PORT 0x3F8 /* COM1 */

void serial_write(char a);
void serial_print(char* message);
void serial_print_int(int num);

void init_serial();

#endif
