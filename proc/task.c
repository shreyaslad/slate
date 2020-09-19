#include <stdint.h>
#include <stddef.h>
#include <proc/task.h>
#include <alloc.h>
#include <assert.h>
#include <bitmap.h>
#include <io.h>
#include <vec.h>
#include <locks.h>
#include <proc/regs.h>
#include <sys/interrupts.h>
#include <mm/vmm.h>
#include <drivers/apic.h>
#include <lib/rbtree.h>

#define T_STACK_SIZE        1024

#define T_STATE_NOT_READY   0
#define T_STATE_READY       1
#define T_STATE_RUNNING     2

/**
 * THEORY
 * ------
 * Every process is a node in an rb tree.
 * Statndard CFS stuff applies.
 * 
 * The scheduler picks a process that needs to be run
 * and then the process picks the next thread that needs to
 * be run.
 */

struct thread_t {
    tid_t tid;
    pid_t ppid;
    size_t tpl;

    size_t cpu;
    size_t state;

    struct vector_t* fds;
    struct regs_t ctx;
};

struct process_t {
    struct rb_node_t base;

    pid_t pid;
    pid_t ppid;

    char* cwd;
    
    size_t* pml4;
    size_t* stack;

    struct vector_t* children;
    struct vector_t* threads;
    size_t l_tid_idx;

    size_t vruntime;
    size_t static_prio, rt_prio, prio;
};

extern void exec_regs(struct regs_t* regs);

static spinlock_t scheduler_lock;

struct rb_root_t* root;

static struct bitmap_t* pid_bitmap;
static struct bitmap_t* tid_bitmap;

static struct regs_t default_krnl = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0x08,0x202,0,0x10};
static struct regs_t default_usr = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0x18,0x202,0,0x20};

void init_scheduler() {
    root = kmalloc(sizeof(struct rb_root_t));

    pid_bitmap = kmalloc(sizeof(struct bitmap_t));
    tid_bitmap = kmalloc(sizeof(struct bitmap_t));

    bitmap_n(pid_bitmap, 1);
    bitmap_n(tid_bitmap, 1);
}
