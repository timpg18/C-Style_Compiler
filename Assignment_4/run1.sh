nasm -f elf64 main.asm -o main.o
ld -m elf_x86_64 \
   --dynamic-linker /lib64/ld-linux-x86-64.so.2 \
   -lc main.o -o main
 ./main
echo $?
