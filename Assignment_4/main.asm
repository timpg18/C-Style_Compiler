; Generated Assembly Code
<<<<<<< HEAD
=======
section .data
str1 db "%d %c 5", 10, 0

>>>>>>> e0f8ed7c1e45a357bb6b13127ae841a14a86ffbb
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
<<<<<<< HEAD
sub rsp, 16
mov DWORD [rbp - 4], 5
cmp DWORD[rbp - 4], 1
je @L0
cmp DWORD[rbp - 4], 2
je @L1
jmp @L2
@L0:
mov DWORD [rbp - 4], 2
jmp @L3
@L1:
mov DWORD [rbp - 4], 3
jmp @L3
@L2:
mov DWORD [rbp - 4], 7
jmp @L3
@L3:
mov eax, DWORD [rbp - 4]
jmp @E1
=======
sub rsp, 32
mov r9d, DWORD [rbp - 44]
mov r9d, 0
imul r9d, 4
lea r8, [rbp - 20] 
mov r11, r9
lea r8, [r8 + r11] 
mov DWORD [r8] , 2
mov r8d, DWORD [rbp - 48]
mov r8d, 0
imul r8d, 4
lea rcx, [rbp - 20] 
lea rcx, [rcx + r8] 
mov ecx, DWORD [rcx]
mov DWORD [rbp - 24], ecx
mov ecx, DWORD [rbp - 52]
mov ecx, 8
imul ecx, 1
lea rdx, [rbp - 34] 
mov r11, rcx
lea rdx, [rdx + r11] 
mov BYTE [rdx] , 51
mov edx, DWORD [rbp - 56]
mov edx, 8
imul edx, 1
lea rsi, [rbp - 34] 
lea rsi, [rsi + rdx] 
mov sil, BYTE [rsi]
mov BYTE [rbp - 37], sil
mov DWORD [rbp - 52], ecx
mov DWORD [rbp - 56], edx
mov DWORD [rbp - 48], r8d
mov DWORD [rbp - 44], r9d
mov rdi, str1
mov esi, DWORD [rbp - 24]
mov dl, BYTE [rbp - 37]
call printf
<<<<<<< HEAD
mov DWORD [rbp - 36], eax
>>>>>>> e0f8ed7c1e45a357bb6b13127ae841a14a86ffbb
=======
mov DWORD [rbp - 60], eax
>>>>>>> 4786886 (arrays)
@E1:
leave
ret