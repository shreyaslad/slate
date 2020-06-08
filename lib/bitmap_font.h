#ifndef __LIB__BITMAP_FONT_H__
#define __LIB__BITMAP_FONT_H__

#include <stdint.h>

#define FONT_WIDTH	8
#define FONT_HEIGHT	16
#define FONT_MAX	(FONT_WIDTH * FONT_HEIGHT)

extern uint8_t bitmap_font[FONT_MAX];

#endif
