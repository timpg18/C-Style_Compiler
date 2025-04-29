section .data
str1 db "%d ", 10, 0
str2 db "%d", 10, 0
str3 db "%d %d", 10, 0

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
sub rsp, 96
mov DWORD [rbp - 4], 2
mov r9d, DWORD [rbp - 4]
cmp r9d, 0
mov DWORD [rbp - 4], r9d
jg @L0
jmp @L1
@L0:
mov r11d, DWORD [rbp - 4]
mov DWORD [rbp - 28], r11d
mov r9d, DWORD [rbp - 4]
mov r9d, r9d
add r9d, 1
mov DWORD [rbp - 4], r9d
jmp @L2
@L1:
mov r11d, DWORD [rbp - 4]
mov DWORD [rbp - 32], r11d
mov r9d, DWORD [rbp - 4]
mov r9d, r9d
sub r9d, 1
@L2:
mov DWORD [rbp - 4], r9d
mov rdi, str1
mov esi, DWORD [rbp - 4]
mov eax, 0
call printf
mov DWORD [rbp - 36], eax
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
mov DWORD [rbp - 44], r9d
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
mov DWORD [rbp - 48], eax
cmp QWORD[rbp - 4], 1
je @L6
cmp QWORD[rbp - 4], 58
je @L7
jmp @L8
@L6:
mov r11d, DWORD [rbp - 4]
mov DWORD [rbp - 52], r11d
mov r9d, DWORD [rbp - 4]
mov r9d, r9d
add r9d, 1
mov DWORD [rbp - 4], r9d
jmp @L8
@L7:
mov DWORD [rbp - 4], 20
jmp @L8
@L8:
mov r9d, DWORD [rbp - 4]
cmp r9d, 0
mov DWORD [rbp - 4], r9d
jl @L9
jmp @L10
@L9:
mov r11d, DWORD [rbp - 4]
mov DWORD [rbp - 60], r11d
mov r9d, DWORD [rbp - 4]
mov r9d, r9d
add r9d, 1
mov DWORD [rbp - 4], r9d
jmp @L11
@L10:
mov r11d, DWORD [rbp - 4]
mov DWORD [rbp - 64], r11d
mov r9d, DWORD [rbp - 4]
mov r9d, r9d
sub r9d, 1
@L11:
mov DWORD [rbp - 4], r9d
mov rdi, str1
mov esi, DWORD [rbp - 4]
mov eax, 0
call printf
mov DWORD [rbp - 68], eax
mov DWORD [rbp - 12], 0
mov DWORD [rbp - 16], 0
@L15:
mov r9d, DWORD [rbp - 16]
cmp r9d, 10
mov DWORD [rbp - 16], r9d
jle @L16
jmp @L17
@L16:
mov DWORD [rbp - 20], 0
@L12:
mov DWORD [rbp - 76], -5
mov r9d, DWORD [rbp - 76]
mov r8d, DWORD [rbp - 20]
cmp r8d, r9d
mov DWORD [rbp - 20], r8d
mov DWORD [rbp - 76], r9d
jg @L13
jmp @L14
@L13:
mov r9d, DWORD [rbp - 20]
mov r8d, DWORD [rbp - 16]
mov ecx, DWORD [rbp - 84]
mov ecx, r8d
sub ecx, r9d
mov edx, DWORD [rbp - 12]
mov edx, edx
add edx, ecx
mov DWORD [rbp - 88], r9d
mov r9d, r9d
sub r9d, 1
mov DWORD [rbp - 84], ecx
mov DWORD [rbp - 12], edx
mov DWORD [rbp - 16], r8d
mov DWORD [rbp - 20], r9d
jmp @L12
@L14:
mov r11d, DWORD [rbp - 16]
mov DWORD [rbp - 92], r11d
mov r9d, DWORD [rbp - 16]
mov r9d, r9d
add r9d, 1
mov DWORD [rbp - 16], r9d
jmp @L15
@L17:
mov rdi, str3
mov esi, DWORD [rbp - 4]
mov edx, DWORD [rbp - 12]
mov eax, 0
call printf
mov DWORD [rbp - 96], eax
@E1:
leave
ret
