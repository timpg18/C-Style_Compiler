; Generated Assembly Code
section .data
float1 dd 7.9

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
sub rsp, 16
movss xmm7, [float1]
cvttss2si r11d, xmm7
mov DWORD [rbp - 8], r11d
mov r11d, DWORD [rbp - 8]
mov DWORD [rbp - 4], r11d
mov eax, DWORD [rbp - 4]
jmp @E1
@E1:
leave
ret