#!/bin/bash

# Navigate to the script's directory
cd "$(dirname "$0")" || exit 1

# Step 1: Build the parser
echo "Building the Parser..."
make

# Check if build was successful
if [ ! -f "./parser" ]; then
    echo "Error: Parser executable not found. Build failed."
    exit 1
fi

# Step 2: Prepare output directory
mkdir -p ./nasm_assembly

# Step 3: Run the parser on all test cases
echo "Running parser on test cases..."
counter=1
for testfile in ./testing/*.c; do
    echo "-----------------------------------"
    echo "Processing: $testfile"
    echo "-----------------------------------"

    # Run the parser and capture its output
    full_output=$(./parser "$testfile")

    # Print only the lines BEFORE the marker
    echo "$full_output" | awk '/; Generated Assembly Code/ {exit} {print}'

    # Save only the lines AFTER the marker
    echo "$full_output" | awk '/; Generated Assembly Code/ {found=1; next} found' > "./nasm_assembly/main${counter}.asm"

    echo "Saved assembly to ./nasm_assembly/main${counter}.asm"
    counter=$((counter + 1))
    echo -e "\n"
done

echo "All test cases processed."

