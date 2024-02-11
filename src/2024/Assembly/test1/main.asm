section .bss
    buf resb 6
    bufsize equ $-buf

section .text
    global _start
    extern _exit
    extern sample001_helloworld
    extern sample002_alphabet
    extern itoa
    extern puts

_start:
    call sample001_helloworld
    call sample002_alphabet

    mov rdi, 987654
    mov rsi, buf
    mov rdx, bufsize
    call itoa

    call puts

    mov rdi, rax
    call _exit
