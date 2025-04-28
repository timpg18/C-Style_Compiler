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
sub rsp, 16
mov DWORD [rbp - 4], 5
cmp DWORD[rbp - 4], 1
je @L0
cmp DWORD[rbp - 4], 2
je @L1
jmp @L2
@L0:
mov DWORD [rbp - 4], 2
jmp @L3
@L1:
mov DWORD [rbp - 4], 3
jmp @L3
@L2:
mov DWORD [rbp - 4], 7
jmp @L3
@L3:
mov eax, DWORD [rbp - 4]
jmp @E1
@E1:
leave
ret