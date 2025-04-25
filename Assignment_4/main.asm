; Generated Assembly Code
section .data
float1 dd 9.0

section .text
global _start
extern printf
extern exit

_start:
and rsp, 0xfffffffffffffff0
call main
mov rdi, rax
call exit

main:
push rbp
mov rbp, rsp
sub rsp, 16
movss xmm7, [float1]
movss [rbp - 4], xmm7
movss xmm7, [rbp - 4]
movss [rbp - 8], xmm7
movss xmm6, [rbp - 4]
movss xmm5, [rbp - 12]
movss xmm5, xmm6
addss xmm5, [float1]
movss [rbp - 8], xmm5
mov eax, 0
leave
ret
