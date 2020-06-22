[extern irq_handler]
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

irq_common:
	pushaq
	cld
	xor rax, rax
	mov rdi, rsp
	call irq_handler
	popaq
	add rsp, 16
	sti
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
; IRQs
global irq_0
global irq_1
global irq_2
global irq_3
global irq_4
global irq_5
global irq_6
global irq_7
global irq_8
global irq_9
global irq_10
global irq_11
global irq_12
global irq_13
global irq_14
global irq_15

; 0: Divide By Zero Exception
err_0:
    cli
    push byte 0
    push byte 0
    jmp err_common

; 1: Debug Exception
err_1:
    cli
    push byte 0
    push byte 1
    jmp err_common

; 2: Non Maskable Interrupt Exception
err_2:
    cli
    push byte 0
    push byte 2
    jmp err_common

; 3: Int 3 Exception
err_3:
    cli
    push byte 0
    push byte 3
    jmp err_common

; 4: INTO Exception
err_4:
    cli
    push byte 0
    push byte 4
    jmp err_common

; 5: Out of Bounds Exception
err_5:
    cli
    push byte 0
    push byte 5
    jmp err_common

; 6: Invalid Opcode Exception
err_6:
    cli
    push byte 0
    push byte 6
    jmp err_common

; 7: Coprocessor Not Available Exception
err_7:
    cli
    push byte 0
    push byte 7
    jmp err_common

; 8: Double Fault Exception (With Error Code!)
err_8:
    cli
    push byte 8
    jmp err_common

; 9: Coprocessor Segment Overrun Exception
err_9:
    cli
    push byte 0
    push byte 9
    jmp err_common

; 10: Bad TSS Exception (With Error Code!)
err_10:
    cli
    push byte 10
    jmp err_common

; 11: Segment Not Present Exception (With Error Code!)
err_11:
    cli
    push byte 11
    jmp err_common

; 12: Stack Fault Exception (With Error Code!)
err_12:
    cli
    push byte 12
    jmp err_common

; 13: General Protection Fault Exception (With Error Code!)
err_13:
    cli
    push byte 13
    jmp err_common

; 14: Page Fault Exception (With Error Code!)
err_14:
    cli
    push byte 14
    jmp err_common

; 15: Reserved Exception
err_15:
    cli
    push byte 0
    push byte 15
    jmp err_common

; 16: Floating Point Exception
err_16:
    cli
    push byte 0
    push byte 16
    jmp err_common

; 17: Alignment Check Exception
err_17:
    cli
    push byte 0
    push byte 17
    jmp err_common

; 18: Machine Check Exception
err_18:
    cli
    push byte 0
    push byte 18
    jmp err_common

; 19: Reserved
err_19:
    cli
    push byte 0
    push byte 19
    jmp err_common

; 20: Reserved
err_20:
    cli
    push byte 0
    push byte 20
    jmp err_common

; 21: Reserved
err_21:
    cli
    push byte 0
    push byte 21
    jmp err_common

; 22: Reserved
err_22:
    cli
    push byte 0
    push byte 22
    jmp err_common

; 23: Reserved
err_23:
    cli
    push byte 0
    push byte 23
    jmp err_common

; 24: Reserved
err_24:
    cli
    push byte 0
    push byte 24
    jmp err_common

; 25: Reserved
err_25:
    cli
    push byte 0
    push byte 25
    jmp err_common

; 26: Reserved
err_26:
    cli
    push byte 0
    push byte 26
    jmp err_common

; 27: Reserved
err_27:
    cli
    push byte 0
    push byte 27
    jmp err_common

; 28: Reserved
err_28:
    cli
    push byte 0
    push byte 28
    jmp err_common

; 29: Reserved
err_29:
    cli
    push byte 0
    push byte 29
    jmp err_common

; 30: Reserved
err_30:
    cli
    push byte 0
    push byte 30
    jmp err_common

; 31: Reserved
err_31:
    cli
    push byte 0
    push byte 31
    jmp err_common

; IRQ handlers
irq_0:
	cli
	push byte 0
	push byte 32
	jmp irq_common

irq_1:
	cli
	push byte 1
	push byte 33
	jmp irq_common

irq_2:
	cli
	push byte 2
	push byte 34
	jmp irq_common

irq_3:
	cli
	push byte 3
	push byte 35
	jmp irq_common

irq_4:
	cli
	push byte 4
	push byte 36
	jmp irq_common

irq_5:
	cli
	push byte 5
	push byte 37
	jmp irq_common

irq_6:
	cli
	push byte 6
	push byte 38
	jmp irq_common

irq_7:
	cli
	push byte 7
	push byte 39
	jmp irq_common

irq_8:
	cli
	push byte 8
	push byte 40
	jmp irq_common

irq_9:
	cli
	push byte 9
	push byte 41
	jmp irq_common

irq_10:
	cli
	push byte 10
	push byte 42
	jmp irq_common

irq_11:
	cli
	push byte 11
	push byte 43
	jmp irq_common

irq_12:
	cli
	push byte 12
	push byte 44
	jmp irq_common

irq_13:
	cli
	push byte 13
	push byte 45
	jmp irq_common

irq_14:
	cli
	push byte 14
	push byte 46
	jmp irq_common

irq_15:
	cli
	push byte 15
	push byte 47
	jmp irq_common