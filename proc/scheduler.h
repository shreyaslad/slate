#ifndef __PROC__SCHEDULER_H__
#define __PROC__SCHEDULER_H__

#include <stdint.h>
#include <stddef.h>
#include <io.h>
#include <alloc.h>
#include <bitmap.h>
#include <vec.h>
#include <locks.h>
#include <proc/regs.h>
#include <sys/int.h>
#include <mm/vmm.h>

/* Thread States */
#define T_STATE_NOT_READY	0
#define T_STATE_RUNNING		1
#define T_STATE_IDLE		2
#define T_STATE_KILLABLE	3

struct thread_t {
    size_t tid;
    size_t ppid;
	size_t tpl;

	size_t cpu;
	size_t runtime;	// ms
	size_t state;
	
	struct vector_t* fds;

    struct registers_t* regdump;
};

struct process_t {
    size_t pid;
	size_t ppid;
    size_t* context;

	size_t runtime;	// ms

    struct process_t* parent;
    struct vector_t* children;
};

extern struct proc_t* cur_proc;
extern struct thread_t* cur_thread;

int spawn(size_t ppid, size_t entry_point, size_t tpl);
int fork(size_t tpid);

void schedule(struct registers_t* regs);

void init_scheduler();

#endif
