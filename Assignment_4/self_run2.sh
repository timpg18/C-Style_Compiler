#!/bin/bash

# Navigate to the script's directory (root of the project)
cd "$(dirname "$0")" || exit 1

# Step 1: Assemble the main.nasm file into .o
echo "Assembling main.nasm into object file..."
nasm -f elf64 main.nasm -o main.o


ld -m elf_x86_64 \
   --dynamic-linker /lib64/ld-linux-x86-64.so.2 \
   -lc main.o -o main

# Step 3: Run the executable with input 51 and print the result
echo "Running the executable with input 51..."
./main 51

# Step 4: Print the exit code from the program
echo "Exit code: $?"  # Print the exit code from the program

echo "Processing complete."
