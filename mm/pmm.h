#ifndef __MM__PMM_H__
#define __MM__PMM_H__

#include <stdint.h>
#include <stddef.h>
#include <kstdio.h>
#include <mm/bit.h>
#include <mm/mem.h>
#include <sys/locks.h>

extern uint64_t* bitmap;
extern uint64_t bitmapEntries;
extern uint64_t totalmem;

/* Physical Memory Allocation */
void* pmalloc(size_t pages);
void pmfree(void* ptr, size_t pages);
void* pmrealloc(void* ptr, size_t oldSize, size_t newSize);

#endif
