#include <proc/scheduler.h>

static struct vector_t* procs;
static struct vector_t* threads;
static struct proc_t* root;
static struct thread_t* root_worker;

struct proc_t* cur_proc;
struct thread_t* cur_thread;

void schedule(struct registers_t* regs) {
	serial_printf(KPRN_INFO, "SCHEDULER", "here\n");
}

void idle() {
	while (1)
		asm volatile("");
}

void init_scheduler() {
	procs = kmalloc(sizeof(struct vector_t));
	threads = kmalloc(sizeof(struct vector_t));

	root = kmalloc(sizeof(struct proc_t));
	root_worker = kmalloc(sizeof(struct thread_t));

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

	root_worker->tid = 1;
	root_worker->ppid = 1;
	root_worker->tpl = 0;
	root_worker->cpu = 0;
	root_worker->runtime = 0;
	root_worker->state = TSTATE_IDLE;
	root_worker->regdump->rip = (uint64_t)idle;

	vec_a(root->children, root_worker);

	register_handler(32, schedule);
}