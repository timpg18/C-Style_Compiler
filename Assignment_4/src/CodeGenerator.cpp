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
    addressTable.printTable();
    
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
    std::istringstream iss(line);
    std::vector<std::string> words;
    std::string word;
    while (iss >> word) {
        words.push_back(word);
        std::cout <<word <<"\n";
    }

    int size = addressTable.getFunctionStackSize(words[1],irCode);
    auto params = addressTable.getFunctionParameters(words[1]);
    std::string assm = "sub rsp, " + std::to_string(size) + "\n";
    if (line.find("func_begin") != std::string::npos) {
        std::stringstream assembly;
        assembly << "push rbp\n";
        assembly << "mov rbp, rsp\n";
        assembly << assm;
        if(params.size() > 0){
            for(auto param : params){
                std::string reg ="";
                switch (param.second) {
                    case 1: reg = registerDesc.getRegisterForType("rdi",addressTable.getType(param.first));
                            break;
                    case 2: reg = registerDesc.getRegisterForType("rsi",addressTable.getType(param.first));
                            break;
                    case 3: reg = registerDesc.getRegisterForType("rdx",addressTable.getType(param.first));
                            break;
                    case 4: reg = registerDesc.getRegisterForType("rcx",addressTable.getType(param.first));
                            break;
                    case 5: reg = registerDesc.getRegisterForType("r8",addressTable.getType(param.first));
                            break;
                    case 6: reg = registerDesc.getRegisterForType("r9",addressTable.getType(param.first));
                            break;
                }
                std::string to_add = "mov " + getAsmSizeDirective(param.first) + " ["+addressTable.getVariableAddress(param.first)+"]," + reg + "\n";
                assembly << to_add;
            }
           
        }

        return assembly.str();
    }
    
    return "";
}

std::string CodeGenerator::generateFunctionEnd(const std::string& line) {
    // Check if the line contains "func_end"

    if (line.find("func_end") != std::string::npos) {
        std::stringstream assembly;
        assembly << "pop rbp\n";
        assembly << "ret\n";
        return assembly.str();
    }
    
    return "";
}

bool CodeGenerator::isTempOrVar(const std::string& line){
    return ((line.find("$") != std::string::npos) || (line.find("#") != std::string::npos));
}

bool CodeGenerator::isTemp(const std::string& line){
    return ((line.find("$") != std::string::npos));
}

bool CodeGenerator::isVar(const std::string& line){
    return ((line.find("#") != std::string::npos));
}

std::vector<std::string> CodeGenerator::getReg(const std::string& line, std::vector<std::string>&assembly, std::map<int,int> ind){
    // tokenizing input
    std::istringstream iss(line);
    std::vector<std::string> words;
    std::string word;
    std::vector<std::string> mapped;
    std::vector<std::string> instructions;
    std::vector<std::string> cannot_spill;
    //i will extract registers for mapped ind
    while (iss >> word) {
        words.push_back(word);
        std::cout <<word <<"\n";
        std::cout <<"each" <<"\n";
    }
    for(auto &in: ind){
        int i = in.first;
        if(isTempOrVar(words[i])){
            if(addressTable.isEmpty(words[i])){
                std::string type = addressTable.getType(words[i]);
                std::cout<<"HELLO WORLD "<<type<<"\n";
                std::string reg = registerDesc.getAvailableRegister(type);
                // spill in case all registers are in use
                if(reg == ""){
                    std::vector<std::string> spill = registerDesc.spillRegister(cannot_spill);
                }
                std::cout<<reg<<std::endl;
                mapped.push_back(reg);
                // update address allocation table for future use
                addressTable.addRegisterToDescriptor(words[i],reg,"0");
                if(isVar(words[i])){
                    std::cout<<words[i]<<" allocated issue\n";
                    std::string assm = "";
                    assm = "mov " + reg + ", " + getAsmSizeDirective(addressTable.getType(words[i])) + " ["+ addressTable.getVariableAddress(words[i]) +"]\n";
                    assembly.push_back(assm);
                }
                // update the register descriptor as well
                std::cout<<"reg allocated "<<registerDesc.allocateRegister(reg,words[i])<<"\n";


            }
            else{
                std::cout <<"bruh \n";
                // std::cout <<words[i] <<"\n";
                // addressTable.printTable();
                auto it = addressTable.getRegisterDescriptor(words[i]);
                auto it2 = it.begin();
                // considering that only 1 register per address
                // removed because this thing is not possible
                mapped.push_back(it2->first);
                cannot_spill.push_back(it2->first);
                std::cout <<it.size() <<"\n";
                
            }
        }
        else{
            mapped.push_back(words[i]);
        }
    }
    // if(words.size()==5){
    //     for(int i=0;i<5;i++){
    //         // skip operators
    //         if(i&1)continue;
    //         // check if the var/temp has a register already or not
            
    //     }
    // }
    // else if(words.size()==3){
    //     // assignment
    // }
    // else{
    //     // others
    // }
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
    std::map<int,int> req;
    req[0] = 1;
    req[2] = 1;
    req[4] = 1;
    std::vector<std::string> registers  = getReg(line,assembly, req);
    //we also push the extra instructions in getReg only
    std::cout <<instruction_op <<"\n";
    std::string code;
    if(registers.size() == 3){
        //ie type a = b op c
            //3 operand instruction 
            code = "mov " + registers[0] + ", " + registers[1] +"\n";
            code += instruction_op + " " + registers[0] + ", " + registers[2] + "\n";
    }
    assembly.push_back(code);
    return assembly;
    
}

