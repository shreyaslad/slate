#include <proc/scheduler.h>

#define T_STACK_SIZE 		1024

#define T_STATE_NOT_READY	0
#define T_STATE_READY		1
#define T_STATE_RUNNING		2

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

	struct vector_t* children;
	struct vector_t* threads;
};

extern void exec_regs(struct registers_t* regs);

static spinlock_t scheduler_lock;

static struct vector_t* processes;
static struct vector_t* threads;
static struct process_t* root;
static struct thread_t* idle_thread;

static struct process_t* cur_process;
static struct thread_t* cur_thread;

static struct bitmap_t* pid_bitmap;
static struct bitmap_t* tid_bitmap;

int kill(size_t tid) {
	if (tid >= threads->n)
		return 0;

	spinlock_lock(&scheduler_lock);

	struct thread_t* t_target = threads->items[tid - 1];
	struct process_t* p_target = processes->items[t_target->ppid - 1];

	bitmap_f(tid_bitmap, t_target->tid - 1, 1);

	kfree(t_target->fds);
	kfree(t_target->regdump);
	kfree(t_target);

	threads->items[tid - 1] = NULL;

	if (!(p_target->threads->n--)) {
		for (int i = 0; i < p_target->children->n; i++)
			vec_a(root->children, p_target->children->items[i]);

		bitmap_f(pid_bitmap, p_target->pid - 1, 1);
		kfree(p_target);
	}
	
	spinlock_release(&scheduler_lock);
	return tid;
}

int exec(size_t tid, size_t entry_point, size_t tpl) {
	if (tid >= threads->n)
		return 0;

	spinlock_lock(&scheduler_lock);

	struct thread_t* target = threads->items[tid - 1];
	target->regdump->rip = entry_point;
	
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

	target->state = T_STATE_READY;

	spinlock_release(&scheduler_lock);
	return tid;
}

int spawn(size_t ppid) {
	if (ppid >= processes->n)
		return 0;

	spinlock_lock(&scheduler_lock);

	struct thread_t* ret = kmalloc(sizeof(struct thread_t)) + HIGH_VMA;
	ret->tid = bitmap_a(tid_bitmap, 1) + 1;
	ret->ppid = ppid;
	ret->tpl = 0;
	ret->cpu = 0;
	ret->runtime = 0;
	ret->state = T_STATE_NOT_READY;
	ret->fds = kmalloc(sizeof(struct vector_t)) + HIGH_VMA;
	ret->regdump = kmalloc(sizeof(struct registers_t)) + HIGH_VMA;
	memset(ret->regdump, 0, sizeof(struct registers_t));
	ret->regdump->rflags = 0x282;

	ret->regdump->rsp = kmalloc(T_STACK_SIZE) + HIGH_VMA;

	vec_n(ret->fds);

	vec_a(threads, ret);

	struct process_t* parent = processes->items[ret->ppid - 1];
	vec_a(parent->children, ret);

	printf(KPRN_INFO, "sched: Created Thread %u\n", ret->tid);

	spinlock_release(&scheduler_lock);
	return ret->tid;
}

int fork(size_t ppid) {
	if (ppid >= processes->n)
		return 0;

	spinlock_lock(&scheduler_lock);

	struct process_t* parent = processes->items[ppid - 1];
	struct process_t* ret = kmalloc(sizeof(struct process_t)) + HIGH_VMA;

	ret->pid = bitmap_a(pid_bitmap, 1);
	ret->ppid = ppid;
	ret->runtime = 0;
	ret->children = kmalloc(sizeof(struct vector_t)) + HIGH_VMA;
	ret->threads = kmalloc(sizeof(struct vector_t)) + HIGH_VMA;
	// TODO: fork context

	vec_n(ret->threads);
	vec_n(ret->children);

	vec_a(parent->children, ret);

	spawn(ret->pid);

	spinlock_release(&scheduler_lock);
	return ret->pid;
}

__attribute__((noreturn))
static void idle() {
	while (1)
		printf(KPRN_INFO, "Hello from idle thread!\n");
}

static size_t pick_next(size_t tid) {
	if (tid == threads->n)
		return 1;

	return tid++;
}

__attribute__((noreturn))
void schedule(struct registers_t* regs) {
	printf(KPRN_INFO, "here\n");

	exec_regs(idle_thread->regdump);
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

	root->pid = bitmap_a(pid_bitmap, 1) + 1;
	root->ppid = 0;
	root->context = get_pml4();
	root->runtime = 0;
	root->children = kmalloc(sizeof(struct vector_t)) + HIGH_VMA;
	root->threads = kmalloc(sizeof(struct vector_t)) + HIGH_VMA;
	vec_n(root->children);
	vec_n(root->threads);

	idle_thread->tid = bitmap_a(tid_bitmap, 1) + 1;
	idle_thread->ppid = 1;
	idle_thread->tpl = 0;
	idle_thread->cpu = 0;
	idle_thread->runtime = 0;
	idle_thread->state = T_STATE_READY;
	idle_thread->fds = kmalloc(sizeof(struct vector_t));
	idle_thread->regdump->rip = (uint64_t)idle;
	idle_thread->regdump->rsp = (uint64_t)kmalloc(T_STACK_SIZE) + HIGH_VMA;
	idle_thread->regdump->cs = 0x8;
	idle_thread->regdump->ss = 0x10;
	idle_thread->regdump->rflags = 0x282;

	vec_n(idle_thread->fds);
	vec_a(root->threads, idle_thread);

	vec_a(processes, root);
	vec_a(threads, idle_thread);

	pid_bitmap = kmalloc(sizeof(struct bitmap_t));
	tid_bitmap = kmalloc(sizeof(struct bitmap_t));
	bitmap_n(pid_bitmap, 1);
	bitmap_n(tid_bitmap, 1);

	cur_thread = idle_thread;
	cur_process = root;

	register_handler(32, schedule);

	printf(KPRN_INFO, "sched: Initialized\n");

	asm volatile("sti");
}
