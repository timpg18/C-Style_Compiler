section .data
str1 db "fdsgsdgsg %d\n",10, "fsdhk", 0

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
mov rdi, str1
mov esi, 100
call printf
leave
ret

