#ifndef __ASSERT_H__
#define __ASSERT_H__

#include <io.h>

#define ASSERT(expr) \
	if (!(expr)) { \
		asm volatile("cli"); \
		serial_printf(KPRN_ERR, "ASSERT", "Assert \"%s\" failed at %s:%d\n", #expr, __FILE__, __LINE__); \
		asm volatile("hlt"); \
	}

#endif
