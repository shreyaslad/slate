[extern isr_handler]
[extern irq_handler]

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

isr_common:
    pushaq
    cld
    xor rax, rax
    mov rdi, rsp
    call isr_handler
    popaq
    ; Remove the error code and interrupt number
    add rsp, 16
    iretq

irq_common:
    pushaq
    cld
    xor rax, rax
    mov rdi, rsp
    call irq_handler
    popaq
    ; Remove the error code and interrupt number
    add rsp, 16
    iretq

global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31
; IRQs
global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15

; 0: Divide By Zero Exception
isr0:
    push qword 0
    push qword 0
    jmp isr_common

; 1: Debug Exception
isr1:
    push qword 0
    push qword 1
    jmp isr_common

; 2: Non Maskable Interrupt Exception
isr2:
    push qword 0
    push qword 2
    jmp isr_common

; 3: Int 3 Exception
isr3:
    push qword 0
    push qword 3
    jmp isr_common

; 4: INTO Exception
isr4:
    push qword 0
    push qword 4
    jmp isr_common

; 5: Out of Bounds Exception
isr5:
    push qword 0
    push qword 5
    jmp isr_common

; 6: Invalid Opcode Exception
isr6:
    push qword 0
    push qword 6
    jmp isr_common

; 7: Coprocessor Not Available Exception
isr7:
    push qword 0
    push qword 7
    jmp isr_common

; 8: Double Fault Exception (With Error Code!)
isr8:
    push qword 8
    jmp isr_common

; 9: Coprocessor Segment Overrun Exception
isr9:
    push qword 0
    push qword 9
    jmp isr_common

; 10: Bad TSS Exception (With Error Code!)
isr10:
    push qword 10
    jmp isr_common

; 11: Segment Not Present Exception (With Error Code!)
isr11:
    push qword 11
    jmp isr_common

; 12: Stack Fault Exception (With Error Code!)
isr12:
    push qword 12
    jmp isr_common

; 13: General Protection Fault Exception (With Error Code!)
isr13:
    push qword 13
    jmp isr_common

; 14: Page Fault Exception (With Error Code!)
isr14:
    push qword 14
    jmp isr_common

; 15: Reserved Exception
isr15:
    push qword 0
    push qword 15
    jmp isr_common

; 16: Floating Point Exception
isr16:
    push qword 0
    push qword 16
    jmp isr_common

; 17: Alignment Check Exception
isr17:
    push qword 0
    push qword 17
    jmp isr_common

; 18: Machine Check Exception
isr18:
    push qword 0
    push qword 18
    jmp isr_common

; 19: Reserved
isr19:
    push qword 0
    push qword 19
    jmp isr_common

; 20: Reserved
isr20:
    push qword 0
    push qword 20
    jmp isr_common

; 21: Reserved
isr21:
    push qword 0
    push qword 21
    jmp isr_common

; 22: Reserved
isr22:
    push qword 0
    push qword 22
    jmp isr_common

; 23: Reserved
isr23:
    push qword 0
    push qword 23
    jmp isr_common

; 24: Reserved
isr24:
    push qword 0
    push qword 24
    jmp isr_common

; 25: Reserved
isr25:
    push qword 0
    push qword 25
    jmp isr_common

; 26: Reserved
isr26:
    push qword 0
    push qword 26
    jmp isr_common

; 27: Reserved
isr27:
    push qword 0
    push qword 27
    jmp isr_common

; 28: Reserved
isr28:
    push qword 0
    push qword 28
    jmp isr_common

; 29: Reserved
isr29:
    push qword 0
    push qword 29
    jmp isr_common

; 30: Reserved
isr30:
    push qword 0
    push qword 30
    jmp isr_common

; 31: Reserved
isr31:
    push qword 0
    push qword 31
    jmp isr_common

; IRQ handlers
irq0:
	push qword 0
	push qword 32
	jmp irq_common

irq1:
	push qword 1
	push qword 33
	jmp irq_common

irq2:
	push qword 2
	push qword 34
	jmp irq_common

irq3:
	push qword 3
	push qword 35
	jmp irq_common

irq4:
	push qword 4
	push qword 36
	jmp irq_common

irq5:
	push qword 5
	push qword 37
	jmp irq_common

irq6:
	push qword 6
	push qword 38
	jmp irq_common

irq7:
	push qword 7
	push qword 39
	jmp irq_common

irq8:
	push qword 8
	push qword 40
	jmp irq_common

irq9:
	push qword 9
	push qword 41
	jmp irq_common

irq10:
	push qword 10
	push qword 42
	jmp irq_common

irq11:
	push qword 11
	push qword 43
	jmp irq_common

irq12:
	push qword 12
	push qword 44
	jmp irq_common

irq13:
	push qword 13
	push qword 45
	jmp irq_common

irq14:
	push qword 14
	push qword 46
	jmp irq_common

irq15:
	push qword 15
	push qword 47
	jmp irq_common
