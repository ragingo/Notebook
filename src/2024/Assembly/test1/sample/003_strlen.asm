%include "lib/include/stdio.inc"

section .text
    global sample003_strlen
    extern strlen

sample003_strlen:
    push rbp
    mov rbp, rsp
    sub rsp, 32

    mov al, 'a'
    mov rcx, 0

.sample003_strlen.loop:
    mov byte [rbp - 32 + rcx], al
    inc al
    inc rcx
    cmp rcx, 26
    jl .sample003_strlen.loop
    mov byte [rbp - 32 + rcx], NULL

    lea rdi, [rbp - 32]
    call strlen

    leave
    ret
