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
- Scheduler

### Planned Features
These are in order of implementation.
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
- parted
- losetup
- mkfs
- make
- clang
- qemu

```
make [FS="ext2|echfs"] [-j<n>]
```
