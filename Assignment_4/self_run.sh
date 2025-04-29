#!/bin/bash

# Check if file_name argument is provided
if [ -z "$1" ]; then
    echo "Error: No file_name provided."
    echo "Usage: ./run_parser.sh <file_name>"
    exit 1
fi

# Set the file name argument
file_name="$1"

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

# Step 2: Run the parser on the provided file
echo "Running parser on test case: $file_name"
echo "-----------------------------------"

# Run the parser and capture its output
full_output=$(./parser "$file_name")

# Print only the lines BEFORE the marker
echo "$full_output" | awk '/; Generated Assembly Code/ {exit} {print}'

# Save only the lines AFTER the marker to main.nasm in the current directory
echo "$full_output" | awk '/; Generated Assembly Code/ {found=1; next} found' > "./main.nasm"

echo "Saved assembly to ./main.nasm"

echo "Test case processed."
