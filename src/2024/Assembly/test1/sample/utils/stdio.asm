section .text
    global print_number
    extern digits
    extern itoa
    extern print

;==================================================
; Description
;   数値を標準出力へ出力する
; Parameters
;   - rdi : integer
;   - rsi : width
;==================================================
print_number:
    push rbp
    mov rbp, rsp
    sub rsp, 32

    ; 桁数取得
    push rdi
    push rsi
    call digits
    pop rsi
    pop rdi
    mov r8, rax

    ; rax = rax 桁数 < rsi width なら
    ; rsi - rax の分だけ前方にスペースを入れる
    cmp rax, rsi
    jg .print_number.L1
    sub rsi, rax
    mov rcx, rsi
    mov r8, rcx
.print_number.memset:
    mov byte [rbp - 32 + rcx - 1], ' '
    loop .print_number.memset

    ; NULL 用に +1
    inc rax
    lea rsi, [rbp - 32 + r8]
    jmp .print_number.L2

.print_number.L1:
    ; NULL 用に +1
    inc rax
    lea rsi, [rbp - 32]

.print_number.L2:
    mov rdx, rax
    call itoa

    lea rdi, [rbp - 32]
    xor rsi, rsi
    call print

    leave
    ret
