#ifndef __SYS__LOCKS_H__
#define __SYS__LOCKS_H__

#include <stdint.h>

typedef volatile uint64_t spinlock_t;

extern void spinlock_lock(spinlock_t* spinlock);
extern void spinlock_release(spinlock_t* spinlock);

#endif