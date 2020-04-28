#include <mm/mem.h>

void* memset(void* bufptr, int value, size_t size) {
    unsigned char* buf = (unsigned char*)bufptr;
    for (uint64_t i = 0; i < size; i++) {
        buf[i] = value;
    }

    return bufptr;
}

void* memcpy(void* dest, const void* src, size_t size) {
    const unsigned char* src2 = (const unsigned char*)src;
    unsigned char* dst = (unsigned char*)dest;
    for (uint64_t i = 0; i < size; i++) {
        dst[i] = src2[i];
    }
    return dst;
}

int memcmp(const void* s1, const void* s2, size_t n) {
    const uint8_t* p1 = s1;
    const uint8_t* p2 = s2;

    for (size_t i = 0; i < n; i++) {
    if (p1[i] != p2[i])
        return p1[i] < p2[i] ? -1 : 1;
    }

    return 0;
}
