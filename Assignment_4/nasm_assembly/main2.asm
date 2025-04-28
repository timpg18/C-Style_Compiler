section .data
str1 db "factorial of %d is %d ", 10, 0

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

fac:
push rbp
mov rbp, rsp
sub rsp, 32
mov DWORD [rbp - 4], edi
mov r9d, DWORD [rbp - 4]
cmp r9d, 1
mov DWORD [rbp - 4], r9d
je @L0
jmp @L1
@L0:
mov eax, 1
jmp @E1
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
mov r9d, DWORD [rbp - 16]
mov r8d, DWORD [rbp - 4]
mov ecx, DWORD [rbp - 20]
mov ecx, r8d
imul ecx, r9d
mov eax, ecx
jmp @E1
@E1:
mov DWORD [rbp - 20], ecx
mov DWORD [rbp - 4], r8d
mov DWORD [rbp - 16], r9d
leave
ret
main:
push rbp
mov rbp, rsp
sub rsp, 32
mov DWORD [rbp - 4], 5
mov edi, DWORD [rbp - 4]
call fac
mov DWORD [rbp - 12], eax
mov r11d, DWORD [rbp - 12]
mov DWORD [rbp - 8], r11d
mov rdi, str1
mov esi, DWORD [rbp - 4]
mov edx, DWORD [rbp - 8]
mov eax, 0
call printf
mov DWORD [rbp - 16], eax
mov DWORD [rbp - 4], 9
mov edi, DWORD [rbp - 4]
call fac
mov DWORD [rbp - 20], eax
mov r11d, DWORD [rbp - 20]
mov DWORD [rbp - 8], r11d
mov rdi, str1
mov esi, DWORD [rbp - 4]
mov edx, DWORD [rbp - 8]
mov eax, 0
call printf
mov DWORD [rbp - 24], eax
mov eax, 0
jmp @E2
@E2:
leave
ret
