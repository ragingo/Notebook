section .text
    global is_positive

;==================================================
; Description
;   与えられた数値が符号なしかどうかを返す
; Parameters
;   - rdi : integer
; Returns
;   - rax : 1 (positive), 0 (negative)
;==================================================
is_positive:
    cmp rdi, 0
    js .is_positive.negative
    mov rax, 1
    ret
.is_positive.negative:
    mov rax, 0
    ret
