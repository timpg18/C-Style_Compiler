nasm -f elf64 main.asm -o main.o
gcc -no-pie main.o -o main
./main
echo $?
