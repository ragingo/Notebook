%include "lib/include/syscall.inc"

section .text
    global _exit

; 第1引数 RDI をそのまま SYS_EXIT に渡す
_exit:
    mov rax, SYS_EXIT
    syscall
