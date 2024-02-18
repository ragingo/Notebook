section .text
    global _start
    extern _exit
    extern sample001_helloworld
    extern sample002_alphabet
    extern sample003_strlen
    extern sample004_itoa
    extern puts_number

_start:
    push rbp
    mov rbp, rsp
    sub rsp, 16

    call sample001_helloworld
    call sample002_alphabet
    call sample003_strlen ; peda で rax の値を確認
    call sample004_itoa

    mov rdi, 9876543210
    call puts_number

    mov rdi, rax
    call _exit
