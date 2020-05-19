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

err_0:
    push qword 0
    push qword 0
    jmp err_common

err_1:
    push qword 0
    push qword 1
    jmp err_common

err_2:
    push qword 0
    push qword 2
    jmp err_common

err_3:
    push qword 0
    push qword 3
    jmp err_common

err_4:
    push qword 0
    push qword 4
    jmp err_common

err_5:
    push qword 0
    push qword 5
    jmp err_common

err_6:
    push qword 0
    push qword 6
    jmp err_common

err_7:
    push qword 0
    push qword 7
    jmp err_common

err_8:
    push qword 8
    jmp err_common

err_9:
    push qword 0
    push qword 9
    jmp err_common

err_10:
    push qword 10
    jmp err_common

err_11:
    push qword 11
    jmp err_common

err_12:
    push qword 12
    jmp err_common

err_13:
    push qword 13
    jmp err_common

err_14:
    push qword 14
    jmp err_common

err_15:
    push qword 0
    push qword 15
    jmp err_common

err_16:
    push qword 0
    push qword 16
    jmp err_common

err_17:
    push qword 0
    push qword 17
    jmp err_common

err_18:
    push qword 0
    push qword 18
    jmp err_common

err_19:
    push qword 0
    push qword 19
    jmp err_common

err_20:
    push qword 0
    push qword 20
    jmp err_common

err_21:
    push qword 0
    push qword 21
    jmp err_common

err_22:
    push qword 0
    push qword 22
    jmp err_common

err_23:
    push qword 0
    push qword 23
    jmp err_common

err_24:
    push qword 0
    push qword 24
    jmp err_common

err_25:
    push qword 0
    push qword 25
    jmp err_common

err_26:
    push qword 0
    push qword 26
    jmp err_common

err_27:
    push qword 0
    push qword 27
    jmp err_common

err_28:
    push qword 0
    push qword 28
    jmp err_common

err_29:
    push qword 0
    push qword 29
    jmp err_common

err_30:
    push qword 0
    push qword 30
    jmp err_common

err_31:
	push qword 0
	push qword 31
	jmp err_common