void keyword_init( std::map<std::string,std::string> &keywords){
    
    for (const auto& op : {"+", "*", "/", "-"}) {
        keywords[op] = "arithmetic";
    }

    for (const auto& op : {"<", ">", "<=", ">=", "==", "!="}) {
        keywords[op] = "cmp";
    }
}

std::vector<std::string> CodeGenerator::generateCMP(const std::string& line, std::string op){
    std::istringstream iss(line);
    std::vector<std::string> words;
    std::string word;
    while (iss >> word) {
        words.push_back(word);
        std::cout <<word <<"\n";
    }
    std::vector<std::string> assembly;
    //of form $x = a op b
    //op is !=, == etc
    std::map<int,int> req;
    req[2] = 1;
    req[4] = 1;
    //handle 0 $x alag se
    std::vector<std::string> reg = getReg(line ,assembly, req);
    std::string cd = "cmp " + reg[0] + ", " + reg[1] + "\n";
    assembly.push_back(cd);
    addressTable.set_relop(words[0],op);
    return assembly;
}

std::map<std::string, std::string> jump_init(){
    std::map<std::string, std::string> rel_ops_to_jumps = {
        {"==", "je"},    // Jump if equal
        {"!=", "jne"},   // Jump if not equal
        {">",  "jg"},    // Jump if greater (signed)
        {">=", "jge"},   // Jump if greater or equal (signed)
        {"<",  "jl"},    // Jump if less (signed)
        {"<=", "jle"}    // Jump if less or equal (signed)
    };
    return rel_ops_to_jumps;
}

