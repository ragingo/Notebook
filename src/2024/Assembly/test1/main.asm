section .text
    global _start
    extern sys_exit
    extern sample001_helloworld
    extern sample002_alphabet
    extern sample003_strlen
    extern sample004_itoa
    extern sample005_kuku
    extern sample006_file_write

_start:
    push rbp
    mov rbp, rsp
    sub rsp, 16

    call sample001_helloworld
    call sample002_alphabet
    call sample003_strlen ; peda で rax の値を確認
    call sample004_itoa
    call sample005_kuku
    call sample006_file_write

    mov rdi, rax
    call sys_exit
