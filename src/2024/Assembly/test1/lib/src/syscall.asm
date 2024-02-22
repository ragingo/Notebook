%include "lib/include/syscall.inc"

section .text
    extern strlen
    global sys_write
    global sys_open
    global sys_creat
    global sys_nanosleep
    global sys_exit

;==================================================
; Description
;   write system call
; Parameters
;   - rdi : file descriptor
;   - rsi : buffer
;   - rdx : buffer size
;==================================================
sys_write:
    mov rax, SYS_WRITE
    syscall
    ret

;==================================================
; Description
;   open system call
; Parameters
;   - rdi : fileName
;   - rsi : flags
;   - rdx : mode (permission)
; Returns
;   - rax : file descriptor
;==================================================
sys_open:
    mov rax, SYS_OPEN
    syscall
    ret

;==================================================
; Description
;   creat system call
; Parameters
;   - rdi : fileName
;   - rsi : mode (permission)
; Returns
;   - rax : file descriptor
;==================================================
sys_creat:
    mov rax, SYS_CREAT
    syscall
    ret

;==================================================
; Description
;   指定された時間だけプロセスを停止
; Parameters
;   - rdi : const struct timespec *req
;   - rsi : struct timespec *rem
;==================================================
sys_nanosleep:
    mov rax, SYS_NANOSLEEP
    syscall
    ret

;==================================================
; Description
;   指定された時間だけプロセスを停止
; Parameters
;   - rdi : exit code
;==================================================
sys_exit:
    mov rax, SYS_EXIT
    syscall
    ret
