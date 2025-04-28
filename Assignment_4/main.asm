; Generated Assembly Code
section .data
str1 db "%d %d ", 10, 0

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
mov QWORD [rbp - 4],edi
mov QWORD [rbp - 8],esi
mov r9d, DWORD [rbp - 8]
mov r8d, DWORD [rbp - 4]
mov ecx, DWORD [rbp - 12]
mov ecx, r8d
add ecx, r9d
mov eax, ecx
jmp @E1
@E1:
mov DWORD [rbp - 12], ecx
mov DWORD [rbp - 4], r8d
mov DWORD [rbp - 8], r9d
leave
ret
main:
push rbp
mov rbp, rsp
sub rsp, 16
mov DWORD [rbp - 4], 2
mov DWORD [rbp - 8], 3
mov rdi, str1
mov esi, DWORD [rbp - 4]
mov edx, DWORD [rbp - 8]
call printf
mov DWORD [rbp - 12], eax
mov eax, 0
jmp @E2
@E2:
leave
ret

