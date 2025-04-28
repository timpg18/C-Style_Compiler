; Generated Assembly Code
section .data
str1 db "%d", 0

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
mov r9d, DWORD [rbp - 44]
mov r9d, 1
imul r9d, 4
lea r8, [rbp - 40]
mov r11, r9
lea r8, [r8 + r11]
mov DWORD [r8] , 11
mov r8d, DWORD [rbp - 48]
mov r8d, 1
imul r8d, 4
lea rcx, [rbp - 40]
lea rcx, [rcx + r8]
mov ecx, DWORD [rcx]
mov DWORD [rbp - 52], ecx
mov DWORD [rbp - 48], r8d
mov DWORD [rbp - 44], r9d
mov rdi, str1
mov esi, DWORD [rbp - 52]
mov eax, 0
call printf
mov DWORD [rbp - 56], eax
@E1:
leave
ret

