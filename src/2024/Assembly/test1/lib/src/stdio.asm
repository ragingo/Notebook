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
;   - rsi : モード
;     - (r|w|a)\+?
; Returns
;   - rax : ファイルディスクリプタ
;==================================================
fopen:
    push rdi
    mov rdi, rsi
    call strlen

    cmp rax, 0
    je .fopen.mode_default
    cmp rax, 1
    je .fopen.mode_simple
    cmp rax, 2
    je .fopen.mode_complex

.fopen.mode_default:
    mov rsi, O_CREAT | O_WRONLY | O_TRUNC
    jmp .fopen.call_open

.fopen.mode_simple:
    mov r8b, byte [rsi + 0]
    cmp r8, 'r'
    jne .fopen.mode_simple.not_read
    mov rsi, O_RDONLY
.fopen.mode_simple.not_read:
    cmp r8, 'w'
    jne .fopen.mode_simple.not_write
    mov rsi, O_WRONLY
.fopen.mode_simple.not_write:
    cmp r8, 'a'
    jne .fopen.mode_default ; デフォルトに飛ばす
    mov rsi, O_APPEND
    jmp .fopen.call_open

; TODO: "+" 対応
.fopen.mode_complex:
    jmp .fopen.call_open

.fopen.call_open:
    pop rdi
    mov rdx, S_IRWXU | S_IRWXG | S_IRWXO
    call sys_open

    ret
