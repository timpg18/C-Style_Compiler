; Generated Assembly Code
<<<<<<< HEAD
=======
section .data
str1 db "%d", 0

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
mov r9d, DWORD [rbp - 28]
mov r9d, 0
imul r9d, 4
lea r8, [rbp - 20] 
lea r8, [r8 + r9d] 
mov DWORD [r8] , 2
mov r8d, DWORD [rbp - 32]
mov r8d, 0
imul r8d, 4
lea rcx, [rbp - 20] 
lea rcx, [rcx + r8d] 
mov rcx, DWORD [rcx]
mov DWORD [rbp - 24], rcx
mov DWORD [rbp - 32], r8d
mov DWORD [rbp - 28], r9d
mov rdi, str1
mov esi, DWORD [rbp - 24]
call printf
mov DWORD [rbp - 36], eax
>>>>>>> e0f8ed7c1e45a357bb6b13127ae841a14a86ffbb
@E1:
leave
ret