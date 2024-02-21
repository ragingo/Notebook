section .data
    filename db '/home/ragingo/temp/a.txt', 0

section .text
    global sample006_file_write
    extern fopen
    extern sys_write
    extern strlen

sample006_file_write:
    push rbp
    mov rbp, rsp

    mov rdi, filename
    push 'w'
    mov rsi, rsp
    call fopen
    pop r8

    mov r8, rax ; fd

    mov rdi, filename
    call strlen

    mov rdi, r8
    mov rsi, filename
    mov rdx, rax
    call sys_write

    leave
    ret
