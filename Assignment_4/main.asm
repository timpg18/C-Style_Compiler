; Generated Assembly Code
section .data
str1 db "%d ", 0

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

func:
push rbp
mov rbp, rsp
sub rsp, 16
mov QWORD [rbp - 4], 1
mov rdi, str1
mov rsi, QWORD [rbp - 4]
mov eax, 0
call printf
mov DWORD [rbp - 8], eax
@E1:
leave
ret