#!/bin/bash

# Colors for better output
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Check if IR directory exists, if not create it
IR_DIR="intermediate_representation"
if [ ! -d "$IR_DIR" ]; then
    echo -e "${GREEN}Creating directory $IR_DIR${NC}"
    mkdir -p "$IR_DIR"
fi

# Compile the basic block constructor
echo -e "${GREEN}Compiling basic block constructor...${NC}"
g++ -o basic_block_constructor basic_block_constructor.cpp -std=c++11

# Check if compilation was successful
if [ $? -ne 0 ]; then
    echo -e "${RED}Compilation failed!${NC}"
    exit 1
fi

echo -e "${GREEN}Compilation successful!${NC}"

# Check if a specific file was provided
if [ $# -eq 1 ]; then
    # Process the specified file
    echo -e "${GREEN}Processing $1...${NC}"
    ./basic_block_constructor "$1"
else
    # If no specific file provided, look for .tac files in the IR directory
    echo -e "${GREEN}Looking for .tac files in $IR_DIR...${NC}"
    
    files=("$IR_DIR"/*.tac)
    
    if [ ${#files[@]} -eq 0 ] || [ ! -e "${files[0]}" ]; then
        echo -e "${RED}No .tac files found in $IR_DIR${NC}"
        echo "Please provide a specific file path or place .tac files in the $IR_DIR directory"
        exit 1
    fi
    
    # Process each .tac file
    for file in "${files[@]}"; do
        echo -e "${GREEN}Processing $file...${NC}"
        echo -e "${GREEN}==================================${NC}"
        ./basic_block_constructor "$file"
        echo -e "${GREEN}==================================${NC}"
        echo ""
    done
fi

echo -e "${GREEN}Done!${NC}"