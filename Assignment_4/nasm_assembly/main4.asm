section .data
str1 db "%d %d", 10, 0

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
mov DWORD [rbp - 4], 5
mov r9d, DWORD [rbp - 4]
mov r8d, DWORD [rbp - 16]
mov r8d, r9d
shl r8d, 4
mov DWORD [rbp - 8], r8d
mov ecx, DWORD [rbp - 20]
mov ecx, r9d
sar ecx, 1
mov DWORD [rbp - 12], ecx
mov DWORD [rbp - 20], ecx
mov DWORD [rbp - 16], r8d
mov DWORD [rbp - 4], r9d
mov rdi, str1
mov esi, DWORD [rbp - 8]
mov edx, DWORD [rbp - 12]
mov eax, 0
call printf
mov DWORD [rbp - 24], eax
mov eax, 0
jmp @E1
@E1:
leave
ret
