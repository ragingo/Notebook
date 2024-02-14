%include "lib/include/stdio.inc"

section .text
    global sample003_strlen
    extern strlen
    extern itoa

sample003_strlen:
    push rbp
    mov rbp, rsp

    sub rsp, 16
    mov byte [rbp - 4], 'a'
    mov byte [rbp - 3], 'b'
    mov byte [rbp - 2], 'c'
    mov byte [rbp - 1], 0

    lea rdi, [rbp - 4]
    call strlen

    add rsp, 16
    pop rbp
    ret
