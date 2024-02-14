%include "lib/include/ascii.inc"
%include "lib/include/stdio.inc"
%include "lib/include/syscall.inc"

section .text
    extern strlen
    global putc
    global puts

;==================================================
; Description
;   文字を任意のストリームへ出力
; Parameters
;   - rdi : character
;   - rsi : file descriptor (ex. STDOUT_FILENO)
;==================================================
putc:
    push rdi
    mov rax, SYS_WRITE
    mov rdi, rsi
    mov rsi, rsp
    mov rdx, 1
    syscall
    pop rdi
    ret

;==================================================
; Description
;   文字列を標準出力へ出力
; Parameters
;   - rdi : buffer
;==================================================
puts:
    push rdi
    call strlen
    pop rdi
    mov byte [rdi + rax], ASCII_LF
    inc rax

    mov rdx, rax
    mov rax, SYS_WRITE
    mov rsi, rdi
    mov rdi, STDOUT_FILENO
    syscall

    ret
