#include <bitmap.h>

int bitmap_r(struct bitmap_t* bm, size_t idx, size_t n, size_t s) {
    if (!bm)
        return 0;

    spinlock_lock(&bm->bm_lock);

    bitmap_f(bm, idx, n);
    size_t ret = bitmap_a(bm, s);

    spinlock_release(&bm->bm_lock);
    return ret;
}

int bitmap_f(struct bitmap_t* bm, size_t idx, size_t n) {
    if (!bm)
        return 0;

    spinlock_lock(&bm->bm_lock);

    for (size_t i = idx; i < n; i++)
        cls_abs_bit(bm->bitmap, i);

    bm->free += n;

    spinlock_release(&bm->bm_lock);
    return n;
}

int bitmap_a(struct bitmap_t* bm, size_t n) {
    if (!bm)
        return 0;

    spinlock_lock(&bm->bm_lock);

    size_t first_bit = 0;
    size_t concurrent_bits = 0;

    for (size_t i = 0; i < (bm->entries * sizeof(size_t) * 8); i++) {

        if (!get_abs_bit(bm->bitmap, i)) {
            if (!concurrent_bits)
                first_bit = i;

            concurrent_bits++;

            if (n == concurrent_bits)
                goto alloc;
        } else {
            first_bit = 0;
            concurrent_bits = 0;
        }

        if (i == (bm->entries * sizeof(size_t) * 8) - 1) {
            krealloc(bm->bitmap, (bm->entries * sizeof(size_t)) + 1);
            bm->entries++;
            bm->free += sizeof(size_t);
        }
    }

    bm->free -= n;

    spinlock_release(&bm->bm_lock);
    return 0;

alloc:
    // iterate over bits now that a block has been found
    for (size_t i = first_bit; i < first_bit + n; i++) {
        set_abs_bit(bm->bitmap, i);
    }

    spinlock_release(&bm->bm_lock);
    return first_bit;
}

int bitmap_n(struct bitmap_t* bm, size_t n) {
    if (!bm)
        return 0;

    bm->bm_lock = 0; // is this safe

    spinlock_lock(&bm->bm_lock);

    bm->bitmap = kmalloc(sizeof(size_t) * n);
    bm->entries = 1;
    bm->free = 64;

    memset(bm->bitmap, 0, sizeof(size_t) * n);

    spinlock_release(&bm->bm_lock);
    return 1;
}
