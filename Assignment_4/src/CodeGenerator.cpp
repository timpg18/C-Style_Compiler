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

std::vector<std::string> CodeGenerator::getReg(const std::string& line, std::vector<std::string>&assembly){
    // tokenizing input
    std::istringstream iss(line);
    std::vector<std::string> words;
    std::string word;
    std::vector<std::string> mapped;
    std::vector<std::string> instructions;
    std::vector<std::string> cannot_spill;

    while (iss >> word) {
        words.push_back(word);
        std::cout <<word <<"\n";
        std::cout <<"each" <<"\n";
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
                    std::cout <<"bruh \n";
                    // std::cout <<words[i] <<"\n";
                    // addressTable.printTable();
                    auto it = addressTable.getRegisterDescriptor(words[i]);
                    auto it2 = it.begin();
                    if(it.size() > 0){
                    //return the first register descripter it has to use
                    //can be optimised though
                    mapped.push_back(it2->first);
                    cannot_spill.push_back(it2->first);
                    }
                    else{
                        //no register descriptor, we must add one
                        //btw we must add mov instruction for this
                        //we take int for now, otherwise add temp ka type
                        std::string reg_name = registerDesc.getAvailableRegister("INT");
                        if(reg_name != ""){
                            if(registerDesc.allocateRegister(reg_name, words[i]) == true){
                                mapped.push_back(reg_name);
                                std::string mov;
                                mov += "mov "+ reg_name + ", " + words[i] +"\n";
                               
                                assembly.push_back(mov);
                                std::cout <<reg_name <<"\n";
                            }
                            else{
                                std::cout <<"error! could not allocate register \n";
                            }
                        }
                        else{
                            //spill scenario
                        }
                    }
    
                    std::cout <<it.size() <<"\n";
                    
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
    return mapped;
    
}

std::vector<std::string> CodeGenerator::generateArithmetic(const std::string& line){
    std::string instruction_op;
    std::vector<std::string> assembly;
    if (line.find("+") != std::string::npos) {
        instruction_op = "add";
    }else if (line.find("-") != std::string::npos) {
        instruction_op = "sub";
    }else if (line.find("*") != std::string::npos) {
        instruction_op = "imul";
    }else if (line.find("/") != std::string::npos){
        instruction_op = "idiv";
    }
    // std::stringstream assembly;
    std::vector<std::string> registers  = getReg(line,assembly);
    //we also push the extra instructions in getReg only
    std::cout <<instruction_op <<"\n";
    std::string code;
    if(registers.size() == 3){
        //ie type a = b op c
        if(instruction_op[0] == 'i'){
            //3 operand instruction 
            code =instruction_op + " " + registers[0] +", " + registers[1] + ", " + registers[2] + "\n";
        }
        else{
            //handle + , - etc
        }
    }
    assembly.push_back(code);
    return assembly;
    
}

void CodeGenerator::processBasicBlock(const BasicBlockConstructor::BasicBlock& block) {
    std::stringstream blockCode;
    
    for (const auto& instruction : block.instructions) {
        std::string instr = instruction.text;
        std::vector<std::string> assembly;
        if(instr.empty() == true)continue;
        std::vector<std::string> keywords;
        keywords.push_back("+");
        keywords.push_back("*");
        keywords.push_back("/");
        keywords.push_back("+");
        bool found = std::any_of(keywords.begin(),keywords.end(),[&](const std::string &keyword){
            return instr.find(keyword) != std::string::npos;
        });
        // Apply mapping for the specific IR instructions
        if (instr.find("label") == 0 && instr.find(":") != std::string::npos) {
            assembly.push_back(generateFunctionLabel(instr));
        } else if (instr.find("func_begin") != std::string::npos) {
            assembly.push_back(generateFunctionBegin(instr));
        } else if (instr.find("func_end") != std::string::npos) {
            assembly.push_back(generateFunctionBegin(instr));
        } 
        else if(found == true){
            std::cout <<instr <<"\n";
            std::cout <<"the assembly code \n";
            assembly = generateArithmetic(instr);
        }
        else {
            // For now, we're ignoring other instructions as requested
            continue;
        }
        
        if (assembly.size() > 0) {
            for(auto &str: assembly){
                blockCode << str;
            }
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