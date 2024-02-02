%include "include/ascii.inc"
%include "include/stdio.inc"
%include "include/syscall.inc"

section .data
    message db 'Hello, world', ASCII_LF, 0

section .text
    global _start

_start:
    mov rax, SYS_WRITE
    mov rdi, STDOUT_FILENO
    mov rsi, message
    mov rdx, 14     ; length
    syscall

    mov rax, SYS_EXIT
    xor rdi, rdi    ; exit code 0
    syscall
