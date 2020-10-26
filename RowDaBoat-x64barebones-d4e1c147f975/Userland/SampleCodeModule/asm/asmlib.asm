GLOBAL writeAsm
GLOBAL readAsm
GLOBAL getTime
GLOBAL getCpuData
GLOBAL readCpuModel
GLOBAL readmem
GLOBAL readRegisters
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
GLOBAL yieldAsm
GLOBAL pipeAsm
GLOBAL closeAsm
GLOBAL _listPipes
GLOBAL semPostAsm
GLOBAL semWaitAsm
GLOBAL semOpenAsm
GLOBAL semCloseAsm
GLOBAL _listSemaphores
GLOBAL waitAsm

section .text
    writeAsm:
        push rbp
        mov rbp, rsp
        push rbx
        push rcx
        mov rax, 1
        mov rbx, rdi
        mov rcx, rsi
        int 80h
        pop rcx
        pop rbx
        mov rsp, rbp
        pop rbp
        ret

    readAsm:
        push rbp
        mov rbp, rsp
        push rbx
        push rcx
        mov rax, 2
        mov rbx, rdi
        mov rcx, rsi
        int 80h
        pop rcx
        pop rbx
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

    getCpuTemp:
        push rbp
	    mov rbp, rsp
        mov rax, 7
        int 80h
	    mov rsp, rbp
	    pop rbp
	    ret
    
    getMalloc:
        push rbp
        mov rbp, rsp
        push rbx
        mov rax, 8
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
        mov rax, 9
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
        mov rax, 10
        mov rbx, rdi
        mov rcx, rsi
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
        mov rax, 11
        int 80h
        pop rax
        mov rsp, rbp
        pop rbp
        ret

    getPidAsm:
        push rbp
        mov rbp, rsp
        mov rax, 12
        int 80h
        mov rsp, rbp
        pop rbp
        ret

    changeProcessStateAsm:
        push rbp
        mov rbp, rsp
        push rbx
        mov rax, 13
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
	    mov rax, 14
	    int 80h
	    mov rsp, rbp
	    pop rbp
	    ret

    niceAsm:
	    push rbp
	    mov rbp, rsp
	    push rbx
        push rcx
	    mov rax, 15
        mov rbx, rdi
        mov rcx, rsi
	    int 80h
	    pop rcx
        pop rbx 
	    mov rsp, rbp
	    pop rbp
	    ret

    yieldAsm:
        push rbp
        mov rbp, rsp
        push rax
        mov rax, 16
        int 80h
        pop rax
        mov rsp, rbp
        pop rbp
        ret

    pipeAsm:
        push rbp
        mov rbp, rsp
        push rbx
        mov rax, 17
        mov rbx, rdi
        int 80h
        pop rbx
        mov rsp, rbp
        pop rbp
        ret

    closeAsm:
        push rbp
        mov rbp, rsp
        push rax
        push rbx
        mov rax, 18
        mov rbx, rdi
        int 80h
        pop rbx
        pop rax
        mov rsp, rbp
        pop rbp
        ret

    _listPipes:
        push rbp
        mov rbp, rsp
        mov rax, 19
        int 80h
        mov rsp, rbp
        pop rbp
        ret

    semPostAsm:
        push rbp
        mov rbp, rsp
        push rbx
        mov rax, 20
        mov rbx, rdi
        int 80h
        pop rbx
        mov rsp, rbp
        pop rbp
        ret

    semWaitAsm:
        push rbp
        mov rbp, rsp
        push rbx
        mov rax, 21
        mov rbx, rdi
        int 80h
        pop rbx
        mov rsp, rbp
        pop rbp
        ret

    semOpenAsm:
        push rbp
        mov rbp, rsp
        push rbx
        push rcx
        mov rax, 22
        mov rbx, rdi
        mov rcx, rsi
        int 80h
        pop rcx
        pop rbx
        mov rsp, rbp
        pop rbp
        ret

    semCloseAsm:
        push rbp
        mov rbp, rsp
        push rbx
        mov rax, 23
        mov rbx, rdi
        int 80h
        pop rbx
        mov rsp, rbp
        pop rbp
        ret

    _listSemaphores:
        push rbp
        mov rbp, rsp
        mov rax, 24
        int 80h
        mov rsp, rbp
        pop rbp
        ret

    waitAsm:
        push rbp
        mov rbp, rsp
        push rax
        push rbx
        mov rax, 25
        mov rbx, rdi
        int 80h
        pop rbx
        pop rax
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