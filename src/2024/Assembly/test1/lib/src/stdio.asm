%include "lib/include/ascii.inc"
%include "lib/include/stdio.inc"
%include "lib/include/syscall.inc"

section .text
    extern strlen
    global putc
    global putchar
    global puts
    global println
    global print

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
;   文字を標準出力へ出力
; Parameters
;   - rdi : character
;==================================================
putchar:
    mov rsi, STDOUT_FILENO
    call putc
    ret

;==================================================
; Description
;   文字列を標準出力へ出力
; Parameters
;   - rdi : buffer
;==================================================
puts:
    call println
    ret

;==================================================
; Description
;   文字列を標準出力へ出力
; Parameters
;   - rdi : buffer
;==================================================
println:
    mov rsi, 1
    call print
    ret

;==================================================
; Description
;   文字列を標準出力へ出力
; Parameters
;   - rdi : buffer
;   - rsi : 終端の \0 を LF に書き換えるかどうか
;       - 0 : 書き換えない
;       - 1 : 書き換える
;==================================================
print:
    push rdi
    call strlen
    pop rdi

    cmp rsi, 0
    jz .print.overwrite.done
    mov byte [rdi + rax], ASCII_LF
.print.overwrite.done:
    inc rax

    mov rdx, rax
    mov rax, SYS_WRITE
    mov rsi, rdi
    mov rdi, STDOUT_FILENO
    syscall

    ret
