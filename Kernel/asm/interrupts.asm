GLOBAL _hlt
GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL saveRegisters
GLOBAL recoverExceptionRegisters
GLOBAL schedule_handler
GLOBAL createProcess_asm

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler
GLOBAL _irq80Handler

GLOBAL _exception0Handler
GLOBAL _exception6Handler

EXTERN irqDispatcher
EXTERN exceptionDispatcher
EXTERN syscallDispatcher
EXTERN schedule


SECTION .text

%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

%macro pushStateNoRax 0
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popStateNoRax 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
%endmacro

%macro irqHandlerMaster 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro



%macro exceptionHandler 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	call exceptionDispatcher

	popState
	iretq
%endmacro

%macro syscallHandler 3
	pushStateNoRax
	
	mov rdi, %1 ; pasaje nro de syscall
	mov rsi, %2 ; parametros de la syscall
	mov rdx, %3
	call syscallDispatcher
	push rax

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	pop rax

	popStateNoRax

	iretq
%endmacro

schedule_handler: ;<- RIP
	pushState

    mov rdi, rsp  ;gene + 1
    call schedule
    mov rsp, rax

    ;Send EOI
    mov al, 20h
    out 20h, al

    popState

    iretq  ;se vuelve a pisar el RIP

createProcess_asm: 

cli
mov rbp,rdi ;hago que rsp y rbp apunten a la direccion inmediatamente despues a la del stack del proceso (en push: primero se decrementa y despues se copia)
mov rsp,rbp


push 0x0 ;SS
push rbp ;RSP 
push 0x202 ;RFLAGS
push 0x8 ;CS
push rsi;RIP

;inicializo valores en forma ascendente para debugging

push 1 ;RAX
push 2 ;RBX
push 3 ;RCX
push 4 ;RDX
push 5 ;RBP
push 6 ;RDI ;argc
push 7 ;RSI ;argv
push 8 ;R8
push 9 ;R9
push 10 ;R10
push 11 ;R11
push 12 ;R12
push 13 ;R13
push 14 ;R14
push 15 ;R15

sti
hlt

ret

_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret


_sti:
	sti
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    retn


;8254 Timer (Timer Tick)
_irq00Handler:
	irqHandlerMaster 0

;Keyboard
_irq01Handler:
	irqHandlerMaster 1

;Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2

;Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3

;Serial Port 1 and 3
_irq04Handler:
	irqHandlerMaster 4

;USB
_irq05Handler:
	irqHandlerMaster 5


;Zero Division Exception
_exception0Handler:
	exceptionHandler 0
	
_exception6Handler:
	exceptionHandler 6

;SystemCall
_irq80Handler:
	syscallHandler rax, rbx, rcx 

saveRegisters:
	push rbp
	mov rbp, rsp
	mov rax, [rbp + 208]  
	mov [rdi], rax  
	mov rax, [rbp + 200]
	mov [rdi + 4], rax
	mov rax, [rbp + 192]  
	mov [rdi + 8], rax
	mov rax, [rbp + 184]  
	mov [rdi + 12], rax 
	mov rax, [rbp + 176]  
	mov [rdi + 16], rax 
	mov rax, [rbp + 168]  
	mov [rdi + 20], rax 
	mov rax, [rbp + 160]  
	mov [rdi + 24], rax 
	mov rax, [rbp + 152]  
	mov [rdi + 28], rax 
	mov rax, [rbp + 144]  
	mov [rdi + 32], rax 
	mov rax, [rbp + 136]  
	mov [rdi + 36], rax 
	mov rax, [rbp + 128]  
	mov [rdi + 40], rax 
	mov rax, [rbp + 120]  
	mov [rdi + 44], rax 
	mov rax, [rbp + 112]  
	mov [rdi + 48], rax 
	mov rax, [rbp + 104]  
	mov [rdi + 52], rax 
	mov rax, [rbp + 96]  
	mov [rdi + 56], rax
	mov rsp, rbp
	pop rbp
	ret

	recoverExceptionRegisters:
	push rbp
	mov rbp, rsp
	mov rax, [rbp + 8]
	mov [rdi], rax
	mov rax, rbp
	mov [rdi + 4], rax
	mov rax, [rbp + 8]  
	mov [rdi + 8], rax
	mov rax, [rbp + 264]  
	mov [rdi + 12], rax
	mov rax, [rbp + 256]  
	mov [rdi + 16], rax
	mov rax, [rbp + 248]  
	mov [rdi + 20], rax
	mov rax, [rbp + 240]  
	mov [rdi + 24], rax
	mov rax, [rbp + 232]  
	mov [rdi + 28], rax
	mov rax, [rbp + 224]  
	mov [rdi + 32], rax
	mov rax, [rbp + 216]  
	mov [rdi + 36], rax
	mov rax, [rbp + 208]  
	mov [rdi + 40], rax
	mov rax, [rbp + 200]  
	mov [rdi + 44], rax
	mov rax, [rbp + 192]  
	mov [rdi + 48], rax
	mov rax, [rbp + 184]  
	mov [rdi + 52], rax
	mov rax, [rbp + 176]  
	mov [rdi + 56], rax
	mov rax, [rbp + 168]  
	mov [rdi + 60], rax
	mov rax, [rbp + 160]  
	mov [rdi + 64], rax
	mov rsp, rbp
	pop rbp
	ret

haltcpu:
	cli
	hlt
	ret

SECTION .bss
	aux resq 1