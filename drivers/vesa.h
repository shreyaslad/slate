#ifndef __DRIVERS__VESA_H__
#define __DRIVERS__VESA_H__

#include <stdint.h>
#include <stddef.h>
#include <io.h> // nice circular include
#include <boot/stivale.h>
#include <mm/mem.h>
#include <mm/vmm.h>

struct color_t {
	uint8_t r;
	uint8_t g;
	uint8_t b;
};

uint32_t get_color(struct color_t* color);
void clear_screen(struct color_t* color);

void init_vesa(struct stivale_info_t* info);

#endif
