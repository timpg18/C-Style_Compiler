section .data
float1 dd 9.0
float2 dd 9.8

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

f:
push rbp
mov rbp, rsp
sub rsp, 16
mov DWORD [rbp - 4], edi
mov eax, 0
jmp @E1
@E1:
leave
ret
main:
push rbp
mov rbp, rsp
sub rsp, 16
mov edi, 1
mov esi, 2
mov edx, 4
cvtss2sd xmm0, [float1]
cvtss2sd xmm1, [float2]
mov ecx, 4
call f
mov DWORD [rbp - 8], eax
mov r11d, DWORD [rbp - 8]
mov DWORD [rbp - 4], r11d
@E2:
leave
ret
