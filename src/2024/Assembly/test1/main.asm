section .text
    global _start
    extern _exit
    extern sample001_helloworld
    extern sample002_alphabet
    extern digits

_start:
    call sample001_helloworld
    call sample002_alphabet

    mov rdi, 123456789
    call digits

    mov rdi, rax
    call _exit
