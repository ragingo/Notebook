section .data
    filename db '/home/ragingo/temp/a.txt', 0

section .text
    global _start
    extern _exit
    extern sample001_helloworld
    extern sample002_alphabet
    extern sample003_strlen
    extern sample004_itoa
    extern sample005_kuku
    extern fopen
    extern sys_write
    extern strlen

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
    push 'w'
    mov rsi, rsp
    call fopen
    pop r8

    mov r8, rax ; fd

    mov rdi, filename
    call strlen

    mov rdi, r8
    mov rsi, filename
    mov rdx, rax
    call sys_write

    mov rdi, rax
    call _exit
