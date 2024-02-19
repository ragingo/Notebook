%include "lib/include/syscall.inc"

section .text
    global nanosleep

;==================================================
; Description
;   指定された時間だけプロセスを停止
; Parameters
;   - rdi : const struct timespec *req
;   - rsi : struct timespec *rem
;==================================================
nanosleep:
    mov rax, SYS_NANOSLEEP
    syscall
    ret
