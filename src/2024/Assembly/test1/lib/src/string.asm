%include "lib/include/stdio.inc"

section .text
    global strlen

;==================================================
; Description
;   文字列長(バイト数)を返す
; Parameters
;   - rdi : NULL終端文字列
; Returns
;   - rax : バイト数
;==================================================
strlen:
    xor rax, rax
    xor rcx, rcx
    dec rcx
    repne scasb

    not rcx
    dec rcx
    mov rax, rcx
    ret
