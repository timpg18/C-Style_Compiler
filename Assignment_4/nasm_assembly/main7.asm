section .data
str1 db "the number of arguments: %d", 10, 0
str2 db "argument 1:%d ", 10, 0

section .text
	global _start
	extern printf
	extern scanf
	extern free
	extern malloc
	extern exit

_start:
	pop rdi 
	lea rsi, [rsp] 
	and rsp, 0xfffffffffffffff0
	call main
	mov rdi, rax
	call exit

main:
push rbp
mov rbp, rsp
sub rsp, 32
mov DWORD [rbp - 4], edi
mov QWORD [rbp - 12], rsi
mov rdi, str1
mov esi, DWORD [rbp - 4]
mov eax, 0
call printf
mov DWORD [rbp - 16], eax
mov r9, QWORD [rbp - 12]
mov r8, QWORD [rbp - 24]
mov r8, r9
add r8, 8
mov QWORD [rbp - 24], r8
mov QWORD [rbp - 12], r9
mov rdi, str2
mov rsi, QWORD [rbp - 24]
mov eax, 0
call printf
mov DWORD [rbp - 28], eax
mov eax, 0
jmp @E1
@E1:
leave
ret
