section .text
    global _start
    extern _exit
    extern sample001_helloworld
    extern sample002_alphabet
    extern sample003_strlen
    extern sample004_itoa

_start:
    push rbp
    mov rbp, rsp
    sub rsp, 16

    call sample001_helloworld
    call sample002_alphabet
    call sample003_strlen ; peda で rax の値を確認
    call sample004_itoa

    mov rdi, rax
    call _exit
