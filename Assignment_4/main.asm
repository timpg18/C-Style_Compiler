; Generated Assembly Code
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
sub rsp, 464
mov r9d, DWORD [rbp - 8]
mov r9d, 1
add r9d, 1
mov r8d, DWORD [rbp - 12]
mov r8d, r9d
add r8d, 1
mov ecx, DWORD [rbp - 16]
mov ecx, r8d
add ecx, 1
mov edx, DWORD [rbp - 20]
mov edx, ecx
add edx, 1
mov esi, DWORD [rbp - 24]
mov esi, edx
add esi, 1
mov edi, DWORD [rbp - 28]
mov edi, esi
add edi, 1
mov r10d, DWORD [rbp - 32]
mov r10d, edi
add r10d, 1
mov eax, DWORD [rbp - 36]
mov eax, r10d
add eax, 1
mov DWORD [rbp - 8], r9d
mov r9d, DWORD [rbp - 40]
mov r9d, eax
add r9d, 1
mov DWORD [rbp - 12], r8d
mov r8d, DWORD [rbp - 44]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 40], r9d
mov r9d, DWORD [rbp - 48]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 44], r8d
mov r8d, DWORD [rbp - 52]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 48], r9d
mov r9d, DWORD [rbp - 56]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 52], r8d
mov r8d, DWORD [rbp - 60]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 56], r9d
mov r9d, DWORD [rbp - 64]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 60], r8d
mov r8d, DWORD [rbp - 68]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 64], r9d
mov r9d, DWORD [rbp - 72]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 68], r8d
mov r8d, DWORD [rbp - 76]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 72], r9d
mov r9d, DWORD [rbp - 80]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 76], r8d
mov r8d, DWORD [rbp - 84]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 80], r9d
mov r9d, DWORD [rbp - 88]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 84], r8d
mov r8d, DWORD [rbp - 92]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 88], r9d
mov r9d, DWORD [rbp - 96]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 92], r8d
mov r8d, DWORD [rbp - 100]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 96], r9d
mov r9d, DWORD [rbp - 104]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 100], r8d
mov r8d, DWORD [rbp - 108]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 104], r9d
mov r9d, DWORD [rbp - 112]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 108], r8d
mov r8d, DWORD [rbp - 116]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 112], r9d
mov r9d, DWORD [rbp - 120]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 116], r8d
mov r8d, DWORD [rbp - 124]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 120], r9d
mov r9d, DWORD [rbp - 128]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 124], r8d
mov r8d, DWORD [rbp - 132]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 128], r9d
mov r9d, DWORD [rbp - 136]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 132], r8d
mov r8d, DWORD [rbp - 140]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 136], r9d
mov r9d, DWORD [rbp - 144]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 140], r8d
mov r8d, DWORD [rbp - 148]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 144], r9d
mov r9d, DWORD [rbp - 152]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 148], r8d
mov r8d, DWORD [rbp - 156]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 152], r9d
mov r9d, DWORD [rbp - 160]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 156], r8d
mov r8d, DWORD [rbp - 164]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 160], r9d
mov r9d, DWORD [rbp - 168]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 164], r8d
mov r8d, DWORD [rbp - 172]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 168], r9d
mov r9d, DWORD [rbp - 176]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 172], r8d
mov r8d, DWORD [rbp - 180]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 176], r9d
mov r9d, DWORD [rbp - 184]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 180], r8d
mov r8d, DWORD [rbp - 188]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 184], r9d
mov r9d, DWORD [rbp - 192]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 188], r8d
mov r8d, DWORD [rbp - 196]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 192], r9d
mov r9d, DWORD [rbp - 200]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 196], r8d
mov r8d, DWORD [rbp - 204]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 200], r9d
mov r9d, DWORD [rbp - 208]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 204], r8d
mov r8d, DWORD [rbp - 212]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 208], r9d
mov r9d, DWORD [rbp - 216]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 212], r8d
mov r8d, DWORD [rbp - 220]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 216], r9d
mov r9d, DWORD [rbp - 224]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 220], r8d
mov r8d, DWORD [rbp - 228]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 224], r9d
mov r9d, DWORD [rbp - 232]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 228], r8d
mov r8d, DWORD [rbp - 236]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 232], r9d
mov r9d, DWORD [rbp - 240]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 236], r8d
mov r8d, DWORD [rbp - 244]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 240], r9d
mov r9d, DWORD [rbp - 248]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 244], r8d
mov r8d, DWORD [rbp - 252]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 248], r9d
mov r9d, DWORD [rbp - 256]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 252], r8d
mov r8d, DWORD [rbp - 260]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 256], r9d
mov r9d, DWORD [rbp - 264]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 260], r8d
mov r8d, DWORD [rbp - 268]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 264], r9d
mov r9d, DWORD [rbp - 272]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 268], r8d
mov r8d, DWORD [rbp - 276]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 272], r9d
mov r9d, DWORD [rbp - 280]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 276], r8d
mov r8d, DWORD [rbp - 284]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 280], r9d
mov r9d, DWORD [rbp - 288]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 284], r8d
mov r8d, DWORD [rbp - 292]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 288], r9d
mov r9d, DWORD [rbp - 296]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 292], r8d
mov r8d, DWORD [rbp - 300]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 296], r9d
mov r9d, DWORD [rbp - 304]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 300], r8d
mov r8d, DWORD [rbp - 308]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 304], r9d
mov r9d, DWORD [rbp - 312]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 308], r8d
mov r8d, DWORD [rbp - 316]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 312], r9d
mov r9d, DWORD [rbp - 320]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 316], r8d
mov r8d, DWORD [rbp - 324]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 320], r9d
mov r9d, DWORD [rbp - 328]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 324], r8d
mov r8d, DWORD [rbp - 332]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 328], r9d
mov r9d, DWORD [rbp - 336]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 332], r8d
mov r8d, DWORD [rbp - 340]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 336], r9d
mov r9d, DWORD [rbp - 344]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 340], r8d
mov r8d, DWORD [rbp - 348]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 344], r9d
mov r9d, DWORD [rbp - 352]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 348], r8d
mov r8d, DWORD [rbp - 356]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 352], r9d
mov r9d, DWORD [rbp - 360]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 356], r8d
mov r8d, DWORD [rbp - 364]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 360], r9d
mov r9d, DWORD [rbp - 368]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 364], r8d
mov r8d, DWORD [rbp - 372]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 368], r9d
mov r9d, DWORD [rbp - 376]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 372], r8d
mov r8d, DWORD [rbp - 380]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 376], r9d
mov r9d, DWORD [rbp - 384]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 380], r8d
mov r8d, DWORD [rbp - 388]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 384], r9d
mov r9d, DWORD [rbp - 392]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 388], r8d
mov r8d, DWORD [rbp - 396]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 392], r9d
mov r9d, DWORD [rbp - 400]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 396], r8d
mov r8d, DWORD [rbp - 404]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 400], r9d
mov r9d, DWORD [rbp - 408]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 404], r8d
mov r8d, DWORD [rbp - 412]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 408], r9d
mov r9d, DWORD [rbp - 416]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 412], r8d
mov r8d, DWORD [rbp - 420]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 416], r9d
mov r9d, DWORD [rbp - 424]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 420], r8d
mov r8d, DWORD [rbp - 428]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 424], r9d
mov r9d, DWORD [rbp - 432]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 428], r8d
mov r8d, DWORD [rbp - 436]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 432], r9d
mov r9d, DWORD [rbp - 440]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 436], r8d
mov r8d, DWORD [rbp - 444]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 440], r9d
mov r9d, DWORD [rbp - 448]
mov r9d, r8d
add r9d, 1
mov DWORD [rbp - 444], r8d
mov r8d, DWORD [rbp - 452]
mov r8d, r9d
add r8d, 1
mov DWORD [rbp - 4], r8d
mov eax, DWORD [rbp - 4]
jmp @E1
@E1:
mov DWORD [rbp - 36], eax
mov DWORD [rbp - 16], ecx
mov DWORD [rbp - 28], edi
mov DWORD [rbp - 20], edx
mov DWORD [rbp - 24], esi
mov DWORD [rbp - 32], r10d
mov DWORD [rbp - 452], r8d
mov DWORD [rbp - 448], r9d
leave
ret

