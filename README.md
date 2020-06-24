![alt text](slate.png "slate")

[![Build Status](https://travis-ci.org/404meemr/slate.svg?branch=master)](https://travis-ci.org/404meemr/slate)

### Features
- 64 bit Higher Half Long Mode
- Memory Management
- COM1 Serial
- VESA
- APICs
- HPET
- LAPIC Timer

### Planned Features
These are in order of implementation.
- Scheduler
- SMP
- Syscalls
- Userspace
- PCI
- Disk
    - VFS
    - AHCI
    - EXT2/4
- Binary and ELF loading
- mlibc

### Building and Running

Prerequisites:
- nasm
- make
- clang
- qemu

```
make [FS="ext2|ext4|echfs"] [-j<n>]
```
