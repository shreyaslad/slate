#ifndef __MM__PMM_H__
#define __MM__PMM_H__

#include <stdint.h>
#include <stddef.h>
#include <io.h>
#include <mm/bit.h>
#include <mm/mem.h>
#include <sys/locks.h>

extern uint64_t* bitmap;
extern uint64_t bitmapEntries;
extern uint64_t totalmem;

/* Physical Memory Allocation */
void* pmm_alloc(size_t pages);
void pmm_free(void* ptr, size_t pages);
void* pmm_realloc(void* ptr, size_t old, size_t new);

#endif
