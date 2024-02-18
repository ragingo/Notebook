%include "lib/include/ascii.inc"

section .text
    global sample005_kuku
    extern putchar
    extern print_number

sample005_kuku:
    push rbp
    mov rbp, rsp
    sub rsp, 16

    xor r8, r8  ; row

.loop.row:
    inc r8
    xor r9, r9  ; col
.loop.col:
    inc r9
    push r8
    push r9

    mov rax, r8
    mul r9

    mov rdi, rax
    call print_number

    pop r9
    pop r8
    cmp r9, 9
    jl .loop.col

    push r8
    mov rdi, ASCII_LF
    call putchar
    pop r8

    cmp r8, 9
    jl .loop.row

    leave
    ret
