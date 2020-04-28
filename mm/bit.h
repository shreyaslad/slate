#ifndef __MM__BIT_H__
#define __MM__BIT_H__

#include <stdint.h>
#include <stddef.h>

uint8_t getBit(uint8_t num, uint8_t bit);
void setBit(uint8_t* num, uint8_t bit, uint8_t state);

uint8_t getAbsoluteBit(uint64_t* bitmap, uint64_t bit);
void setAbsoluteBit(uint64_t* bitmap, uint64_t bit);
void clearAbsoluteBit(uint64_t* bitmap, uint64_t bit);

#endif
