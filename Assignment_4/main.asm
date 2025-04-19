section .text
global main

main:
push rbp
mov rbp, rsp
mov DWORD [rbp - 8], 1
mov eax, 8
imul eax, 7
mov edx, DWORD [rbp - 8]
mov ecx, edx
add ecx, eax
mov esi, ecx
add esi, 20
mov edx, esi
mov eax, edx
pop rbp
ret