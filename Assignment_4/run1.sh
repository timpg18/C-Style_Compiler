#!/bin/bash

# Define the directory where 3AC files are stored
IR_DIR="intermediate_representation"

# Create the directory if it doesn't exist
mkdir -p "$IR_DIR"

# Compile the C++ program
echo "Compiling label processor..."
g++ -o label_processor label_processor.cpp -std=c++11

# Check if compilation was successful
if [ $? -ne 0 ]; then
    echo "Compilation failed"
    exit 1
fi

echo "Compilation successful"

# Process all .3ac files in the intermediate_representation directory
for input_file in "$IR_DIR"/*.tac; do
    if [ -f "$input_file" ]; then
        filename=$(basename -- "$input_file")
        output_file="$IR_DIR/${filename%.3ac}"
        
        echo "Processing $filename..."
        ./label_processor "$input_file" "$output_file"
        
        if [ $? -eq 0 ]; then
            echo "Successfully processed $filename to $(basename -- "$output_file")"
        else
            echo "Failed to process $filename"
        fi
    fi
done

# If no .3ac files found, inform the user
if [ ! "$(ls -A "$IR_DIR"/*.tac 2>/dev/null)" ]; then
    echo "No .3ac files found in $IR_DIR"
    echo "Please place your 3AC files in the $IR_DIR directory with .tac extension"
fi

echo "All done!"