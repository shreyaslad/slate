[bits 64]
[extern kmain]

%define KNL_HIGH_VMA 0xFFFFFFFF80000000

section .stivalehdr

stivale_header:
	dq stack.top
	dw 1
	dw 0
	dw 0
	dw 0
	dq 0

section .data

	gdt:                               ; Global Descriptor Table (64-bit).
	  .null: equ $ - gdt                ; The null descriptor.
		dw 0xFFFF						; Limit (low).
		dw 0							; Base (low).
		db 0							; Base (middle)
		db 0							; Access.
		db 0							; Granularity.
		db 0							; Base (high).
	  .code: equ $ - gdt               ; The code descriptor.
		dw 0xFFFF						; Limit (low).
		dw 0							; Base (low).
		db 0							; Base (middle)
		db 10011010b					; Access (exec/read).
		db 10101111b					; Granularity, 64 bits flag, limit19:16.
		db 0							; Base (high).
	  .data: equ $ - gdt               ; The data descriptor.
		dw 0xFFFF						; Limit (low).
		dw 0							; Base (low).
		db 0							; Base (middle)
		db 10010010b					; Access (read/write).
		db 00000000b					; Granularity.
		db 0							; Base (high).
	  .usercode: equ $ - gdt           ; The userspace code descriptor
		dw 0xFFFF						; Limit (low).
		dw 0                            ; Base (low).
		db 0							; Base (middle)
		db 11111101b					; Access (exec/read).
		db 10101111b					; Granularity, 64 bits flag, limit19:16.
		db 0                            ; Base (high).
	  .userdata: equ $ - gdt           ; The userspace data descriptor
		dw 0xFFFF						; Limit (low).
		dw 0							; Base (low).
		db 0							; Base (middle)
		db 11110011b					; Access (read/write).
		db 11001111b					; Granularity.
		db 0                            ; Base (high).
	  .pointer:							; The GDT-pointer.
		dw $ - gdt - 1					; Limit.
		dq gdt							; Base.
	  .pointer32:						; The GDT-pointer for 32 bit mode.
		dw $ - gdt - 1					; Limit.
		dd gdt - KNL_HIGH_VMA			; Base.

section .bss

	stack:
		resb 4096
	  .top:

section .text

	global _start
	_start:
		lgdt [gdt.pointer]

		push 0x10
		push rsp
		pushf
		push 0x8
		push _startup64
		iretq

	_startup64:
		mov ax, 0x10
		mov ds, ax
		mov es, ax
		mov ss, ax
		mov ax, 0x20
		mov gs, ax
		mov fs, ax

		call kmain
