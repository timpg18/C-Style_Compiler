#!/bin/bash

# Navigate to script directory (ensure it runs from the correct location)
cd "$(dirname "$0")"

# Step 1: Build the parser
echo "Building the Parser..."
make

# Check if build was successful
if [ ! -f "./parser" ]; then
    echo "Error: Parser executable not found. Build failed."
    exit 1
fi

# Step 2: Run the lexer on all test cases
echo "Running lexer on test cases..."
for testfile in ./testing/*.c; do
    echo "-----------------------------------"
    echo "Processing: $testfile"
    echo "-----------------------------------"
    ./parser < "$testfile"
    echo -e "\n"
done

echo "All test cases processed."
