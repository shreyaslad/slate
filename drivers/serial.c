#include <drivers/serial.h>

spinlock_t serial_lock;

void init_serial() {
  outb(PORT + 1, 0x00); // Disable all interrupts
  outb(PORT + 3, 0x80); // Enable DLAB (set baud rate divisor)
  outb(PORT + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
  outb(PORT + 1, 0x00); //                  (hi byte)
  outb(PORT + 3, 0x03); // 8 bits, no parity, one stop bit
  outb(PORT + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
  outb(PORT + 4, 0x0B); // IRQs enabled, RTS/DSR set
}

static int is_transmit_empty() { return inb(PORT + 5) & 0x20; }

void serial_write(char a) {
  while (is_transmit_empty() == 0)
    ;

  outb(PORT, a);
}

void serial_print(char* message) {
    spinlock_lock(&serial_lock);

    int i = 0;
    while (message[i] != 0) {
        serial_write(message[i++]);
    }

    spinlock_release(&serial_lock);
}

void serial_print_int(int num) {
    char toprint[33];
    itoa(num, toprint);
    serial_print(toprint);
}
