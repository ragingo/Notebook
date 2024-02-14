section .text
    global _start
    extern _exit
    extern sample001_helloworld
    extern sample002_alphabet
    extern sample003_strlen

_start:
    call sample001_helloworld
    call sample002_alphabet
    call sample003_strlen ; peda で rax の値を確認

    mov rdi, rax
    call _exit
