#!/bin/bash

# Navigate to the script's directory (root of the project)
cd "$(dirname "$0")" || exit 1

# Go to the nasm_assembly directory (relative to the script)
cd nasm_assembly || exit 1

# Loop through all .asm files in nasm_assembly/
for asm_file in main*.asm; do
    echo "Processing: $asm_file"
    
    # Step 1: Assemble the .asm file into .o
    nasm -f elf64 "$asm_file" -o "${asm_file%.asm}.o"
    
    # Step 2: Link the .o file into an executable
    ld -m elf_x86_64 \
       --dynamic-linker /lib64/ld-linux-x86-64.so.2 \
       -lc "${asm_file%.asm}.o" -o "${asm_file%.asm}"
    
    # Step 3: Run the executable with input 51 and print the result
    ./"${asm_file%.asm}" 51
    echo "Exit code: $?"  # Print the exit code from the program
    
    echo "-----------------------------------"
done

echo "All .asm files processed."
