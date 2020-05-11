#ifndef __ACPI__ACPI_H__
#define __ACPI__ACPI_H__

#include <stdint.h>
#include <stddef.h>

struct acpi_sdt_header {
    char signature[4];
    uint32_t len;
    uint8_t rev;
    uint8_t csum;
    char oem_id[6];
    char oem_table_id[8];
    uint32_t oem_rev;
    uint32_t creator_id;
    uint32_t creator_rev;
} __attribute__((packed));

void init_acpi(uint64_t rsdp);

#endif
