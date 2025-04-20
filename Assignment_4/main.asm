section .data
    msg db "Value is: %d", 10, 0   ; format string with newline

section .text
    global main
    extern printf

main:
    push rbp
    mov rbp, rsp

    mov edi, msg          ; 1st argument to printf (format string)
    mov esi, 42           ; 2nd argument to printf (%d → 42)
    xor eax, eax          ; clear rax (for variadic functions)
    call printf           ; call printf("Value is: %d", 42)

    mov eax, 0            ; return 0 from main
    pop rbp
    ret
