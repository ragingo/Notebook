; nasm 公式サイトのサンプルをいじった

global  main
extern  printf

section .text
main:
  sub  rsp,  28h    ; Reserve the shadow space
  mov  rcx,  fmt
  mov  rdx,  msg
  mov  r8,   msg
  mov  r9,   msg
  call printf
  add  rsp,  28h    ; Remove shadow space
  ret
fmt:
  db '%s %s %s', 0
msg:
  db 'Hello World!', 0
