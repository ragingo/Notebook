%include "lib/include/stdio.inc"

section .text
    global strlen

strlen:
    xor rcx, rcx
.loop:
    cmp byte [rsi + rcx], NULL
    je .done
    inc rcx
    jmp .loop
.done:
    mov rax, rcx
    ret
