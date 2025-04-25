; Generated Assembly Code
section .text
        global _start
        extern printf
        extern exit

_start:
        and rsp, 0xfffffffffffffff0
        call main
        mov rdi, rax
        call exit

fac:
push rbp
mov rbp, rsp
sub rsp, 32
mov DWORD [rbp - 4],edi
mov r9d, DWORD [rbp - 4]
cmp r9d, 1
mov DWORD [rbp - 4], r9d
je @L0
jmp @L1
@L0:
mov eax, 1
jmp @L2
@L1:
mov r9d, DWORD [rbp - 4]
mov r8d, DWORD [rbp - 12]
mov r8d, r9d
sub r8d, 1
mov DWORD [rbp - 12], r8d
mov DWORD [rbp - 4], r9d
mov edi, DWORD [rbp - 12]
call fac
mov DWORD [rbp - 16], eax
mov r9d, DWORD [rbp - 4]
mov r8d, DWORD [rbp - 16]
mov ecx, DWORD [rbp - 20]
mov ecx, r8d
imul ecx, r9d
mov eax, ecx
@L2:
leave
ret
main:
push rbp
mov rbp, rsp
sub rsp, 16
mov DWORD [rbp - 20], ecx
mov DWORD [rbp - 16], r8d
mov DWORD [rbp - 4], r9d
mov edi, 6
call fac
mov DWORD [rbp - 8], eax
mov r11d, DWORD [rbp - 8]
mov DWORD [rbp - 4], r11d
mov eax, DWORD [rbp - 4]
leave
ret
