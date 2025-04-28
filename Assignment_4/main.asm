; Generated Assembly Code
section .data
str1 db "%d %c k", 0
str2 db "%d %c ", 10, 0

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
sub rsp, 112
mov r9d, DWORD [rbp - 80]
mov r9d, 0
imul r9d, 4
lea r8, [rbp - 20] 
mov r11, r9
lea r8, [r8 + r11] 
mov DWORD [r8] , 10
mov r8d, DWORD [rbp - 84]
mov r8d, 5
imul r8d, 1
lea rcx, [rbp - 40] 
mov r11, r8
lea rcx, [rcx + r11] 
mov BYTE [rcx] , 107
mov ecx, DWORD [rbp - 88]
mov ecx, 0
imul ecx, 4
lea rdx, [rbp - 20] 
lea rdx, [rdx + rcx] 
mov edx, DWORD [rdx]
mov DWORD [rbp - 44], edx
mov edx, DWORD [rbp - 92]
mov edx, 5
imul edx, 1
lea rsi, [rbp - 40] 
lea rsi, [rsi + rdx] 
mov sil, BYTE [rsi]
mov BYTE [rbp - 45], sil
mov DWORD [rbp - 88], ecx
mov DWORD [rbp - 92], edx
mov DWORD [rbp - 84], r8d
mov DWORD [rbp - 80], r9d
mov rdi, str1
mov esi, DWORD [rbp - 44]
mov dl, BYTE [rbp - 45]
mov eax, 0
call printf
mov DWORD [rbp - 96], eax
mov r9d, DWORD [rbp - 100]
mov r9d, 0
add r9d, 0
lea r8, [rbp - 60] 
mov r11, r9
lea r8, [r8 + r11] 
mov QWORD [r8] , 1
mov r8d, DWORD [rbp - 104]
mov r8d, 0
add r8d, 4
lea rcx, [rbp - 60] 
mov r11, r8
lea rcx, [rcx + r11] 
mov QWORD [rcx] , 107
mov ecx, DWORD [rbp - 108]
mov ecx, 0
add ecx, 0
lea rdx, [rbp - 60] 
lea rdx, [rdx + rcx] 
mov rdx, QWORD [rdx]
mov DWORD [rbp - 64], edx
mov edx, DWORD [rbp - 112]
mov edx, 0
add edx, 4
lea rsi, [rbp - 60] 
lea rsi, [rsi + rdx] 
mov rsi, QWORD [rsi]
mov BYTE [rbp - 65], sil
mov DWORD [rbp - 108], ecx
mov DWORD [rbp - 112], edx
mov DWORD [rbp - 104], r8d
mov DWORD [rbp - 100], r9d
mov rdi, str2
mov esi, DWORD [rbp - 64]
mov dl, BYTE [rbp - 65]
mov eax, 0
call printf
mov DWORD [rbp - 116], eax
mov r9d, DWORD [rbp - 120]
mov r9d, 0
add r9d, 0
lea r8, [rbp - 72] 
mov r11, r9
lea r8, [r8 + r11] 
mov QWORD [r8] , 5
mov r8d, DWORD [rbp - 124]
mov r8d, 0
add r8d, 0
lea rcx, [rbp - 72] 
mov r11, r8
lea rcx, [rcx + r11] 
mov QWORD [rcx] , 107
mov ecx, DWORD [rbp - 128]
mov ecx, 0
add ecx, 0
lea rdx, [rbp - 72] 
lea rdx, [rdx + rcx] 
mov rdx, QWORD [rdx]
mov DWORD [rbp - 64], edx
mov edx, DWORD [rbp - 132]
mov edx, 0
add edx, 0
lea rsi, [rbp - 72] 
lea rsi, [rsi + rdx] 
mov rsi, QWORD [rsi]
mov BYTE [rbp - 65], sil
mov DWORD [rbp - 128], ecx
mov DWORD [rbp - 132], edx
mov DWORD [rbp - 124], r8d
mov DWORD [rbp - 120], r9d
mov rdi, str2
mov esi, DWORD [rbp - 64]
mov dl, BYTE [rbp - 65]
mov eax, 0
call printf
mov DWORD [rbp - 136], eax
mov QWORD [rbp - 76], 1
mov DWORD [rbp - 64], 1
mov rdi, str2
mov esi, DWORD [rbp - 64]
mov dl, BYTE [rbp - 65]
mov eax, 0
call printf
mov DWORD [rbp - 140], eax
@E1:
leave
ret