#include "CodeGenerator.hpp"
#include <iostream>
#include <sstream>

CodeGenerator::CodeGenerator(const std::string& irCode, SymbolTable& symbolTable) 
    : irCode(irCode) {
    // Set the symbol table in the address allocation table
    addressTable.setSymbolTable(symbolTable);
    
    // Initialize components
    initialize();
}

void CodeGenerator::initialize() {
    // Parse IR code and populate address allocation table
    addressTable.parseIRCode(irCode);
    
    // Construct basic blocks
    BasicBlockConstructor constructor;
    basicBlocks = constructor.constructBasicBlocks(irCode);
}

void CodeGenerator::printComponentInfo() const {
    std::cout << "=============================================" << std::endl;
    std::cout << "CODE GENERATOR COMPONENT INFORMATION" << std::endl;
    std::cout << "=============================================" << std::endl;
    
    // Print address allocation table information
    std::cout << "\n[ADDRESS ALLOCATION TABLE]" << std::endl;
    addressTable.printTable();
    
    // Print register descriptor information
    std::cout << "\n[REGISTER DESCRIPTOR]" << std::endl;
    registerDesc.printDescriptor();
    
    // Print basic block information
    std::cout << "\n[BASIC BLOCKS]" << std::endl;
    BasicBlockConstructor constructor;
    constructor.printBasicBlocks(basicBlocks);
    
    std::cout << "=============================================" << std::endl;
}

std::string CodeGenerator::generateFunctionLabel(const std::string& line) {
    // Extract function name from "label function:"
    size_t labelPos = line.find("label ");
    size_t colonPos = line.find(":");
    
    if (labelPos != std::string::npos && colonPos != std::string::npos) {
        std::string funcName = line.substr(labelPos + 6, colonPos - (labelPos + 6));
        return funcName + ":\n";
    }
    
    return "";
}

std::string CodeGenerator::generateFunctionBegin(const std::string& line) {
    // Check if the line contains "func_begin"
    if (line.find("func_begin") != std::string::npos) {
        std::stringstream assembly;
        assembly << "push    rbp\n";
        assembly << "mov     rbp, rsp\n";
        return assembly.str();
    }
    
    return "";
}

std::string CodeGenerator::generateFunctionEnd(const std::string& line) {
    // Check if the line contains "func_end"
    if (line.find("func_end") != std::string::npos) {
        std::stringstream assembly;
        assembly << "pop     rbp\n";
        assembly << "ret\n";
        return assembly.str();
    }
    
    return "";
}

bool CodeGenerator::isTempOrVar(const std::string& line){
    return ((line.find("$") != std::string::npos) || (line.find("#") != std::string::npos));
}

std::vector<std::string> CodeGenerator::getReg(const std::string& line){
    // tokenizing input
    std::istringstream iss(line);
    std::vector<std::string> words;
    std::string word;
    std::vector<std::string> mapped;
    std::vector<std::string> instructions;
    std::vector<std::string> cannot_spill;

    while (iss >> word) {
        words.push_back(word);
    }

    if(words.size()==5){
        for(int i=0;i<5;i++){
            // skip operators
            if(i&1)continue;
            // check if the var/temp has a register already or not
            if(isTempOrVar(words[i])){
                if(addressTable.isEmpty(words[i])){
                    std::string reg = registerDesc.getAvailableRegister(addressTable.getType(words[i]));
                    if(reg == ""){
                        std::vector<std::string> spill = registerDesc.spillRegister(cannot_spill);
                    }
                    std::cout<<reg<<std::endl;
                }
                else{
                    auto it = addressTable.getRegisterDescriptor(words[i]);
                    auto it2 = it.begin();
                    mapped.push_back(it2->first);
                    cannot_spill.push_back(it2->first);
                }
            }
            else{
                mapped.push_back(words[i]);
            }
        }
    }
    else if(words.size()==3){
        // assignment
    }
    else{
        // others
    }
}

std::string CodeGenerator::generateArithmetic(const std::string& line){
    std::string instruction_op;
    if (line.find("+") != std::string::npos) {
        instruction_op = "add";;
    }else if (line.find("-") != std::string::npos) {
        instruction_op = "sub";
    }else if (line.find("*") != std::string::npos) {
        instruction_op = "imul";
    }else if (line.find("/") != std::string::npos){
        instruction_op = "idiv";
    }
    std::stringstream assembly;
    getReg(line);
    return "";
    
}

void CodeGenerator::processBasicBlock(const BasicBlockConstructor::BasicBlock& block) {
    std::stringstream blockCode;
    
    for (const auto& instruction : block.instructions) {
        std::string instr = instruction.text;
        std::string assembly;
        
        // Apply mapping for the specific IR instructions
        if (instr.find("label") == 0 && instr.find(":") != std::string::npos) {
            assembly = generateFunctionLabel(instr);
        } else if (instr.find("func_begin") != std::string::npos) {
            assembly = generateFunctionBegin(instr);
        } else if (instr.find("func_end") != std::string::npos) {
            assembly = generateFunctionEnd(instr);
        } 
        else if(instr.find("+") || 
                instr.find("*") ||
                instr.find("/") ||
                instr.find("+")
            ){
            //assembly = generateArithmetic(instr);
        }
        else {
            // For now, we're ignoring other instructions as requested
            continue;
        }
        
        if (!assembly.empty()) {
            blockCode << assembly;
        }
    }
    
    // Store the generated code for this block
    blockCodeMap[block.id] = blockCode.str();
}

std::string CodeGenerator::combineBlockCode() {
    std::stringstream finalCode;
    finalCode << "; Generated Assembly Code\n";
    finalCode << ".text\n";
    finalCode << ".global main\n\n";
    
    // For now, just append blocks in order
    // Later, we can use the CFG to determine the correct order
    for (const auto& entry : blockCodeMap) {
        finalCode << entry.second;
    }
    
    return finalCode.str();
}

std::string CodeGenerator::generateCode() {
    // Clear any previous generation
    blockCodeMap.clear();
    
    // Process each basic block
    for (const auto& block : basicBlocks) {
        processBasicBlock(block);
    }
    
    // Combine the blocks' code into the final assembly
    return combineBlockCode();
}