section .data
float1 dd 9.0
float2 dd 7.0
str1 db "%f", 0

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
sub rsp, 96
mov r9d, DWORD [rbp - 72]
mov r9d, 0
imul r9d, 4
lea r8, [rbp - 40] 
mov r11, r9
lea r8, [r8 + r11] 
mov DWORD [r8] , 9
mov r8d, DWORD [rbp - 76]
mov r8d, 2
imul r8d, 4
lea rcx, [rbp - 40] 
mov r11, r8
lea rcx, [rcx + r11] 
mov DWORD [rcx] , 10
mov ecx, DWORD [rbp - 80]
mov ecx, 1
imul ecx, 4
mov edx, DWORD [rbp - 84]
mov edx, 2
imul edx, 4
lea rsi, [rbp - 40] 
lea rsi, [rsi + rcx] 
mov esi, DWORD [rsi]
mov DWORD [rbp - 88], esi
lea rsi, [rbp - 40] 
lea rsi, [rsi + rdx] 
mov esi, DWORD [rsi]
mov DWORD [rbp - 92], esi
mov esi, DWORD [rbp - 92]
mov edi, DWORD [rbp - 88]
mov r10d, DWORD [rbp - 96]
mov r10d, edi
add r10d, esi
mov DWORD [rbp - 44], r10d
movss xmm7, [float1]
cvtss2sd xmm7, xmm7
movsd QWORD [rbp - 104], xmm7
movsd xmm7, QWORD [rbp - 104]
movsd QWORD [rbp - 52], xmm7 
movss xmm7, [float2]
cvtss2sd xmm7, xmm7
movsd QWORD [rbp - 112], xmm7
movsd xmm7, QWORD [rbp - 112]
movsd QWORD [rbp - 60], xmm7 
movsd xmm6, [rbp - 60]
movsd xmm5, [rbp - 52]
movsd xmm4, [rbp - 120]
movsd xmm4, xmm5
addsd  xmm4, xmm6
movsd QWORD [rbp - 68], xmm4
mov DWORD [rbp - 80], ecx
mov DWORD [rbp - 88], edi
mov DWORD [rbp - 84], edx
mov DWORD [rbp - 92], esi
mov DWORD [rbp - 96], r10d
mov DWORD [rbp - 76], r8d
mov DWORD [rbp - 72], r9d
movsd QWORD [rbp - 120], xmm4
movsd QWORD [rbp - 52], xmm5
movsd QWORD [rbp - 60], xmm6
mov rdi, str1
movsd xmm0, QWORD [rbp - 68]
mov eax, 1
call printf
mov DWORD [rbp - 124], eax
movsd xmm7, QWORD [rbp - 68]
cvttsd2si r11d, xmm7
mov DWORD [rbp - 128], r11d
mov eax, DWORD [rbp - 128]
jmp @E1
@E1:
leave
ret
