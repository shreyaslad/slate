#ifndef __PROC__SCHEDULER_H__
#define __PROC__SCHEDULER_H__

#include <stdint.h>
#include <stddef.h>
#include <io.h>
#include <alloc.h>
#include <vec.h>
#include <locks.h>
#include <proc/regs.h>
#include <sys/int.h>
#include <mm/vmm.h>

/* Thread States */
#define T_STATE_STOPPED		0
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

struct proc_t {
	char name[50];

    size_t pid;
	size_t ppid;
    size_t* context;

	size_t runtime;	// ms

    struct proc_t* parent;
    struct vector_t* children;
};

extern struct proc_t* cur_proc;
extern struct thread_t* cur_thread;

void schedule(struct registers_t* regs);

void init_scheduler();

#endif
