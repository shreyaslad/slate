#include <sys/trace.h>

char* trace_addr(size_t* offset, size_t addr) {
	for (size_t i = 0; ; i++) {
		if (symlist[i].addr >= addr) {
			*offset = addr - symlist[i - 1].addr;
			return symlist[i - 1].name;
		}
	}
}

void stacktrace(size_t* base) {
	printf(KPRN_ERR, "err: Stacktrace:\n");

	for (;;) {
		size_t old_bp = base[0];
		size_t ret_addr = base[1];
		size_t off;

		if (!ret_addr)
			break;

		char* name = trace_addr(&off, ret_addr);
		printf(KPRN_ERR, "err:\t[%#16lx] <%s+%#lx>\n", ret_addr, name, off);

		if (!old_bp)
			break;

		base = (void *)old_bp;
	}
}
