; Generated Assembly Code
section .data
float1 dd 9.0
str1 db "this is a float = %f", 0

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
mov rdi, str1
cvtss2sd xmm0, [float1]
mov eax, 1
call printf
mov eax, 0
leave
ret

