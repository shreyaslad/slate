# Kernel Replacement Specification

### Theory

The operating system will be split into three parts:
1. Bootloader
2. Bootstrap Kernel
3. Main Kernel

When the system starts, the bootloader will load and pass control to the Bootstrap Kernel. This kernel is responsible for bootstrapping memory and initializing basic drivers just enough to load the Main Kernel. Once it has finished, the Bootstrap Kernel will load and pass control over to the Main Kernel. The Main Kernel will have some method to understand when it should check for updates, which can either be scheduled or invoked directly by the user, or both. In the event that an update has been issued, the Main Kernel will create a new thread to determine the validity of an update (i.e. whether it is actually an update or an old image, and/or whether or not the new image is properly signed). If the thread determines that the new kernel image is valid, it will send a message to the Main Kernel, at which point all execution will be temporarily halted. The Main Kernel at this point will save the kernel and system state somewhere, and describe them in a series of tables. It will then return control back to the Bootstrap Kernel and pass the tables containing information about the current system state as well as the new kernel image. The Bootstrap Kernel will then load the new kernel image into memory and pass the tables describing the system state to it. The new Main Kernel will then take over control and restore the system state, then continue execution as normal. If the Main Kernel is loaded by the Bootstrap Kernel for the first time (i.e. the system just started up), the Bootstrap Kernel will pass NULL as the system state to the Main Kernel.

All code will be statically linked against the Bootstrap Kernel and dynamically linked against the Main Kernel.

### Tables

The following tables should be defined in order to properly convey the current system state. As features are added, this list of tables will grow.

Each table must contain a header that describes the table's signature (a SHA256 hash of the table's name) and it's length (in bytes). For sanity, each table header will contain the checksum of the rest of the table to ensure the data has not been corrupted or overwritten.
The header structure is as follows:

```c
struct sys_state_hdr {
    char ident[9]; // 8 byte null-terminated identifier
    uint64_t len;
    uint64_t chcksm;
} __attribute__((packed));
```

The structure of the following tables has not been finalized and will be updated as time goes on.

```c
struct sys_proc_tbl {
    struct process_t processes[PROC_MAX];

    // Additional information
} __attribute__((packed));
```

```c
enum fs {
    echfs = 0,
    ext2 = 1
};

struct sys_fs_tbl {
    uint8_t fs;

    struct fd_t fds[FD_MAX];
    struct fd_t open_fds[FD_MAX];

    // Additional information
} __attribute__((packed));
```

### Bootstrap Kernel
- COM1 Serial
- Disk
    - AHCI
    - FS
    - VFS
- ELF Loader

### Main Kernel
- Literally everything else