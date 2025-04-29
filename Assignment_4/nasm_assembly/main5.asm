section .data
str1 db "%d ", 10, 0
str2 db "%c ", 10, 0
str3 db "%d", 10, 0

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
sub rsp, 128
mov r9d, DWORD [rbp - 60]
mov r9d, 0
add r9d, 4
lea r8, [rbp - 16] 
mov r11, r9
lea r8, [r8 + r11] 
mov QWORD [r8] , 1
mov r8d, DWORD [rbp - 64]
mov r8d, 0
add r8d, 4
lea rcx, [rbp - 16] 
lea rcx, [rcx + r8] 
mov rcx, QWORD [rcx]
mov DWORD [rbp - 20], ecx
mov DWORD [rbp - 64], r8d
mov DWORD [rbp - 60], r9d
mov rdi, str1
mov esi, DWORD [rbp - 20]
mov eax, 0
call printf
mov DWORD [rbp - 68], eax
mov r9d, DWORD [rbp - 72]
mov r9d, 0
add r9d, 8
lea r8, [rbp - 52] 
mov r11, r9
lea r8, [r8 + r11] 
mov QWORD [r8] , 107
mov r8d, DWORD [rbp - 76]
mov r8d, 0
add r8d, 8
lea rcx, [rbp - 52] 
lea rcx, [rcx + r8] 
mov rcx, QWORD [rcx]
mov QWORD [rbp - 108], rcx
mov DWORD [rbp - 76], r8d
mov DWORD [rbp - 72], r9d
mov rdi, str2
mov rsi, QWORD [rbp - 108]
mov eax, 0
call printf
mov DWORD [rbp - 112], eax
mov DWORD [rbp - 56], 0
@L0:
mov r11d, DWORD [rbp - 56]
mov DWORD [rbp - 116], r11d
mov r9d, DWORD [rbp - 56]
mov r9d, r9d
add r9d, 1
@L1:
cmp r9d, 10
mov DWORD [rbp - 56], r9d
jg @L2
jmp @L0
@L2:
mov rdi, str3
mov esi, DWORD [rbp - 56]
mov eax, 0
call printf
mov DWORD [rbp - 124], eax
@E1:
leave
ret
