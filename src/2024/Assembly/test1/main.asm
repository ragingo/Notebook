%include "lib/include/ascii.inc"
%include "lib/include/stdio.inc"

section .data
    message db 'Hello, world!!!', ASCII_LF, 0

section .text
    extern puts
    extern _exit
    global _start

_start:
    mov rsi, message
    call puts

    ;mov rdi, 1
    xor rdi, rdi
    call _exit
