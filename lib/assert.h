#ifndef __ASSERT_H__
#define __ASSERT_H__

#include <trace.h>

#undef __MODULE__
#define __MODULE__ "assert"

#define ASSERT(expr) \
    if (!(expr)) { \
        ERR("Assertation \"%s\" failed\n", #expr); \
        ERR("\t%s:%d\n", __FILE__, __LINE__); \
        for (;;) ; \
    } \

#endif
