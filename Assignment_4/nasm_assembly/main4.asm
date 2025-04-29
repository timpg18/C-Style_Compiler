section .data
str1 db "%d %d %d", 10, 0
str2 db "%d ", 10, 0

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
sub rsp, 64
mov DWORD [rbp - 4], 100
mov DWORD [rbp - 8], 1
mov DWORD [rbp - 12], 0
@L0:
mov r9d, DWORD [rbp - 8]
cmp r9d, 10
mov DWORD [rbp - 8], r9d
jne @L1
jmp @L2
@L1:
mov r11d, DWORD [rbp - 8]
mov DWORD [rbp - 28], r11d
mov r9d, DWORD [rbp - 8]
mov r9d, r9d
add r9d, 1
mov DWORD [rbp - 8], r9d
jmp @L0
mov r11d, DWORD [rbp - 12]
mov DWORD [rbp - 32], r11d
mov r9d, DWORD [rbp - 12]
mov r9d, r9d
add r9d, 1
mov DWORD [rbp - 12], r9d
jmp @L0
@L2:
mov rdi, str1
mov esi, DWORD [rbp - 8]
mov edx, DWORD [rbp - 12]
mov ecx, DWORD [rbp - 4]
mov eax, 0
call printf
mov DWORD [rbp - 36], eax
@L3:
mov r11d, DWORD [rbp - 8]
mov DWORD [rbp - 40], r11d
mov r9d, DWORD [rbp - 8]
mov r9d, r9d
sub r9d, 1
@L4:
cmp r9d, 0
mov DWORD [rbp - 8], r9d
jg @L3
jmp @L5
@L5:
mov rdi, str2
mov esi, DWORD [rbp - 8]
mov eax, 0
call printf
mov DWORD [rbp - 48], eax
mov DWORD [rbp - 16], 1
mov DWORD [rbp - 20], 0
@L6:
mov r9d, DWORD [rbp - 20]
cmp r9d, 10
mov DWORD [rbp - 20], r9d
jl @L7
jmp @L8
@L7:
mov r11d, DWORD [rbp - 16]
mov DWORD [rbp - 56], r11d
mov r9d, DWORD [rbp - 16]
mov r9d, r9d
add r9d, 1
mov DWORD [rbp - 16], r9d
jmp @L8
mov r11d, DWORD [rbp - 20]
mov DWORD [rbp - 60], r11d
mov r9d, DWORD [rbp - 20]
mov r9d, r9d
add r9d, 1
mov DWORD [rbp - 20], r9d
jmp @L6
@L8:
mov rdi, str2
mov esi, DWORD [rbp - 16]
mov eax, 0
call printf
mov DWORD [rbp - 64], eax
@E1:
leave
ret
