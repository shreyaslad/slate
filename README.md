![alt text](slate.png "slate")

[![Build Status](https://travis-ci.org/404meemr/slate.svg?branch=master)](https://travis-ci.org/404meemr/slate)

### Features
- Long mode
- Memory Management
- COM1 Serial

### Planned Features
These are in order of implementation.
- Scheduler
- LAPIC
- SMP
- Syscalls
- PCI
- Disk
    - AHCI
    - VFS
    - EXT2/4
- Binary and ELF loading
- Userspace
- VESA

### Building and Running

Prerequisites:
- nasm
- make
- mkfs
- clang

```
sudo make all FS="ext2" -j6
```

### Contributing

Refer to CODESTYLE.md to find the preferred codestyle for this project (TODO)
