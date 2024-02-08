section .text
    global digits

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
