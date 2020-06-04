#ifndef __BOOT__STIVALE_H__
#define __BOOT__STIVALE_H__

#include <stdint.h>

#define STIVALE_MEMORY_AVAILABLE              1
#define STIVALE_MEMORY_RESERVED               2
#define STIVALE_MEMORY_ACPI_RECLAIMABLE       3
#define STIVALE_MEMORY_NVS                    4
#define STIVALE_MEMORY_BADRAM                 5

struct stivale_module_t {
    uint64_t begin;
    uint64_t end;
    char     string[128];
} __attribute__((packed));

struct mmap_entry_t {
    uint64_t addr;
    uint64_t len;
    uint32_t type;
    uint32_t unused;
} __attribute__((packed));

struct stivale_info_t {
    char    *cmdline;
    uint64_t memory_map_addr;
    uint64_t memory_map_entries;
    uint64_t framebuffer_addr;
    uint16_t framebuffer_pitch;
    uint16_t framebuffer_width;
    uint16_t framebuffer_height;
    uint16_t framebuffer_bpp;
    uint64_t rsdp;
    uint64_t module_count;
    struct stivale_module_t modules[];
} __attribute__((packed));

#endif
