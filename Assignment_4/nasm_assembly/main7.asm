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
mov DWORD [rbp - 4], 1
@E1:
leave
ret
