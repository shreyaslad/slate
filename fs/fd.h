#ifndef __FD_H__
#define __FD_H__

#include <stddef.h>
#include <alloc.h>
#include <mem.h>
#include <locks.h>
#include <vec.h>
#include <fs/vfs.h>

struct fd_t {
    struct vfs_node_t* node;
    size_t mode;
    size_t seek;
};

size_t fd_open(struct vfs_node_t* target, size_t mode);
size_t fd_close(size_t fd);
size_t fd_seek(size_t fd, size_t pos);

void init_fds();

#endif
