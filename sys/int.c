#include <sys/int.h>
#include <sys/ports.h>
#include <io.h>

static isr_t interrupt_handlers[256];

struct idt_entry {
	uint16_t offset_lo;
	uint16_t cs;
	uint8_t	zero_8;		
	uint8_t	flags;
	uint16_t offset_mid;
	uint32_t offset_hi;
	uint32_t zero_32;
} __attribute((packed));

struct idt_reg {
	uint16_t limit;
	uint64_t base;
} __attribute__((packed));

#define KNL_CS 		0x8
#define IDT_ENTRIES 256

static struct idt_entry idt[IDT_ENTRIES];
static struct idt_reg 	idtr;

#define MIDDLE_16(address)	(((uint32_t)address) >> 16)
#define HIGH_32(address)	(((uint32_t)address) >> 32)

static void set_entry(int idx, size_t handle, uint8_t flags) {
	idt[idx].offset_lo	= (uint16_t)handle;
	idt[idx].cs			= KNL_CS;
	idt[idx].zero_8		= 0;
	idt[idx].flags		= flags;
	idt[idx].offset_mid	= MIDDLE_16(handle);
	idt[idx].offset_hi	= HIGH_32(handle);
	idt[idx].zero_32	= 0;
}

static void load_idt() {
	idtr.limit = IDT_ENTRIES * sizeof(struct idt_entry) - 1;
	idtr.base = (uint64_t)&idt;

	asm volatile("lidt %0" :: "m"(idtr));
}

extern void err_0();
extern void err_1();
extern void err_2();
extern void err_3();
extern void err_4();
extern void err_5();
extern void err_6();
extern void err_7();
extern void err_8();
extern void err_9();
extern void err_10();
extern void err_11();
extern void err_12();
extern void err_13();
extern void err_14();
extern void err_15();
extern void err_16();
extern void err_17();
extern void err_18();
extern void err_19();
extern void err_20();
extern void err_21();
extern void err_22();
extern void err_23();
extern void err_24();
extern void err_25();
extern void err_26();
extern void err_27();
extern void err_28();
extern void err_29();
extern void err_30();
extern void err_31();

extern void irq_0();
extern void irq_1();
extern void irq_2();
extern void irq_3();
extern void irq_4();
extern void irq_5();
extern void irq_6();
extern void irq_7();
extern void irq_8();
extern void irq_9();
extern void irq_10();
extern void irq_11();
extern void irq_12();
extern void irq_13();
extern void irq_14();
extern void irq_15();

void init_isrs() {
	set_entry(0, (uint64_t)err_0, 0x8E);
	set_entry(1, (uint64_t)err_1, 0x8E);
	set_entry(2, (uint64_t)err_2, 0x8E);
	set_entry(3, (uint64_t)err_3, 0x8E);
	set_entry(4, (uint64_t)err_4, 0x8E);
	set_entry(5, (uint64_t)err_5, 0x8E);
	set_entry(6, (uint64_t)err_6, 0x8E);
	set_entry(7, (uint64_t)err_7, 0x8E);
	set_entry(8, (uint64_t)err_8, 0x8E);
	set_entry(9, (uint64_t)err_9, 0x8E);
	set_entry(10, (uint64_t)err_10, 0x8E);
	set_entry(11, (uint64_t)err_11, 0x8E);
	set_entry(12, (uint64_t)err_12, 0x8E);
	set_entry(13, (uint64_t)err_13, 0x8E);
	set_entry(14, (uint64_t)err_14, 0x8E);
	set_entry(15, (uint64_t)err_15, 0x8E);
	set_entry(16, (uint64_t)err_16, 0x8E);
	set_entry(17, (uint64_t)err_17, 0x8E);
	set_entry(18, (uint64_t)err_18, 0x8E);
	set_entry(19, (uint64_t)err_19, 0x8E);
	set_entry(20, (uint64_t)err_20, 0x8E);
	set_entry(21, (uint64_t)err_21, 0x8E);
	set_entry(22, (uint64_t)err_22, 0x8E);
	set_entry(23, (uint64_t)err_23, 0x8E);
	set_entry(24, (uint64_t)err_24, 0x8E);
	set_entry(25, (uint64_t)err_25, 0x8E);
	set_entry(26, (uint64_t)err_26, 0x8E);
	set_entry(27, (uint64_t)err_27, 0x8E);
	set_entry(28, (uint64_t)err_28, 0x8E);
	set_entry(29, (uint64_t)err_29, 0x8E);
	set_entry(30, (uint64_t)err_30, 0x8E);
	set_entry(31, (uint64_t)err_31, 0x8E);

	// Remap the PIC
	outb(0x20, 0x11);
	outb(0xA0, 0x11);
	outb(0x21, 0x20);
	outb(0xA1, 0x28);
	outb(0x21, 0x04);
	outb(0xA1, 0x02);
	outb(0x21, 0x01);
	outb(0xA1, 0x01);
	outb(0x21, 0x0);
	outb(0xA1, 0x0);

	// Install the IRQs
	set_entry(32, (uint64_t)irq_0, 0x8E);
	set_entry(33, (uint64_t)irq_1, 0x8E);
	set_entry(34, (uint64_t)irq_2, 0x8E);
	set_entry(35, (uint64_t)irq_3, 0x8E);
	set_entry(36, (uint64_t)irq_4, 0x8E);
	set_entry(37, (uint64_t)irq_5, 0x8E);
	set_entry(38, (uint64_t)irq_6, 0x8E);
	set_entry(39, (uint64_t)irq_7, 0x8E);
	set_entry(40, (uint64_t)irq_8, 0x8E);
	set_entry(41, (uint64_t)irq_9, 0x8E);
	set_entry(42, (uint64_t)irq_10, 0x8E);
	set_entry(43, (uint64_t)irq_11, 0x8E);
	set_entry(44, (uint64_t)irq_12, 0x8E);
	set_entry(45, (uint64_t)irq_13, 0x8E);
	set_entry(46, (uint64_t)irq_14, 0x8E);
	set_entry(47, (uint64_t)irq_15, 0x8E);

	load_idt();
	
	serial_printf(KPRN_INFO, "INT", "Installed Interrupts\n");
}

void irq_handler(registers_t* regs) {
	serial_printf(KPRN_WARN, "IRQ", "Got interrupt %U\n", regs->int_no);

	/* After every interrupt we need to send an EOI to the PICs
     * or they will not send another interrupt again */
	if (regs->int_no >= 40)
		outb(0xA0, 0x20); /* slave */
	outb(0x20, 0x20);	  /* master */

	/* Handle the interrupt in a more modular way */
	if (interrupt_handlers[regs->int_no]) {
		isr_t handler = interrupt_handlers[regs->int_no];
		handler(regs);
	}
}

void err_handler(registers_t* regs) {
	serial_printf(KPRN_ERR, "ERR", "oops!\n");
}

void register_interrupt_handler(uint8_t n, isr_t handler) {
	interrupt_handlers[n] = handler;
}
