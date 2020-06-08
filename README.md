![alt text](slate.png "slate")

[![Build Status](https://travis-ci.org/404meemr/slate.svg?branch=master)](https://travis-ci.org/404meemr/slate)

### Features
- 64 bit Higher Half Long Mode
- Memory Management
- COM1 Serial
- VESA
- APICs

### Planned Features
These are in order of implementation.
- HPET
- Scheduler
- SMP
- Userspace
- Syscalls
- PCI
- Disk
    - VFS
    - AHCI
    - EXT2/4
- Binary and ELF loading

### Building and Running

Prerequisites:
- nasm
- make
- clang
- qemu

```
sudo make [FS="ext2|ext4|echfs"] [-j<n>]
```
