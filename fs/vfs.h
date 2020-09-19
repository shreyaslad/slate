#ifndef __FS__VFS_H__
#define __FS__VFS_H__

#include <stdint.h>
#include <stddef.h>
#include <alloc.h>
#include <bitmap.h>
#include <mem.h>
#include <vec.h>
#include <str.h>

#define F_READ  0x1
#define F_WRITE 0x2
#define F_EXEC  0x4

struct vfs_node_t {
    size_t uuid;
    char* name;

    size_t (*open)(char*, size_t);
    size_t (*close)(size_t);
    size_t (*read)(size_t, void*, size_t);
    size_t (*write)(size_t, void*, size_t);
    size_t (*seek)(size_t, size_t);

    size_t permissions;

    struct vfs_node* parent;
    struct vector_t* children;
};

size_t create(char* path, char* filename, size_t permissions);

size_t open(char* path, size_t mode);
size_t close(size_t fd);
size_t read(size_t fd, void* buf, size_t n);
size_t write(size_t fd, void* buf, size_t n);
size_t seek(size_t fd, size_t n);

void init_vfs();

#endif
