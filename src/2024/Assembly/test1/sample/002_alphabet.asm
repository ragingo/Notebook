%include "lib/include/stdio.inc"

section .bss
    buf resb 27

section .text
    global sample002_alphabet
    extern puts

sample002_alphabet:
    mov rdi, buf
    mov al, 'A'

loop:
    mov [rdi], al
    inc al
    inc rdi
    cmp al, 'Z'
    jle loop

    mov byte [rdi], NULL
    mov rdi, buf
    call puts
    ret
