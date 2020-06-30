#include <proc/scheduler.h>

#define T_STACK_SIZE 8192

extern void exec_regs(struct registers_t* regs);

static spinlock_t scheduler_lock;

static struct vector_t* processes;
static struct vector_t* threads;
static struct process_t* root;
static struct thread_t* idle_thread;

struct process_t* cur_process;
struct thread_t* cur_thread;

static struct bitmap_t* pid_bitmap;
static struct bitmap_t* tid_bitmap;

int spawn(size_t ppid, size_t entry_point, size_t tpl) {
	spinlock_lock(&scheduler_lock);

	struct thread_t* target = kmalloc(sizeof(struct thread_t));
	target->tid = bitmap_a(tid_bitmap, 1);
	target->ppid = ppid;
	target->tpl = tpl;
	target->cpu = 0;
	target->runtime = 0;
	target->state = T_STATE_NOT_READY;
	target->fds = kmalloc(sizeof(struct vector_t));
	target->regdump = kmalloc(sizeof(struct registers_t));

	switch (tpl) {
		case 0:
			target->regdump->cs = 0x8;
			target->regdump->ss = 0x10;
			break;
		case 3:
			target->regdump->cs = 0x18;
			target->regdump->ss = 0x20;
			break;
	}

	target->regdump->rip = (uint64_t)entry_point;
	target->regdump->rsp = (uint64_t)kmalloc(T_STACK_SIZE) + HIGH_VMA;

	vec_n(target->fds);

	vec_a(threads, target);

	struct process_t* parent = processes->items[target->ppid - 1];
	vec_a(parent->children, target);

	spinlock_release(&scheduler_lock);
	return target->tid;
}

int fork(size_t tpid) {
	;
}

static size_t pick_next(size_t idx) {
	if (idx == threads->n - 1)
		return 0;

	return idx++;
}

void schedule(struct registers_t* regs) {
	/*cur_thread->regdump = regs;
	threads->items[cur_thread->tid - 1] = cur_thread;

	cur_thread = threads->items[pick_next(cur_thread->tid - 1)];
	cur_proc = processes->items[cur_thread->ppid - 1];

	exec_regs(cur_thread->regdump);*/
	;
}

void idle() {
	serial_printf(KPRN_INFO, "IDLE", "Hello from idle thread!\n");

	while (1)
		asm volatile("");
}

void init_scheduler() {
	processes = kmalloc(sizeof(struct vector_t));
	threads = kmalloc(sizeof(struct vector_t));

	root = kmalloc(sizeof(struct process_t));
	idle_thread = kmalloc(sizeof(struct thread_t));

	cur_process = kmalloc(sizeof(struct process_t));
	cur_thread = kmalloc(sizeof(struct thread_t));

	vec_n(processes);
	vec_n(threads);
	vec_n(cur_process->children);

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
	idle_thread->regdump->rsp = (uint64_t)kmalloc(T_STACK_SIZE) + HIGH_VMA;
	idle_thread->regdump->cs = 0x8;
	idle_thread->regdump->ss = 0x10;
	idle_thread->regdump->rflags = 0x282;

	vec_n(idle_thread->fds);
	vec_a(root->children, idle_thread);

	vec_a(processes, root);
	vec_a(threads, idle_thread);

	pid_bitmap = kmalloc(sizeof(struct bitmap_t));
	tid_bitmap = kmalloc(sizeof(struct bitmap_t));
	bitmap_n(pid_bitmap, 1);
	bitmap_n(tid_bitmap, 1);

	register_handler(32, schedule);

	serial_printf(KPRN_INFO, "SCHED", "Initialized Scheduler\n");

	exec_regs(idle_thread->regdump);
}