void CodeGenerator::processBasicBlock(const BasicBlockConstructor::BasicBlock& block) {
    std::stringstream blockCode;
    
    for (const auto& instruction : block.instructions) {
        std::string instr = instruction.text;
        std::cout<<instr<<std::endl;
        std::vector<std::string> assembly;
        if(instr.empty() == true)continue;
        std::map<std::string,std::string> keywords;
        keyword_init(keywords);
        std::string type;
        std::string op;
        bool found = std::any_of(keywords.begin(),keywords.end(),[&](auto &p){
            if(instr.find(p.first) != std::string::npos){
                type = p.second;
                op = p.first;
                return true;
            }
        });
        // Apply mapping for the specific IR instructions
        if (instr.find("label") == 0 && instr.find(":") != std::string::npos) {
            assembly.push_back(generateFunctionLabel(instr));
        }
        else if(instr.find("goto") != std::string::npos){
            std::istringstream iss(instr);
            std::vector<std::string> words;
            std::string word;
            while (iss >> word) {
                words.push_back(word);
                std::cout <<word <<"\n";
            }
            if(instr.find("if") != std::string::npos){
                //get the relational op
                //depending on relational op, jne/jeq etc to label @L1
                std::string op = addressTable.get_relop(words[1]);
                std::string jmp;
                std::map<std::string,std::string> op_to_j;
                op_to_j = jump_init();
                jmp = op_to_j[op];
                jmp += " " + words[3];
                jmp += "\n";
                assembly.push_back(jmp);
            }
            else{
                std::string jmp = "jmp ";
                jmp += words[1];
                jmp += "\n";
                assembly.push_back(jmp);
            }
        }
        else if (instr.find("return") != std::string::npos) {
            // return value 
            std::istringstream iss(instr);
            std::vector<std::string> words;
            std::string word;
            while (iss >> word) {
                words.push_back(word);
                std::cout <<word <<"\n";
            }

            std::string return_value = words[1];
            if(isTemp(return_value)){
                auto it = addressTable.getRegisterDescriptor(words[1]);
                auto it2 = it.begin();
                std::string reg = it2->first;
                return_value = reg;
            }
            else if(isVar(return_value)){
                if(!addressTable.isEmpty(words[1])){
                    auto it = addressTable.getRegisterDescriptor(words[1]);
                    auto it2 = it.begin();
                    std::string reg = it2->first;
                    return_value = reg;
                }
                else{
                    return_value = getAsmSizeDirective(addressTable.getType(words[1])) + " ["+ addressTable.getVariableAddress(words[1]) +"]";
                }
            }

            std::string assm = "mov eax, " + return_value + "\n";
            assembly.push_back(assm);

        }else if (instr.find("func_begin") != std::string::npos) {
            assembly.push_back(generateFunctionBegin(instr));
        } else if (instr.find("func_end") != std::string::npos) {
            assembly.push_back(generateFunctionEnd(instr));
        } 
        else if(found == true){
            if(type == "arithmetic"){
                std::cout <<instr <<"\n";
                std::cout <<"the assembly code \n";
                assembly = generateArithmetic(instr);
            }
            else if(type == "cmp"){
                std::cout <<instr <<"\n";
                std::cout <<"cmppp \n";
                assembly = generateCMP(instr, op);
            }
        }
        else if(instr.find("=") != std::string::npos){
            //assignment
            std::istringstream iss(instr);
            std::vector<std::string> words;
            std::string word;
            while (iss >> word) {
                words.push_back(word);
                std::cout <<word <<"\n";
            }
            // assignment for variable with constant value
            if(!isTempOrVar(words[2])){
                std::string assm ="";
                // if already not in the register
                if(addressTable.isEmpty(words[0])){
                    assm = "mov " + getAsmSizeDirective(addressTable.getType(words[0])) + " ["+ addressTable.getVariableAddress(words[0]) +"], " + words[2] + "\n";
                }
                // if already in the register
                else{
                    auto it = addressTable.getRegisterDescriptor(words[0]);
                    auto it2 = it.begin();
                    std::string reg = it2->first;
                    assm = "mov " + reg + ", " + words[2] + "\n";
                }
                
                assembly.push_back(assm);
            }else if(isTemp(words[2])){
                // second one is temporary in this case
                std::string assm ="";
                auto it1 = addressTable.getRegisterDescriptor(words[2]);
                auto it3 = it1.begin();
                std::string regTemp = it3->first;
                // considering the fact that temporary will not be assigned to temporary 
                if(addressTable.isEmpty(words[0])){
                    assm = "mov " + getAsmSizeDirective(addressTable.getType(words[0])) + " ["+ addressTable.getVariableAddress(words[0]) +"], " + regTemp + "\n";
                }
                // if already in the register
                else{
                    auto it = addressTable.getRegisterDescriptor(words[0]);
                    auto it2 = it.begin();
                    std::string reg = it2->first;
                    assm = "mov " + reg + ", " + regTemp + "\n";
                }
                assembly.push_back(assm);
            }else if(isVar(words[2])){
                // first lets get the reg or address for the 2nd operand
                std::string reg2 = "";
                if(addressTable.isEmpty(words[2])){
                    reg2 = getAsmSizeDirective(addressTable.getType(words[2])) + " ["+ addressTable.getVariableAddress(words[2]) +"]";
                }
                // if already in the register
                else{
                    auto it = addressTable.getRegisterDescriptor(words[2]);
                    auto it2 = it.begin();
                    reg2 = it2->first;
                }

                // now for the 1st operand
                std::string reg1 = "";
                // first operand may be variable
                if(isVar(words[0])){
                    if(addressTable.isEmpty(words[0])){
                        reg1 = getAsmSizeDirective(addressTable.getType(words[0])) + " ["+ addressTable.getVariableAddress(words[0]) +"]";
                    }
                    // if already in the register
                    else{
                        auto it = addressTable.getRegisterDescriptor(words[0]);
                        auto it2 = it.begin();
                        reg1 = it2->first;
                    }
                }
                // first operand may be temporary
                else{
                    // temporary to be assigned
                    if(addressTable.isEmpty(words[0])){
                        // to handle
                        std::string type = addressTable.getType(words[0]);
                        std::string reg = registerDesc.getAvailableRegister(type);
                        // spill in case all registers are in use
                        if(reg == ""){
                            // to handle
                        }
                        // update address allocation table for future use
                        addressTable.addRegisterToDescriptor(words[0],reg,"0");
                        // update the register descriptor as well
                        std::cout<<"reg allocated "<<registerDesc.allocateRegister(reg,words[0])<<" "<<reg<<"\n";
                        reg1 = reg;
                    }
                    // already assigned temporary
                    else{
                        auto it = addressTable.getRegisterDescriptor(words[0]);
                        auto it2 = it.begin();
                        reg1 = it2->first;
                    }
                }

                std::string assm = "mov " + reg1 + ", " +reg2 + "\n";
                assembly.push_back(assm);
            }
            
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
    finalCode << "section .text\n";
    finalCode << "global main\n\n";
    
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

// type to asm directive
std::string CodeGenerator::getAsmSizeDirective(const std::string& type) const {

    // First check if this is a pointer type
    if (type.find('*') != std::string::npos) {
        // All pointers are QWORD (8 bytes) in 64-bit architecture
        return "QWORD";
    }
    
    // Check if the type exists in our mapping
    auto it = typeToAsmSize.find(type);
    if (it != typeToAsmSize.end()) {
        return it->second;
    }
    // Default to QWORD if unknown type (safer assumption in 64-bit)
    return "DWORD";  // Most common fallback
}