; Generated Assembly Code
section .data
str1 db "%c", 0

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
mov r11d, 50
movsx r11d, r11b
mov DWORD [rbp - 9], r11d
mov r11b, BYTE [rbp - 9]
mov BYTE [rbp - 1], r11b
mov DWORD [rbp - 8], 0
mov rdi, str1
mov sil, BYTE [rbp - 1]
call printf
mov DWORD [rbp - 16], eax
mov eax, 0
jmp @E1
@E1:
leave
ret
