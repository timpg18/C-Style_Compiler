; Generated Assembly Code
section .data
str1 db "%f", 0

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
lea r11, [rbp - 4]
mov QWORD [rbp - 12], r11
mov rdi, str1
mov rsi, QWORD [rbp - 12]
call scanf
mov DWORD [rbp - 16], eax
mov rdi, str1
cvtss2sd xmm0, DWORD [rbp - 4]
mov eax, 1
call printf
mov DWORD [rbp - 20], eax
mov eax, 0
jmp @E1
@E1:
leave
ret
