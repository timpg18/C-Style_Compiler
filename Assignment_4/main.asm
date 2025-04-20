section .data
str1 db "hekllo world i am despacito ", 10, " %d ", 10, 0

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
mov esi, 1923
call printf
leave
ret

