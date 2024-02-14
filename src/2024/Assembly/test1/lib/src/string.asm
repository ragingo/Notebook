%include "lib/include/stdio.inc"

section .text
    global strlen

strlen:
    xor rax, rax
    xor rcx, rcx
    dec rcx
    repne scasb

    not rcx
    dec rcx
    mov rax, rcx
    ret
