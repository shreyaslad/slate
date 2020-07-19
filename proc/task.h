#ifndef __PROC__TASK_H__
#define __PROC__TASK_H__

#include <stdint.h>
#include <stddef.h>
#include <proc/regs.h>

typedef size_t tid_t;
typedef size_t pid_t;

tid_t task_tcreate(pid_t ppid);
pid_t task_pcreate();
int task_tkill(pid_t ppid, tid_t tid);
int task_tpause(pid_t pid, tid_t tid);
int task_tresume(pid_t pid, tid_t tid);

int kill(pid_t pid, int exit);

void schedule(struct regs_t* regs);

void init_scheduler();

#endif
