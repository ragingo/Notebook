section .text
    global _start
    extern _exit
    extern sample001_helloworld
    extern sample002_alphabet

_start:
    call sample001_helloworld
    call sample002_alphabet

    xor rdi, rdi
    call _exit
