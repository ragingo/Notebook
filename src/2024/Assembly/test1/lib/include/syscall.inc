%ifndef _SYSCALL_INC_
%define _SYSCALL_INC_

; system call numbers
; https://github.com/torvalds/linux/blob/master/arch/x86/entry/syscalls/syscall_64.tbl
%define SYS_WRITE        1
%define SYS_OPEN         2
%define SYS_NANOSLEEP   35
%define SYS_EXIT        60
%define SYS_CREAT       85

%macro system_call 1
    mov rax, %1
    syscall
%endmacro

%endif
