section .text
    global _start
    extern _exit
    extern sample001_helloworld
    extern sample002_alphabet
    extern putc

_start:
    call sample001_helloworld
    call sample002_alphabet

    mov rdi, 'A'
    mov rsi, 1
    call putc

    mov rdi, rax
    call _exit
