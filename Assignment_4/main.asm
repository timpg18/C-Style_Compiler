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
sub rsp, 48
mov r9d, DWORD [rbp - 912]
mov r9d, 4
imul r9d, 4
mov r8d, DWORD [rbp - 916]
mov r8d, 4
imul r8d, 20
mov ecx, DWORD [rbp - 920]
mov ecx, r8d
add ecx, 1
mov edx, DWORD [rbp - 924]
mov edx, ecx
imul edx, 4
mov esi, DWORD [rbp - 928]
mov esi, 5
imul esi, 10
mov , QWORD [rbp - 908]
leave
ret