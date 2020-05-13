![alt text](slate.png "slate")

[![Build Status](https://travis-ci.org/404meemr/slate.svg?branch=master)](https://travis-ci.org/404meemr/slate)

### Features
- Long mode
- Memory Management
- COM1 Serial

### Planned Features
These are in order of implementation.
- Scheduler
- APICs
- SMP
- Userspace
- Syscalls
- PCI
- Disk
    - VFS
    - AHCI
    - EXT2/4
- Binary and ELF loading
- VESA

### Building and Running

Prerequisites:
- nasm
- make
- clang
- qemu

```
sudo make [FS="ext2|ext4|echfs"] [-j6]
```
