![alt text](slate.png "slate")

[![Build Status](https://travis-ci.org/404meemr/slate.svg?branch=master)](https://travis-ci.org/404meemr/slate)

### About
Slate is a simple, 64 bit x86 operating system that aims to implement some terrbile form of livepatching, where an image can be pulled from some source (some disk, some server) and then applied as the new kernel. 

### Theory

The operating system will be split into three parts:
1. Bootloader
2. Bootstrap Kernel
3. Main Kernel

When the system starts, the bootloader will load and pass control to the Bootstrap Kernel. This kernel is responsible for bootstrapping memor and initializing basic drivers just enough to load the Main Kernel. Once it has finished, the Bootstrap Kernel will load and pass control over to the Main Kernel. The Main Kernel will have some method to understand when it should check for updates, which can either be scheduled or invoked directly by the user, or both. In the event that an update has been issued, the Main Kernel will create a new thread to determine the validity of an update (i.e. whether it is actually an update or an old image, and/or whether or not the new image is properly signed). If the thread determines that the new kernel image is valid, it will send a message to the Main Kernel, at which point all execution will be temporarily halted. The Main Kernel at this point will save the kernel and system state somewhere, and describe them in a series of tables. It will then return control back to the Bootstrap Kernel and pass the tables containing information about the current system state as well as the new kernel image. The Bootstrap Kernel will then load the new kernel image into memory and pass the tables describing the system state to it. The new Main Kernel will then take over control and restore the system state, then continue execution as normal. If the Main Kernel is loaded by the Bootstrap Kernel for the first time (i.e. the system just started up), the Bootstrap Kernel will pass NULL as the system state to the Main Kernel.

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
