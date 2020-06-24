#ifndef __PROC__SCHEDULER_H__
#define __PROC__SCHEDULER_H__

#include <stdint.h>
#include <stddef.h>
#include <io.h>
#include <lib.h>
#include <alloc.h>
#include <vec.h>
#include <locks.h>
#include <proc/regs.h>
#include <sys/int.h>
#include <mm/vmm.h>

/* Thread States */
#define TSTATE_STOPPED	0
#define TSTATE_RUNNING	1
#define TSTATE_IDLE		2
#define TSTATE_KILLABLE	3

struct thread_t {
    size_t tid;
    size_t ppid;
	size_t tpl;

	size_t cpu;
	size_t runtime;	// ms
	size_t state;
	
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

int cthread(struct thread_t* target, struct proc_t* parent, size_t tpl, size_t rip, size_t rsp);
int fork(struct proc_t* target, struct proc_t* parent);
int exec(struct thread_t* target);

void schedule(struct registers_t* regs);

void init_scheduler();

#endif