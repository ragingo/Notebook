section .text
    global sample004_itoa
    extern itoa
    extern puts

sample004_itoa:
    push rbp
    mov rbp, rsp
    sub rsp, 32

    mov rdi, 1234
    lea rsi, [rbp - 16]
    mov rdx, 5
    call itoa

    mov rdi, rsi
    call puts

    leave
    ret
