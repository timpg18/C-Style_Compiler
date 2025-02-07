#!/bin/bash

# Navigate to script directory (ensure it runs from the correct location)
cd "$(dirname "$0")"

# Step 1: Build the lexer
echo "Building the lexer..."
make

# Check if build was successful
if [ ! -f "./lexer" ]; then
    echo "Error: Lexer executable not found. Build failed."
    exit 1
fi

# Step 2: Run the lexer on all test cases
echo "Running lexer on test cases..."
for testfile in ./testing/*.c; do
    echo "-----------------------------------"
    echo "Processing: $testfile"
    echo "-----------------------------------"
    ./lexer < "$testfile"
    echo -e "\n"
done

echo "All test cases processed."
