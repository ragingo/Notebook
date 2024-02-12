%include "lib/include/syscall.inc"

section .text
    global nanosleep

;==================================================
; Description
;   与えられた10進数の数値を文字列に変換
; Parameters
;   - rdi : const struct timespec *req
;   - rsi : struct timespec *rem
;==================================================
nanosleep:
    mov rax, SYS_NANOSLEEP
    syscall
    ret
