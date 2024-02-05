%include "lib/include/ascii.inc"
%include "lib/include/stdio.inc"
%include "lib/include/syscall.inc"

section .text
    extern strlen
    global puts

puts:
    call strlen
    mov byte [rsi + rax], ASCII_LF
    inc rax

    mov rdx, rax
    mov rax, SYS_WRITE
    mov rdi, STDOUT_FILENO
    syscall

    ret
