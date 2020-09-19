#ifndef __ASSERT_H__
#define __ASSERT_H__

#include <io.h>

#define ASSERT(expr) \
    if (!(expr)) { \
        asm volatile("cli"); \
        printf(KPRN_ERR, "assert: Assertation \"%s\" failed\n", #expr); \
        printf(KPRN_ERR, "assert:\t%s:%d\n", __FILE__, __LINE__); \
        asm volatile("hlt"); \
    } \

#endif
