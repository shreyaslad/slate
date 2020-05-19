#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <stdint.h>
#include <dynarray.h>
#include <proc/regs.h>

typedef struct thread {
    uint64_t tid;
    uint64_t ppid;
    registers_t* regs;

    char* children;
} thread_t;

typedef struct process {
    uint64_t pid;
    uint64_t nthreads;
    uint64_t* context;

    struct process* parent;
    char* children;
} proc_t;

int fork_thread(thread_t* parent, thread_t* child);
int fork_process(proc_t* parent, proc_t* child);

#endif
