section .text
global main

main:
push rbp
mov rbp, rsp
mov DWORD [rbp - 4], 7
mov ecx, DWORD [rbp - 4]
mov eax, ecx
imul eax, 9
mov ecx, eax
mov eax, ecx
pop rbp
ret