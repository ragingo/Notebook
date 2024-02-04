%include "lib/include/stdio.inc"

section .bss
    buf resb 27

section .text
    global sample002_alphabet
    extern puts

sample002_alphabet:
    mov rsi, buf
    mov al, 'A'

loop:
    mov [rsi], al
    inc al
    inc rsi
    cmp al, 'Z'
    jle loop

    mov byte [rsi], NULL
    mov rsi, buf
    call puts
    ret
