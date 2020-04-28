#include <kstdio.h>

static spinlock_t io_lock;

void serial_printf(char* message, ...) {
    spinlock_lock(&io_lock);

    va_list ap;
    uint64_t len = strlen(message);

    char buffer[1024];
    memset(buffer, NULL, 1024);

    va_start(ap, message);

    for (uint64_t i = 0; i < len; i++) {
        if (message[i] == '%' && message[i + 1] == 's') {
            ++i;
            strcat(buffer, va_arg(ap, char*));
        } else if (message[i] == '%' && message[i + 1] == 'd') {
            ++i;
            char buf[64];
            memset(buf, NULL, 64);
            itoa((uint64_t)va_arg(ap, uint64_t), buf);
            strcat(buffer, buf);

            memset(buf, NULL, 64);
        } else if (message[i] == '%' && message[i + 1] == 'x') {
            ++i;
            char buf[64];
            memset(buf, NULL, 64);
            htoa((uint64_t)va_arg(ap, uint64_t), buf);

            strcat(buffer, buf);

            memset(buf, NULL, 64);
        } else {
            append(buffer, message[i]);
        }
    }

    va_end(ap);

    serial_print(buffer);

    spinlock_release(&io_lock);
    return;
}
