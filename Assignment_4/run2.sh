#!/bin/bash

# Compile the C++ program
echo "Compiling address allocation table program..."
g++ -std=c++17 -o address_allocation_table address_allocation_table.cpp

# Check if compilation was successful
if [ $? -ne 0 ]; then
    echo "Compilation failed."
    exit 1
fi

# Run the program
echo "Running address allocation table analysis on all files in intermediate_representation directory..."
./address_allocation_table