section .data
<<<<<<< HEAD
float1 dd 9.0
float2 dd 9.8
=======
float1 dd 7.0
str1 db "%d %c k ", 10, 0
str2 db "%d %f ", 10, 0
str3 db "%d %c ", 10, 0
>>>>>>> origin/codegen

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
<<<<<<< HEAD
sub rsp, 16
mov DWORD [rbp - 4], edi
mov eax, 0
jmp @E1
=======
sub rsp, 224
mov r9d, DWORD [rbp - 124]
mov r9d, 0
imul r9d, 4
lea r8, [rbp - 20] 
mov r11, r9
lea r8, [r8 + r11] 
mov DWORD [r8] , 10
mov r8d, DWORD [rbp - 128]
mov r8d, 5
imul r8d, 1
lea rcx, [rbp - 40] 
mov r11, r8
lea rcx, [rcx + r11] 
mov BYTE [rcx] , 107
mov ecx, DWORD [rbp - 132]
mov ecx, 0
imul ecx, 4
lea rdx, [rbp - 20] 
lea rdx, [rdx + rcx] 
mov edx, DWORD [rdx]
mov DWORD [rbp - 44], edx
mov edx, DWORD [rbp - 136]
mov edx, 1
imul edx, 4
lea rsi, [rbp - 20] 
mov r11, rdx
lea rsi, [rsi + r11] 
mov DWORD [rsi] , 1
mov esi, DWORD [rbp - 140]
mov esi, 2
imul esi, 4
lea rdi, [rbp - 20] 
mov r11, rsi
lea rdi, [rdi + r11] 
mov DWORD [rdi] , 2
mov edi, DWORD [rbp - 144]
mov edi, 5
imul edi, 1
lea r10, [rbp - 40] 
lea r10, [r10 + rdi] 
mov r10b, BYTE [r10]
mov BYTE [rbp - 45], r10b
mov DWORD [rbp - 132], ecx
mov DWORD [rbp - 144], edi
mov DWORD [rbp - 136], edx
mov DWORD [rbp - 140], esi
mov DWORD [rbp - 128], r8d
mov DWORD [rbp - 124], r9d
mov rdi, str1
mov esi, DWORD [rbp - 44]
mov dl, BYTE [rbp - 45]
mov eax, 0
call printf
mov DWORD [rbp - 148], eax
mov r9d, DWORD [rbp - 152]
mov r9d, 1
imul r9d, 4
mov r8d, DWORD [rbp - 156]
mov r8d, 2
imul r8d, 4
lea rcx, [rbp - 20] 
lea rcx, [rcx + r9] 
mov ecx, DWORD [rcx]
mov DWORD [rbp - 160], ecx
lea rcx, [rbp - 20] 
lea rcx, [rcx + r8] 
mov ecx, DWORD [rcx]
mov DWORD [rbp - 164], ecx
mov ecx, DWORD [rbp - 164]
mov edx, DWORD [rbp - 160]
mov esi, DWORD [rbp - 168]
mov esi, edx
add esi, ecx
mov DWORD [rbp - 52], esi
movss xmm7, [float1]
cvtss2sd xmm7, xmm7
movsd QWORD [rbp - 176], xmm7
movsd xmm7, QWORD [rbp - 176]
movsd QWORD [rbp - 60], xmm7 
mov DWORD [rbp - 164], ecx
mov DWORD [rbp - 160], edx
mov DWORD [rbp - 168], esi
mov DWORD [rbp - 156], r8d
mov DWORD [rbp - 152], r9d
mov rdi, str2
mov esi, DWORD [rbp - 52]
movsd xmm0, QWORD [rbp - 60]
mov eax, 1
call printf
mov DWORD [rbp - 180], eax
mov r9d, DWORD [rbp - 184]
mov r9d, 0
add r9d, 0
lea r8, [rbp - 72] 
mov r11, r9
lea r8, [r8 + r11] 
mov QWORD [r8] , 1
mov r8d, DWORD [rbp - 188]
mov r8d, 0
add r8d, 4
lea rcx, [rbp - 72] 
mov r11, r8
lea rcx, [rcx + r11] 
mov QWORD [rcx] , 107
mov ecx, DWORD [rbp - 192]
mov ecx, 0
add ecx, 0
lea rdx, [rbp - 72] 
lea rdx, [rdx + rcx] 
mov rdx, QWORD [rdx]
mov DWORD [rbp - 76], edx
mov edx, DWORD [rbp - 196]
mov edx, 0
add edx, 4
lea rsi, [rbp - 72] 
lea rsi, [rsi + rdx] 
mov rsi, QWORD [rsi]
mov BYTE [rbp - 77], sil
mov DWORD [rbp - 192], ecx
mov DWORD [rbp - 196], edx
mov DWORD [rbp - 188], r8d
mov DWORD [rbp - 184], r9d
mov rdi, str3
mov esi, DWORD [rbp - 76]
mov dl, BYTE [rbp - 77]
mov eax, 0
call printf
mov DWORD [rbp - 200], eax
mov r9d, DWORD [rbp - 204]
mov r9d, 0
add r9d, 0
lea r8, [rbp - 84] 
mov r11, r9
lea r8, [r8 + r11] 
mov QWORD [r8] , 5
mov r8d, DWORD [rbp - 208]
mov r8d, 0
add r8d, 0
lea rcx, [rbp - 84] 
mov r11, r8
lea rcx, [rcx + r11] 
mov QWORD [rcx] , 107
mov ecx, DWORD [rbp - 212]
mov ecx, 0
add ecx, 0
lea rdx, [rbp - 84] 
lea rdx, [rdx + rcx] 
mov rdx, QWORD [rdx]
mov DWORD [rbp - 76], edx
mov edx, DWORD [rbp - 216]
mov edx, 0
add edx, 0
lea rsi, [rbp - 84] 
lea rsi, [rsi + rdx] 
mov rsi, QWORD [rsi]
mov BYTE [rbp - 77], sil
mov DWORD [rbp - 212], ecx
mov DWORD [rbp - 216], edx
mov DWORD [rbp - 208], r8d
mov DWORD [rbp - 204], r9d
mov rdi, str3
mov esi, DWORD [rbp - 76]
mov dl, BYTE [rbp - 77]
mov eax, 0
call printf
mov DWORD [rbp - 220], eax
mov QWORD [rbp - 88], 1
mov DWORD [rbp - 76], 1
mov rdi, str3
mov esi, DWORD [rbp - 76]
mov dl, BYTE [rbp - 77]
mov eax, 0
call printf
mov DWORD [rbp - 224], eax
mov r9d, DWORD [rbp - 228]
mov r9d, 0
add r9d, 8
mov r8d, DWORD [rbp - 232]
mov r8d, r9d
add r8d, 1
lea rcx, [rbp - 112] 
mov r11, r8
lea rcx, [rcx + r11] 
mov QWORD [rcx] , 5
mov ecx, DWORD [rbp - 236]
mov ecx, 0
add ecx, 8
mov edx, DWORD [rbp - 240]
mov edx, ecx
add edx, 0
lea rsi, [rbp - 112] 
mov r11, rdx
lea rsi, [rsi + r11] 
mov QWORD [rsi] , 98
mov esi, DWORD [rbp - 244]
mov esi, 0
add esi, 8
mov edi, DWORD [rbp - 248]
mov edi, esi
add edi, 1
lea r10, [rbp - 112] 
lea r10, [r10 + rdi] 
mov r10, QWORD [r10]
mov DWORD [rbp - 116], r10d
mov BYTE [rbp - 117], 98
mov DWORD [rbp - 236], ecx
mov DWORD [rbp - 248], edi
mov DWORD [rbp - 240], edx
mov DWORD [rbp - 244], esi
mov DWORD [rbp - 232], r8d
mov DWORD [rbp - 228], r9d
mov rdi, str3
mov esi, DWORD [rbp - 116]
mov dl, BYTE [rbp - 117]
mov eax, 0
call printf
mov DWORD [rbp - 252], eax
>>>>>>> origin/codegen
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
