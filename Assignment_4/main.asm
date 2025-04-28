; Generated Assembly Code
section .data
str1 db "%d %c k", 0
str2 db "%d %c ", 10, 0

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
movss xmm7, [float1]
cvttss2si r11d, xmm7
mov DWORD [rbp - 8], r11d
mov r11d, DWORD [rbp - 8]
mov DWORD [rbp - 4], r11d
mov eax, DWORD [rbp - 4]
jmp @E1
@E1:
mov DWORD [rbp - 36], eax
mov DWORD [rbp - 16], ecx
mov DWORD [rbp - 28], edi
mov DWORD [rbp - 20], edx
mov DWORD [rbp - 24], esi
mov DWORD [rbp - 32], r10d
mov DWORD [rbp - 452], r8d
mov DWORD [rbp - 448], r9d
leave
ret

