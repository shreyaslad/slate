#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <stdint.h>
#include <dynarray.h>

typedef struct thread {
    uint64_t        tid;
    uint64_t        ppid;
    registers_t*    regs;
} thread_t;

typedef struct process {
    uint64_t    pid;
    uint64_t    nthreads;
    uint64_t*   context;
} proc_t;

#endif
