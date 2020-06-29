#include <proc/scheduler.h>

#define T_STACK_SIZE 8192

extern void exec(struct registers_t* regs);

static spinlock_t scheduler_lock;

static struct vector_t* procs;
static struct vector_t* threads;
static struct proc_t* root;
static struct thread_t* idle_thread;

struct proc_t* cur_proc;
struct thread_t* cur_thread;

static uint64_t* pid_bitmap;
static uint64_t* tid_bitmap;

void schedule(struct registers_t* regs) {
	exec(idle_thread->regdump);
}

void idle() {
	serial_printf(KPRN_INFO, "IDLE", "Hello from idle thread!\n");
}

void init_scheduler() {
	procs = kmalloc(sizeof(struct vector_t));
	threads = kmalloc(sizeof(struct vector_t));

	root = kmalloc(sizeof(struct proc_t));
	idle_thread = kmalloc(sizeof(struct thread_t));

	cur_proc = kmalloc(sizeof(struct proc_t));
	cur_thread = kmalloc(sizeof(struct thread_t));

	vec_n(procs);
	vec_n(threads);
	vec_n(cur_proc->children);

	strncpy(root->name, "root", 50);
	root->pid = 1;
	root->ppid = 0;
	root->context = get_pml4();
	root->runtime = 0;
	root->parent = NULL;
	vec_n(root->children);

	idle_thread->tid = 1;
	idle_thread->ppid = 1;
	idle_thread->tpl = 0;
	idle_thread->cpu = 0;
	idle_thread->runtime = 0;
	idle_thread->state = T_STATE_IDLE;
	idle_thread->fds = kmalloc(sizeof(struct vector_t));
	idle_thread->regdump->rip = (uint64_t)idle;
	idle_thread->regdump->rsp = kmalloc(T_STACK_SIZE) + HIGH_VMA;
	idle_thread->regdump->cs = 0x8;
	idle_thread->regdump->ss = 0x10;
	idle_thread->regdump->rflags = 0x282;

	vec_n(idle_thread->fds);
	vec_a(root->children, idle_thread);

	vec_a(procs, root);
	vec_a(threads, idle_thread);

	register_handler(32, schedule);

	serial_printf(KPRN_INFO, "SCHED", "Initialized Scheduler\n");
}
