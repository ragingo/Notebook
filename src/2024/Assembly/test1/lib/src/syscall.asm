%include "lib/include/syscall.inc"

section .text
    extern strlen
    global sys_write
    global sys_open
    global sys_creat

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
