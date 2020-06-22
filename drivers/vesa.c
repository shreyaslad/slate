#include <drivers/vesa.h>

static struct stivale_info_t* fb_info;

#define RED_SHIFT	16
#define GREEN_SHIFT	8
#define BLUE_SHIFT	0

uint32_t get_color(struct color_t* color) {
	return (uint32_t)((color->r << RED_SHIFT) | (color->g << GREEN_SHIFT) | (color->b << BLUE_SHIFT));
}

void put_pixel(uint64_t x, uint64_t y, uint32_t color) {
	size_t fb_i = x + (fb_info->framebuffer_pitch / sizeof(uint32_t)) * y;
	uint32_t* fb = (uint32_t *)fb_info->framebuffer_addr;

	fb[fb_i] = color;
}

void clear_screen(struct color_t* color) {
	for (int i = 0; i < fb_info->framebuffer_width; i++) {
		for (int j = 0; j < fb_info->framebuffer_height; j++) {
			put_pixel(i, j, get_color(color));
		}
	}
}

void draw_char(char c, uint64_t x, uint64_t y, struct color_t* fg, struct color_t* bg) {
	int orig_x = x;
	uint8_t* glyph = &bitmap_font[c * FONT_HEIGHT];

	for (int i = 0; i < FONT_HEIGHT; i++) {
		for (int j = FONT_WIDTH - 1; j >= 0; j--)
			put_pixel(x++, y, (glyph[i] & j) ? get_color(fg) : get_color(bg));

		y++;
		x = orig_x;
	}
}

void init_vesa(struct stivale_info_t* info) {
	fb_info = info;
	fb_info->framebuffer_addr += HIGH_VMA;

	size_t fb_size = info->framebuffer_height * info->framebuffer_pitch;

	for (int i = info->framebuffer_addr; i < info->framebuffer_addr + fb_size; i += PAGESIZE) {
		serial_printf(KPRN_INFO, "VESA", "Mapped %X to %X\n", i, i + HIGH_VMA);
		vmm_map(i + HIGH_VMA, i, get_pml4(), TABLEPRESENT | TABLEWRITE);
	}

	serial_printf(KPRN_INFO, "VESA", "Got a %Ux%U display\n", info->framebuffer_width, info->framebuffer_height);
}
