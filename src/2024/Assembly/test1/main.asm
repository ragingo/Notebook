section .data
    filename db '/home/ragingo/a.txt', 0

section .text
    global _start
    extern _exit
    extern sample001_helloworld
    extern sample002_alphabet
    extern sample003_strlen
    extern sample004_itoa
    extern sample005_kuku
    extern fopen

_start:
    push rbp
    mov rbp, rsp
    sub rsp, 16

    call sample001_helloworld
    call sample002_alphabet
    call sample003_strlen ; peda で rax の値を確認
    call sample004_itoa
    call sample005_kuku

    mov rdi, filename
    call fopen

    mov rdi, rax
    call _exit
