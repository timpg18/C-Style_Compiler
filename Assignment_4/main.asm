; Generated Assembly Code
section .data
str1 db "%ld", 0

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
mov r11d, 50
movsx r11, r11d
mov QWORD [rbp - 16],r11
mov r11, QWORD [rbp - 16]
mov QWORD [rbp - 8], r11
mov rdi, str1
mov rsi, QWORD [rbp - 8]
call printf
mov DWORD [rbp - 20], eax
mov eax, 0
jmp @E1
@E1:
leave
ret
