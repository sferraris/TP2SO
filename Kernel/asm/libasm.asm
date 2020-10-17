GLOBAL cpuVendor
GLOBAL cpuModel
GLOBAL cpuBrand
GLOBAL rebootProgram
GLOBAL getCpuTemp
GLOBAL readRTC
GLOBAL getKey

section .text
	
    cpuVendor:
	    push rbp
	    mov rbp, rsp
	    push rbx
        push rcx
        push rdx
	    mov rax, 0
	    cpuid
	    mov [rdi], ebx 
	    mov [rdi + 4], edx 
	    mov [rdi + 8], ecx
	    mov byte [rdi+13], 0
	    mov rax, rdi
        pop rdx
        pop rcx
	    pop rbx
	    mov rsp, rbp
	    pop rbp
	    ret

    cpuModel:
	    push rbp
	    mov rbp, rsp
        push rbx
        push rcx
	    mov rax, 1
	    cpuid
        mov rbx, rax
        and rbx, 0x00000000000F0000 ;extended model id
        shr rbx, 12
        mov rcx, rax
        and rcx, 0x00000000000000F0 ;model id
        shr rcx, 4
        add rbx, rcx
        mov rax, rbx
        pop rcx
        pop rbx
	    mov rsp, rbp
	    pop rbp
	    ret
    
    cpuBrand:
        push rbp
	    mov rbp, rsp
        push rbx
        push rcx
        push rdx
        mov rax, 0x80000000
        cpuid
        cmp eax, 0x80000004
        jb noSup
        mov eax, 0x80000002
        cpuid
        call saveStr
        mov eax, 0x80000003
        cpuid
        call saveStr
        mov eax, 0x80000004
        cpuid
        call saveStr
        sub rdi, 48

        noSup:
		mov rax, rdi
        pop rdx
        pop rcx
        pop rbx
        mov rsp, rbp
	    pop rbp
        ret

        saveStr:
        mov [rdi],eax
        mov [rdi+4],ebx
        mov [rdi+8],ecx
        mov [rdi+12],edx
        add rdi, 16
        ret

    rebootProgram:
        mov [rbp + 8], rdi
        mov rbp, rsi
        ret

    getCpuTemp:
        push rbp
	    mov rbp, rsp
        push rbx
	    mov rax, 0x00690A00
        mov rbx, rax
        and rax, 0x00FF0000
        shr rax, 16
        and rbx, 0x3F000000
        shr rbx, 24
        add rbx, rax
        mov rax, 0x88370000
        and rax, 0x00FF0000
        shr rax, 16
        sub rbx, rax
        mov rax, rbx
        pop rbx
	    mov rsp, rbp
	    pop rbp
	    ret

    readRTC:
        push rbp
        mov rbp,rsp
        mov rax,rdi
        out 70h,al
        mov rax,0
        in al,71h
        mov rsp,rbp
        pop rbp
        ret
    
    getKey:
        push rbp
        mov rbp, rsp
        mov rax,0

        loop:    
        in al,0x64       
        mov cl,al
        and al,0x01       
        cmp al,0
        je loop
        in al,0x60
        mov rsp, rbp
        pop rbp
        ret

  