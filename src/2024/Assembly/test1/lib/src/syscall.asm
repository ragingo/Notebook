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
    system_call SYS_WRITE
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
    system_call SYS_OPEN
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
    system_call SYS_CREAT
    ret

;==================================================
; Description
;   指定された時間だけプロセスを停止
; Parameters
;   - rdi : const struct timespec *req
;   - rsi : struct timespec *rem
;==================================================
sys_nanosleep:
    system_call SYS_NANOSLEEP
    ret

;==================================================
; Description
;   指定された時間だけプロセスを停止
; Parameters
;   - rdi : exit code
;==================================================
sys_exit:
    system_call SYS_EXIT
    ret
