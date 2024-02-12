section .text
    global sleep
    extern nanosleep

;==================================================
; Description
;   指定秒数の間、実行を停止する
; Parameters
;   - rdi : seconds
;==================================================
sleep:
    push qword 0
    push qword rdi

    lea rdi, [rsp]
    xor rsi, rsi
    call nanosleep

    pop rdi
    pop rdi

    ret
