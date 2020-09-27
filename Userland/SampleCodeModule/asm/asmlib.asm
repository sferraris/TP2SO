GLOBAL write
GLOBAL read
GLOBAL getTime
GLOBAL putNum
GLOBAL getCpuData
GLOBAL readCpuModel
GLOBAL readmem
GLOBAL zero_exception_creator
GLOBAL invalid_opcode_creator
GLOBAL readRegisters
GLOBAL sendIp
GLOBAL getCpuTemp
GLOBAL getMalloc
GLOBAL getFree
GLOBAL createProcessAsm
GLOBAL exitAsm
GLOBAL getPidAsm
GLOBAL killProcessAsm
GLOBAL changeProcessStateAsm
GLOBAL listProcessesAsm
GLOBAL niceAsm

section .text
    write:
        push rbp
        mov rbp, rsp
        push rax
        push rbx
        push rcx
        mov rax, 1
        mov rbx, rdi
        mov rcx, rsi
        int 80h
        pop rcx
        pop rbx
        pop rax
        mov rsp, rbp
        pop rbp
        ret

    read:
        push rbp
        mov rbp, rsp
        mov rax, 2
        int 80h
        mov rsp, rbp
        pop rbp
        ret
    
    getTime:
        push rbp
        mov rbp, rsp
        push rbx
        mov rax, 3
        mov rbx, rdi
        int 80h
        pop rbx
        mov rsp, rbp
        pop rbp
        ret

    getCpuData:
	    push rbp
	    mov rbp, rsp
	    push rbx
        push rcx
	    mov rax, 4
        mov rbx, rdi
        mov rcx, rsi
	    int 80h
	    pop rcx
        pop rbx 
	    mov rsp, rbp
	    pop rbp
	    ret

    readCpuModel:
	    push rbp
	    mov rbp, rsp
	    mov rax, 5
	    int 80h
	    mov rsp, rbp
	    pop rbp
	    ret

    readRegisters:
	    push rbp
	    mov rbp, rsp
	    mov rax, 6
	    int 80h
	    mov rsp, rbp
	    pop rbp
	    ret
    
    sendIp:
        push rbp
        mov rbp, rsp
        push rax
        push rbx
        push rcx
        mov rax, 7
        mov rbx, [rbp + 8]
        mov rcx, rbp
        int 80h
        pop rcx
        pop rbx
        pop rax
        mov rsp, rbp
        pop rbp
        ret

    getCpuTemp:
        push rbp
	    mov rbp, rsp
        mov rax, 8
        int 80h
	    mov rsp, rbp
	    pop rbp
	    ret
    
    getMalloc:
        push rbp
        mov rbp, rsp
        push rbx
        mov rax, 9
        mov rbx, rdi
        int 80h
        pop rbx
        mov rsp, rbp
        pop rbp
        ret
    
    getFree:
        push rbp
        mov rbp, rsp
        push rax
        push rbx
        mov rax, 10
        mov rbx, rdi
        int 80h
        pop rbx
        pop rax
        mov rsp, rbp
        pop rbp
        ret

    createProcessAsm:
        push rbp
        mov rbp, rsp
        push rbx
        push rcx
        mov rax, 11
        mov rbx, rdi
        mov rcx,rsi
        int 80h
        pop rcx
        pop rbx
        mov rsp, rbp
        pop rbp
        ret

    exitAsm:
        push rbp
        mov rbp, rsp
        push rax
        mov rax, 12
        int 80h
        pop rax
        mov rsp, rbp
        pop rbp
        ret

    getPidAsm:
        push rbp
        mov rbp, rsp
        mov rax, 13
        int 80h
        mov rsp, rbp
        pop rbp
        ret
    
    
    changeProcessStateAsm:
        push rbp
        mov rbp, rsp
        push rbx
        mov rax, 14
        mov rbx, rdi
        mov rcx, rsi
        int 80h
        pop rbx
        mov rsp, rbp
        pop rbp
        ret
    
    listProcessesAsm:
        push rbp
	    mov rbp, rsp
	    mov rax, 15
	    int 80h
	    mov rsp, rbp
	    pop rbp
	    ret

    niceAsm:
	    push rbp
	    mov rbp, rsp
	    push rbx
        push rcx
	    mov rax, 16
        mov rbx, rdi
        mov rcx, rsi
	    int 80h
	    pop rcx
        pop rbx 
	    mov rsp, rbp
	    pop rbp
	    ret
        

    readmem:
        push rbp
        mov rbp, rsp
        mov rax, [rdi]
        mov [rsi], rax
        mov rax, [rdi + 8]
        mov [rsi + 8], rax
        mov rax, [rdi + 16]
        mov [rsi + 16], rax
        mov rax, [rdi + 24]
        mov [rsi + 24], rax
        mov rsp, rbp
        pop rbp
        ret

    zero_exception_creator:
        push rbp
	    mov rbp, rsp
	    mov rax, 0
        div rdx
	    mov rsp, rbp
	    pop rbp
	    ret

    invalid_opcode_creator:
        push rbp
	    mov rbp, rsp
	    UD2
	    mov rsp, rbp
	    pop rbp
	    ret