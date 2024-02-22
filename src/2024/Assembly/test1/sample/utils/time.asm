section .text
    global sleep
    extern sys_nanosleep

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
    call sys_nanosleep

    pop rdi
    pop rdi

    ret
