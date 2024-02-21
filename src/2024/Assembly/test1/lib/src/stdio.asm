%include "lib/include/ascii.inc"
%include "lib/include/fcntl.inc"
%include "lib/include/stat.inc"
%include "lib/include/stdio.inc"
%include "lib/include/syscall.inc"

section .bss
    buf1 resb 1

section .text
    extern strlen
    extern sys_open
    extern sys_write

section .text
    global putc
    global putchar
    global puts
    global println
    global print
    global fopen

;==================================================
; Description
;   文字を任意のストリームへ出力
; Parameters
;   - rdi : character
;   - rsi : file descriptor (ex. STDOUT_FILENO)
;==================================================
putc:
    push rdi
    push rsi

    mov byte [buf1], dil
    mov rdi, rsi
    mov rsi, buf1
    mov rdx, 1
    call sys_write

    pop rsi
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

    mov rsi, rdi
    mov rdi, STDOUT_FILENO
    mov rdx, rax
    call sys_write

    ret

;==================================================
; Description
;   ファイルを開く
; Parameters
;   - rdi : ファイル名
; Returns
;   - rax : ファイルディスクリプタ
;==================================================
fopen:
    call _open
    ret

; TODO: モード指定対応
; TODO: 権限指定対応
_open:
    mov rax, SYS_OPEN
    mov rsi, 0x441 ; O_CREAT| O_WRONLY | O_APPEND
    mov edx, 0q666 ; permission
    syscall
    ret
