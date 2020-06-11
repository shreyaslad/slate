#include <drivers/hpet.h>

struct acpi_hpet_addr_t {
	uint8_t addr_space_id;
	uint8_t reg_bit_width;
	uint8_t reg_bit_off;
	uint8_t reserved;
	uint64_t addr;
} __attribute__((packed));

struct acpi_hpet_t {
	struct sdt_t sdt;
	uint8_t hw_rev_id;
	uint8_t comparator_cnt;
	uint8_t counter_size;
	uint8_t reserved;
	uint8_t legacy;
	uint16_t pci_v_id;
	struct acpi_hpet_addr_t addr;
	uint8_t num;
	uint16_t min_tick;
	uint8_t page_prot;
} __attribute__((packed));;

struct hpet_t {
	uint64_t general_capabilities;
	uint64_t unused0;
	uint64_t general_config;
	uint64_t unused1;
	uint64_t general_int_status;
	uint64_t unused2;
	uint64_t unused3[24];
	uint64_t main_counter_value;
	uint64_t unused4;
} __attribute__((packed));

static struct acpi_hpet_t* acpi_hpet_table;
static struct hpet_t* hpet;
static uint64_t clk;

uint64_t hpet_read(uint64_t reg) {
	uint64_t* volatile ptr = (uint64_t* volatile)(acpi_hpet_table->addr.addr + KERNEL_HIGH_VMA + reg);
	return * ptr;
}

void hpet_write(uint64_t reg, uint64_t val) {
	uint64_t* volatile ptr = (uint64_t* volatile)(acpi_hpet_table->addr.addr + KERNEL_HIGH_VMA + reg);
	*ptr = val;
}

void hpet_sleep(uint64_t ms) {
	uint64_t target = hpet->main_counter_value + (ms * 1000000000000) / clk;

	while (hpet->main_counter_value < target)
		asm volatile("");
}

void init_hpet() {
	acpi_hpet_table = (struct acpi_hpet_t *)find_sdt("HPET", 0);
	
	if (!acpi_hpet_table)
		return;

	hpet = (struct hpet_t *)(acpi_hpet_table->addr.addr + KERNEL_HIGH_VMA);

	clk = ((hpet->general_capabilities >> 32) & 0xFFFFFFFF);
}