section .text
global main

main:
push rbp
mov rbp, rsp
mov DWORD [rbp - 4], 0
mov DWORD [rbp - 8], 1
@L0:
mov eax, DWORD [rbp - 8]
cmp eax, 10
jle @L1
jmp @L2
@L1:
mov edx, DWORD [rbp - 4]
mov ecx, edx
add ecx, eax
mov edx, ecx
mov esi, eax
add esi, 1
mov eax, esi
jmp @L0
@L2:
mov eax, edx
pop rbp
ret