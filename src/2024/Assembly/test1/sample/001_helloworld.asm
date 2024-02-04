%include "lib/include/ascii.inc"
%include "lib/include/stdio.inc"

section .data
    message db 'Hello, world!', ASCII_LF, 0

section .text
    global sample001_helloworld
    extern puts

sample001_helloworld:
    mov rsi, message
    call puts
    ret
