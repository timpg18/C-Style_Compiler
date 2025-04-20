section .text
global main

main:
push rbp
mov rbp, rsp
mov DWORD [rbp - 8], 1
@L0:
mov eax, DWORD [rbp - 8]
cmp eax, 10
mov DWORD [rbp - 8], eax
jl @L1
jmp @L2
@L1:
mov ecx, DWORD [rbp - 4]
mov edx, DWORD [rbp - 8]
mov eax, ecx
add eax, edx
mov ecx, eax
mov esi, edx
add esi, 1
mov edx, esi
mov DWORD [rbp - 4], ecx
mov DWORD [rbp - 8], edx
jmp @L0
@L2:
mov eax, DWORD [rbp - 4]
pop rbp
ret