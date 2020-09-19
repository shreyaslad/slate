#include <fs/fd.h>

static struct vector_t* fds;

size_t fd_open(struct vfs_node_t* target, size_t mode) {
    if (!target)
        return 0;

    spinlock_lock(&fds->lock);

    struct fd_t* ret = kmalloc(sizeof(struct fd_t)) + HIGH_VMA;
    ret->node = target;
    ret->mode = mode;
    ret->seek = 0;

    vec_a(fds, ret);

    spinlock_release(&fds->lock);

    return 0;
}

size_t fd_close(size_t fd) {
    if (fd >= (fds->n - 1) || fds->items[fd] == NULL)
        return 0;

    spinlock_lock(&fds->lock);

    fds->items[fd] = NULL;

    spinlock_release(&fds->lock);
    return 1;
}

size_t fd_seek(size_t fd, size_t pos) {
    spinlock_lock(&fds->lock);

    ((struct fd_t *)fds->items[fd])->seek = pos;
    
    spinlock_release(&fds->lock);
    return 0;
}

void init_fds() {
    fds = kmalloc(sizeof(struct vector_t)) + HIGH_VMA;
    vec_n(fds);
}
