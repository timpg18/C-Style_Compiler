section .data
float1 dd 9.0
float2 dd 7.0
str1 db "%d %c k ", 10, 0
str2 db "%d %c ", 10, 0
str3 db "%f %f", 0
str4 db "%d %f", 10, 0

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
sub rsp, 272
mov r9d, DWORD [rbp - 180]
mov r9d, 0
imul r9d, 4
lea r8, [rbp - 20] 
mov r11, r9
lea r8, [r8 + r11] 
mov DWORD [r8] , 10
mov r8d, DWORD [rbp - 184]
mov r8d, 5
imul r8d, 1
lea rcx, [rbp - 40] 
mov r11, r8
lea rcx, [rcx + r11] 
mov BYTE [rcx] , 107
mov ecx, DWORD [rbp - 188]
mov ecx, 0
imul ecx, 4
lea rdx, [rbp - 20] 
lea rdx, [rdx + rcx] 
mov edx, DWORD [rdx]
mov DWORD [rbp - 44], edx
mov edx, DWORD [rbp - 192]
mov edx, 1
imul edx, 4
lea rsi, [rbp - 20] 
mov r11, rdx
lea rsi, [rsi + r11] 
mov DWORD [rsi] , 1
mov esi, DWORD [rbp - 196]
mov esi, 2
imul esi, 4
lea rdi, [rbp - 20] 
mov r11, rsi
lea rdi, [rdi + r11] 
mov DWORD [rdi] , 2
mov edi, DWORD [rbp - 200]
mov edi, 5
imul edi, 1
lea r10, [rbp - 40] 
lea r10, [r10 + rdi] 
mov r10b, BYTE [r10]
mov BYTE [rbp - 45], r10b
mov DWORD [rbp - 188], ecx
mov DWORD [rbp - 200], edi
mov DWORD [rbp - 192], edx
mov DWORD [rbp - 196], esi
mov DWORD [rbp - 184], r8d
mov DWORD [rbp - 180], r9d
mov rdi, str1
mov esi, DWORD [rbp - 44]
mov dl, BYTE [rbp - 45]
mov eax, 0
call printf
mov DWORD [rbp - 204], eax
mov r9d, DWORD [rbp - 208]
mov r9d, 0
add r9d, 0
lea r8, [rbp - 60] 
mov r11, r9
lea r8, [r8 + r11] 
mov QWORD [r8] , 1
mov r8d, DWORD [rbp - 212]
mov r8d, 0
add r8d, 4
lea rcx, [rbp - 60] 
mov r11, r8
lea rcx, [rcx + r11] 
mov QWORD [rcx] , 107
mov ecx, DWORD [rbp - 216]
mov ecx, 0
add ecx, 0
lea rdx, [rbp - 60] 
lea rdx, [rdx + rcx] 
mov rdx, QWORD [rdx]
mov DWORD [rbp - 64], edx
mov edx, DWORD [rbp - 220]
mov edx, 0
add edx, 4
lea rsi, [rbp - 60] 
lea rsi, [rsi + rdx] 
mov rsi, QWORD [rsi]
mov BYTE [rbp - 65], sil
mov DWORD [rbp - 216], ecx
mov DWORD [rbp - 220], edx
mov DWORD [rbp - 212], r8d
mov DWORD [rbp - 208], r9d
mov rdi, str2
mov esi, DWORD [rbp - 64]
mov dl, BYTE [rbp - 65]
mov eax, 0
call printf
mov DWORD [rbp - 224], eax
mov r9d, DWORD [rbp - 228]
mov r9d, 0
add r9d, 0
lea r8, [rbp - 72] 
mov r11, r9
lea r8, [r8 + r11] 
mov QWORD [r8] , 5
mov r8d, DWORD [rbp - 232]
mov r8d, 0
add r8d, 0
lea rcx, [rbp - 72] 
mov r11, r8
lea rcx, [rcx + r11] 
mov QWORD [rcx] , 107
mov ecx, DWORD [rbp - 236]
mov ecx, 0
add ecx, 0
lea rdx, [rbp - 72] 
lea rdx, [rdx + rcx] 
mov rdx, QWORD [rdx]
mov DWORD [rbp - 64], edx
mov edx, DWORD [rbp - 240]
mov edx, 0
add edx, 0
lea rsi, [rbp - 72] 
lea rsi, [rsi + rdx] 
mov rsi, QWORD [rsi]
mov BYTE [rbp - 65], sil
mov DWORD [rbp - 236], ecx
mov DWORD [rbp - 240], edx
mov DWORD [rbp - 232], r8d
mov DWORD [rbp - 228], r9d
mov rdi, str2
mov esi, DWORD [rbp - 64]
mov dl, BYTE [rbp - 65]
mov eax, 0
call printf
mov DWORD [rbp - 244], eax
mov QWORD [rbp - 76], 1
mov DWORD [rbp - 64], 1
mov rdi, str2
mov esi, DWORD [rbp - 64]
mov dl, BYTE [rbp - 65]
mov eax, 0
call printf
mov DWORD [rbp - 248], eax
mov r9d, DWORD [rbp - 252]
mov r9d, 0
add r9d, 8
mov r8d, DWORD [rbp - 256]
mov r8d, r9d
add r8d, 1
lea rcx, [rbp - 100] 
mov r11, r8
lea rcx, [rcx + r11] 
mov QWORD [rcx] , 5
mov ecx, DWORD [rbp - 260]
mov ecx, 0
add ecx, 8
mov edx, DWORD [rbp - 264]
mov edx, ecx
add edx, 0
lea rsi, [rbp - 100] 
mov r11, rdx
lea rsi, [rsi + r11] 
mov QWORD [rsi] , 98
mov esi, DWORD [rbp - 268]
mov esi, 0
add esi, 8
mov edi, DWORD [rbp - 272]
mov edi, esi
add edi, 1
lea r10, [rbp - 100] 
lea r10, [r10 + rdi] 
mov r10, QWORD [r10]
mov DWORD [rbp - 104], r10d
mov BYTE [rbp - 105], 98
mov DWORD [rbp - 260], ecx
mov DWORD [rbp - 272], edi
mov DWORD [rbp - 264], edx
mov DWORD [rbp - 268], esi
mov DWORD [rbp - 256], r8d
mov DWORD [rbp - 252], r9d
mov rdi, str2
mov esi, DWORD [rbp - 104]
mov dl, BYTE [rbp - 105]
mov eax, 0
call printf
mov DWORD [rbp - 276], eax
mov r9d, DWORD [rbp - 280]
mov r9d, 0
imul r9d, 4
lea r8, [rbp - 148] 
mov r11, r9
lea r8, [r8 + r11] 
mov DWORD [r8] , 9
mov r8d, DWORD [rbp - 284]
mov r8d, 2
imul r8d, 4
lea rcx, [rbp - 148] 
mov r11, r8
lea rcx, [rcx + r11] 
mov DWORD [rcx] , 10
mov ecx, DWORD [rbp - 288]
mov ecx, 1
imul ecx, 4
mov edx, DWORD [rbp - 292]
mov edx, 2
imul edx, 4
lea rsi, [rbp - 148] 
lea rsi, [rsi + rcx] 
mov esi, DWORD [rsi]
mov DWORD [rbp - 296], esi
lea rsi, [rbp - 148] 
lea rsi, [rsi + rdx] 
mov esi, DWORD [rsi]
mov DWORD [rbp - 300], esi
mov esi, DWORD [rbp - 300]
mov edi, DWORD [rbp - 296]
mov r10d, DWORD [rbp - 304]
mov r10d, edi
add r10d, esi
mov DWORD [rbp - 152], r10d
movss xmm7, [float1]
cvtss2sd xmm7, xmm7
movsd QWORD [rbp - 312], xmm7
movsd xmm7, QWORD [rbp - 312]
movsd QWORD [rbp - 160], xmm7 
movss xmm7, [float2]
cvtss2sd xmm7, xmm7
movsd QWORD [rbp - 320], xmm7
movsd xmm7, QWORD [rbp - 320]
movsd QWORD [rbp - 168], xmm7 
movsd xmm6, [rbp - 168]
movsd xmm5, [rbp - 160]
movsd xmm4, [rbp - 328]
movsd xmm4, xmm5
addsd  xmm4, xmm6
movsd QWORD [rbp - 176], xmm4
mov DWORD [rbp - 288], ecx
mov DWORD [rbp - 296], edi
mov DWORD [rbp - 292], edx
mov DWORD [rbp - 300], esi
mov DWORD [rbp - 304], r10d
mov DWORD [rbp - 284], r8d
mov DWORD [rbp - 280], r9d
movsd QWORD [rbp - 328], xmm4
movsd QWORD [rbp - 160], xmm5
movsd QWORD [rbp - 168], xmm6
mov rdi, str3
movsd xmm0, QWORD [rbp - 160]
movsd xmm1, QWORD [rbp - 168]
mov eax, 2
call printf
mov DWORD [rbp - 332], eax
mov rdi, str4
mov esi, DWORD [rbp - 152]
movsd xmm0, QWORD [rbp - 176]
mov eax, 1
call printf
mov DWORD [rbp - 336], eax
@E2:
leave
ret
