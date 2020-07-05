#ifndef __PROC__SCHEDULER_H__
#define __PROC__SCHEDULER_H__

#include <stdint.h>
#include <stddef.h>
#include <io.h>
#include <alloc.h>
#include <assert.h>
#include <bitmap.h>
#include <vec.h>
#include <locks.h>
#include <proc/regs.h>
#include <sys/int.h>
#include <mm/vmm.h>

int kill(size_t tid);
int exec(size_t tid, size_t entry_point, size_t tpl);
int spawn(size_t ppid);
int fork(size_t ppid);

void schedule(struct registers_t* regs);

void init_scheduler();

#endif
