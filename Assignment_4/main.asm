; Generated Assembly Code
section .text
        global _start
        extern printf
        extern scanf
        extern free
        extern malloc
        extern exit

_start:
        and rsp, 0xfffffffffffffff0
        call main
        mov rdi, rax
        call exit

main:
push rbp
mov rbp, rsp
sub rsp, 32
mov r9d, DWORD [rbp - 28]
mov r9d, 1
imul r9d, 4
lea r8d, [rbp - 20] 
lea r8d, [r8d + r9d] 
mov DWORD [r8d] , 2
mov r8d, DWORD [rbp - 32]
mov r8d, 1
imul r8d, 4
lea rcx, [rbp - 20] 
lea rcx, [rcx + r8d] 
mov DWORD [rbp - 24], rcx
@E1:
mov DWORD [rbp - 32], r8d
mov DWORD [rbp - 28], r9d
leave
ret