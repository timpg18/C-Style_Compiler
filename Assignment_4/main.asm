; Generated Assembly Code
section .data
str1 db "%f", 10, 0

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

mov r11d, 'b'
cvtsi2ss xmm7, r11d
movss DWORD [rbp - 12], xmm7
movss xmm7, DWORD [rbp - 12]
movss DWORD [rbp - 8], xmm7
mov rdi, str1
cvtss2sd xmm0, DWORD [rbp - 8]
mov eax, 1
call printf
mov DWORD [rbp - 16], eax
mov eax, 0
jmp @E1
@E1:
leave
ret

