#include <fs/vfs.h>

static struct vfs_node_t* root;

static spinlock_t vfs_lock;
static struct bitmap_t* uuid_bitmap;

size_t open(char* path, size_t mode) {
    ;
}

size_t close(size_t fd) {
    ;
}

size_t read(size_t fd, void* buf, size_t n) {
    ;
}

size_t write(size_t fd, void* buf, size_t n) {
    ;
}

size_t seek(size_t fd, size_t n) {
    ;
}

void init_vfs() {
    root = kmalloc(sizeof(struct vfs_node_t)) + HIGH_VMA;
    root->name = kmalloc(1);
    uuid_bitmap = kmalloc(sizeof(struct vfs_node_t)) + HIGH_VMA;
    
    bitmap_n(uuid_bitmap, 1);
    
    root->uuid = bitmap_a(uuid_bitmap, 1);
    strncpy(root->name, "/", 1);

    root->open = open;
    root->close = close;
    root->read = read;
    root->write = write;
    root->seek = seek;
}
