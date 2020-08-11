global _main

section .data
  msg db 'Hello, World!', 0x0A
  msg_len equ $ - msg

section .text

_main:
  push  dword msg_len
  push  dword msg
  push  dword 1
  mov   eax, 4
  sub   esp, 4
  int   0x80
  add   esp, 16

  push  dword 0
  mov   eax, 1
  sub   esp, 12
  int   0x80
