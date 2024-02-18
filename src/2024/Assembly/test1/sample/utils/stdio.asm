section .text
    global puts_number
    extern digits
    extern itoa
    extern puts

;==================================================
; Description
;   数値を標準出力へ出力する
; Parameters
;   - rdi : integer
;==================================================
puts_number:
    push rbp
    mov rbp, rsp
    sub rsp, 32

    ; 桁数 + NULL終端文字
    push rdi
    call digits
    pop rdi
    inc rax

    lea rsi, [rbp - 32]
    mov rdx, rax
    call itoa

    mov rdi, rsi
    call puts

    leave
    ret
