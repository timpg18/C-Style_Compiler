section .data
float1 dd 9.0
float2 dd 9.8
str1 db "we are passing a lot of param boiii ", 10, 0
str2 db "%d %d", 10, 0
str3 db "%d ", 10, 0
str4 db "%d ", 0

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
mov rdi, str1
mov eax, 0
call printf
mov DWORD [rbp - 8], eax
mov eax, 1
jmp @E1
@E1:
leave
ret
main:
push rbp
mov rbp, rsp
sub rsp, 208
mov edi, 1
mov esi, 2
mov edx, 4
cvtss2sd xmm0, [float1]
cvtss2sd xmm1, [float2]
mov ecx, 4
call f
mov DWORD [rbp - 112], eax
mov r11d, DWORD [rbp - 112]
mov DWORD [rbp - 4], r11d
lea r11, [rbp - 12]
mov QWORD [rbp - 120], r11
mov r11, QWORD [rbp - 120]
mov QWORD [rbp - 20], r11
lea r11, [rbp - 4]
mov QWORD [rbp - 128], r11
mov r11, QWORD [rbp - 128]
mov QWORD [rbp - 28], r11
mov r11, QWORD [rbp - 28]
mov r11, QWORD [r11]
mov DWORD [rbp - 132], r11d
mov r11d, DWORD [rbp - 132]
mov DWORD [rbp - 32], r11d
mov DWORD [rbp - 36], 2
lea r11, [rbp - 36]
mov QWORD [rbp - 140], r11
mov r11, QWORD [rbp - 140]
mov QWORD [rbp - 44], r11
lea r11, [rbp - 44]
mov QWORD [rbp - 148], r11
mov r11, QWORD [rbp - 148]
mov QWORD [rbp - 52], r11
lea r11, [rbp - 52]
mov QWORD [rbp - 156], r11
mov r11, QWORD [rbp - 156]
mov QWORD [rbp - 20], r11
mov r11, QWORD [rbp - 20]
mov r11, QWORD [r11]
mov QWORD [rbp - 164], r11
mov r11, QWORD [rbp - 164]
mov r11, QWORD [r11]
mov QWORD [rbp - 172], r11
mov r11, QWORD [rbp - 172]
mov r11, QWORD [r11]
mov DWORD [rbp - 176], r11d
mov r11d, DWORD [rbp - 176]
mov DWORD [rbp - 56], r11d
mov rdi, str2
mov esi, DWORD [rbp - 32]
mov edx, DWORD [rbp - 56]
mov eax, 0
call printf
mov DWORD [rbp - 180], eax
mov r9d, DWORD [rbp - 184]
mov r9d, 3
imul r9d, 4
lea r8, [rbp - 88] 
mov r11, r9
lea r8, [r8 + r11] 
mov r11, QWORD [rbp - 44]
mov QWORD [r8], r11
mov r11, QWORD [rbp - 44]
mov r11, QWORD [r11]
mov DWORD [rbp - 188], r11d
mov r11d, DWORD [rbp - 188]
mov DWORD [rbp - 92], r11d
mov DWORD [rbp - 184], r9d
mov rdi, str3
mov esi, DWORD [rbp - 92]
mov eax, 0
call printf
mov DWORD [rbp - 192], eax
mov DWORD [rbp - 96], 0
@L6:
mov r9d, DWORD [rbp - 96]
cmp r9d, 4
mov DWORD [rbp - 96], r9d
jl @L7
jmp @L8
@L7:
mov DWORD [rbp - 100], 0
@L3:
mov r9d, DWORD [rbp - 100]
cmp r9d, 5
mov DWORD [rbp - 100], r9d
jl @L4
jmp @L5
@L4:
mov DWORD [rbp - 104], 0
@L0:
mov r9d, DWORD [rbp - 104]
cmp r9d, 6
mov DWORD [rbp - 104], r9d
jl @L1
jmp @L2
@L1:
mov r9d, DWORD [rbp - 100]
mov r8d, DWORD [rbp - 96]
mov ecx, DWORD [rbp - 208]
mov ecx, r8d
add ecx, r9d
mov edx, DWORD [rbp - 104]
mov esi, DWORD [rbp - 212]
mov esi, ecx
add esi, edx
mov DWORD [rbp - 108], esi
mov DWORD [rbp - 208], ecx
mov DWORD [rbp - 104], edx
mov DWORD [rbp - 212], esi
mov DWORD [rbp - 96], r8d
mov DWORD [rbp - 100], r9d
mov rdi, str4
mov esi, DWORD [rbp - 108]
mov eax, 0
call printf
mov DWORD [rbp - 216], eax
mov r11d, DWORD [rbp - 104]
mov DWORD [rbp - 220], r11d
mov r9d, DWORD [rbp - 104]
mov r9d, r9d
add r9d, 1
mov DWORD [rbp - 104], r9d
jmp @L0
@L2:
mov r11d, DWORD [rbp - 100]
mov DWORD [rbp - 224], r11d
mov r9d, DWORD [rbp - 100]
mov r9d, r9d
add r9d, 1
mov DWORD [rbp - 100], r9d
jmp @L3
@L5:
mov r11d, DWORD [rbp - 96]
mov DWORD [rbp - 228], r11d
mov r9d, DWORD [rbp - 96]
mov r9d, r9d
add r9d, 1
mov DWORD [rbp - 96], r9d
jmp @L6
@L8:
@E2:
leave
ret
