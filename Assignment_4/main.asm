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
sub rsp, 16
mov r11d, 1
cvtsi2ss xmm7, r11d
movss DWORD [rbp - 8], xmm7
movss xmm7, DWORD [rbp - 8]
movss DWORD [rbp - 4], xmm7
mov rdi, str1
cvtss2sd xmm0, DWORD [rbp - 4]
mov eax, 1
call printf
mov DWORD [rbp - 12], eax
mov eax, 0
jmp @E1
@E1:
leave
ret
