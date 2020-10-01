![alt text](slate.png "slate")

[![Build Status](https://travis-ci.org/404meemr/slate.svg?branch=master)](https://travis-ci.org/404meemr/slate)

### About
slate is a 64 bit operating system. Eventually, I plan to make it fully modular - the kernel only contains the barebones code necessary for bootstrapping the system, and all other drivers are rpovided as kernel modules.

### Features
- 64 bit Higher Half Long Mode
- Memory Management
- COM1 Serial
- VESA
- ACPI
- APICs
- HPET
- LAPIC Timer
- PCI
- UBSAN
- SMP
- Disk
    - VFS

### Planned Features
These are in order of implementation.
- Disk
    - AHCI
    - EXT2
- Scheduler (CFS)
- Syscalls
- Binary and ELF loading
- Userspace
- mlibc
- Intel HD Audio

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
