section .text
    global digits
    global itoa

;==================================================
; Description
;   与えられた10進数の数値の桁数を計算
; Parameters
;   - rdi : integer
; Returns
;   - rax : 桁数
;==================================================
digits:
    xor rcx, rcx
    mov rax, rdi
.digits.loop:
    inc rcx
    mov rdi, 10
    cqo
    idiv rdi
    cmp rax, 0
    jnz .digits.loop

    mov rax, rcx
    ret

;==================================================
; Description
;   与えられた10進数の数値を文字列に変換
; Parameters
;   - rdi : integer
;   - rsi : buffer
;   - rdx : buffer size
;==================================================
itoa:
    xor rcx, rcx    ; 文字数カウンタ
    mov rax, rdi    ; 10で割っていく数値
    mov r8, rdx     ; バッファサイズ
.itoa.loop:
    mov rdi, 10
    cqo
    idiv rdi

    mov r9, r8
    dec r9
    sub r9, rcx ; バッファのオフセット
    add rdx, '0'
    mov byte [rsi + r9], dl

    inc rcx
    cmp rax, 0
    jnz .itoa.loop

    ret
