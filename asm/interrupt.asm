[extern int_handler]
[extern err_handler]

%macro pushaq 0
    push rax
    push rbx
    push rcx
    push rdx
    push rbp
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
%endmacro

%macro popaq 0
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rbp
    pop rdx
    pop rcx
    pop rbx
    pop rax
%endmacro

int_common:
	pushaq
	cld
	xor rax, rax
	mov rdi, rsp
	call int_handler
	popaq
	add rsp, 16
	iretq

err_common:
	pushaq
	cld
	xor rax, rax
	mov rdi, rsp
	call err_handler
	popaq
	add rsp, 16
	iretq

global err_0
global err_1
global err_2
global err_3
global err_4
global err_5
global err_6
global err_7
global err_8
global err_9
global err_10
global err_11
global err_12
global err_13
global err_14
global err_15
global err_16
global err_17
global err_18
global err_19
global err_20
global err_21
global err_22
global err_23
global err_24
global err_25
global err_26
global err_27
global err_28
global err_29
global err_30
global err_31

global int_0

err_0:
    push 0
    push 0
    jmp err_common

err_1:
    push 0
    push 1
    jmp err_common

err_2:
    push 0
    push 2
    jmp err_common

err_3:
    push 0
    push 3
    jmp err_common

err_4:
    push 0
    push 4
    jmp err_common

err_5:
    push 0
    push 5
    jmp err_common

err_6:
    push 0
    push 6
    jmp err_common

err_7:
    push 0
    push 7
    jmp err_common

err_8:
    push 8
    jmp err_common

err_9:
    push 0
    push 9
    jmp err_common

err_10:
    push 10
    jmp err_common

err_11:
    push 11
    jmp err_common

err_12:
    push 12
    jmp err_common

err_13:
	mov rax, 'a'
	mov [0xb8000], rax

    push 13
    jmp err_common

err_14:
    push 14
    jmp err_common

err_15:
    push 0
    push 15
    jmp err_common

err_16:
    push 0
    push 16
    jmp err_common

err_17:
    push 0
    push 17
    jmp err_common

err_18:
    push 0
    push 18
    jmp err_common

err_19:
    push 0
    push 19
    jmp err_common

err_20:
    push 0
    push 20
    jmp err_common

err_21:
    push 0
    push 21
    jmp err_common

err_22:
    push 0
    push 22
    jmp err_common

err_23:
    push 0
    push 23
    jmp err_common

err_24:
    push 0
    push 24
    jmp err_common

err_25:
    push 0
    push 25
    jmp err_common

err_26:
    push 0
    push 26
    jmp err_common

err_27:
    push 0
    push 27
    jmp err_common

err_28:
    push 0
    push 28
    jmp err_common

err_29:
    push 0
    push 29
    jmp err_common

err_30:
    push 0
    push 30
    jmp err_common

err_31:
	push 0
	push 31
	jmp err_common

int_0:
	push 0
	push 0