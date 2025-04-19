section .text
global main

main:
push rbp
mov rbp, rsp
mov DWORD [rbp - 4], 1
mov eax, DWORD [rbp - 4]
mov ecx, DWORD [rbp - 4]
mov ecx, ecx
add ecx, 1
mov edx, ecx
mov ecx, ecx
add ecx, 1
mov eax, ecx
pop rbp
ret