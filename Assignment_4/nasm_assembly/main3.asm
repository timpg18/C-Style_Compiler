section .data
str1 db "%d ", 10, 0
str2 db "%d", 10, 0

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
sub rsp, 48
mov DWORD [rbp - 4], 2
mov r9d, DWORD [rbp - 4]
cmp r9d, 0
mov DWORD [rbp - 4], r9d
jg @L0
jmp @L1
@L0:
mov r11d, DWORD [rbp - 4]
mov DWORD [rbp - 16], r11d
mov r9d, DWORD [rbp - 4]
mov r9d, r9d
add r9d, 1
mov DWORD [rbp - 4], r9d
jmp @L2
@L1:
mov r11d, DWORD [rbp - 4]
mov DWORD [rbp - 20], r11d
mov r9d, DWORD [rbp - 4]
mov r9d, r9d
sub r9d, 1
@L2:
mov DWORD [rbp - 4], r9d
mov rdi, str1
mov esi, DWORD [rbp - 4]
mov eax, 0
call printf
mov DWORD [rbp - 24], eax
mov DWORD [rbp - 8], 0
@L3:
mov r9d, DWORD [rbp - 8]
cmp r9d, 10
mov DWORD [rbp - 8], r9d
jle @L4
jmp @L5
@L4:
mov r9d, DWORD [rbp - 8]
mov r8d, DWORD [rbp - 4]
mov r8d, r8d
add r8d, r9d
mov DWORD [rbp - 32], r9d
mov r9d, r9d
add r9d, 1
mov DWORD [rbp - 4], r8d
mov DWORD [rbp - 8], r9d
jmp @L3
@L5:
mov rdi, str2
mov esi, DWORD [rbp - 4]
mov eax, 0
call printf
mov DWORD [rbp - 36], eax
cmp QWORD[rbp - 4], 1
je @L6
cmp QWORD[rbp - 4], 58
je @L7
jmp @L8
@L6:
mov r11d, DWORD [rbp - 4]
mov DWORD [rbp - 40], r11d
mov r9d, DWORD [rbp - 4]
mov r9d, r9d
add r9d, 1
mov DWORD [rbp - 4], r9d
jmp @L8
@L7:
mov DWORD [rbp - 4], 20
jmp @L8
@L8:
mov rdi, str2
mov esi, DWORD [rbp - 4]
mov eax, 0
call printf
mov DWORD [rbp - 44], eax
@E1:
leave
ret
