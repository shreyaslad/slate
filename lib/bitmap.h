#ifndef __BITMAP_H__
#define __BITMAP_H__

#include <stdint.h>
#include <stddef.h>
#include <alloc.h>
#include <locks.h>
#include <bit.h>

struct bitmap_t {
    size_t* bitmap;
    size_t entries;
    size_t free;

    spinlock_t bm_lock;
};

int bitmap_r(struct bitmap_t* bm, size_t idx, size_t n, size_t s);
int bitmap_f(struct bitmap_t* bm, size_t idx, size_t n);
int bitmap_a(struct bitmap_t* bm, size_t n);
int bitmap_n(struct bitmap_t* bm, size_t n);

#endif
