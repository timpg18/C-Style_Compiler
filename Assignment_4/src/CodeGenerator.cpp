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
    // Process data section entries first
    std::string processedIR = processDataSectionEntries(irCode);
    
    // Parse the processed IR code and populate address allocation table
    addressTable.parseIRCode(processedIR);
    addressTable.printTable();
    
    // Construct basic blocks with processed IR
    BasicBlockConstructor constructor;
    basicBlocks = constructor.constructBasicBlocks(processedIR);
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
        assembly << "leave\n";
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


std::vector<std::string> CodeGenerator::write_reg(){
    std::vector<std::string> assembly;
    std::vector<std::pair<std::string,std::string>> rem;
    for(auto &p: registerDesc.registerContent){
        //p.first is reg.name
        //p.second is vector of string of variables it has
        for(auto &s: p.second){
            //s is string of var
            auto varIt = addressTable.variables.find({s, ""});
            if(varIt != addressTable.variables.end()){
                //just write the reg val onto memory. now it'll be consistent
                std::string cd = "mov ";
                cd += "DWORD ["+ varIt->address + "]";
                cd +=", ";
                cd += p.first + "\n";
                assembly.push_back(cd);
                rem.push_back({p.first, s});
            }
        }
    }
    for(auto &p : rem){
        addressTable.removeRegisterFromDescriptor(p.second, p.first);
        //also remove register in var table
        registerDesc.freeRegister(p.first);
        //free the register completely? var is written and temp no need to preserve
    }
    
    return assembly;
}


void CodeGenerator::processBasicBlock(const BasicBlockConstructor::BasicBlock& block) {
    std::stringstream blockCode;
    int paramNumber=1;
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
        }else if(instr.find("goto") != std::string::npos){

            std::istringstream iss(instr);
            std::vector<std::string> words;
            std::string word;
            while (iss >> word) {
                words.push_back(word);
                std::cout <<word <<"\n";
            }
            //first write/store all variables to their address
            //from registers
            assembly = write_reg();
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
        }else if (instr.find("return") != std::string::npos) {
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
        }else if (instr.find("func_end") != std::string::npos) {
            assembly.push_back(generateFunctionEnd(instr));
        }else if(instr.find("param") != std::string::npos){
            std::istringstream iss(instr);
            std::vector<std::string> words;
            std::string word;
            while (iss >> word) {
                words.push_back(word);
                std::cout <<word <<"\n";
            }
            std::string reg = "";
            std::string type = "";
            std::string assm1 = "";
            auto it = dataSectionMap.find(words[1]);
            if(it != dataSectionMap.end()){
                type = it->second.first;
            }
            else{
                if(isTempOrVar(words[1]))type = addressTable.getType(words[1]);
                else{
                    //currently considering int operand;
                    type = "INT";
                }
            }
            switch (paramNumber) {
                case 1: reg = registerDesc.getRegisterForType("rdi",type);
                        break;
                case 2: reg = registerDesc.getRegisterForType("rsi",type);
                        break;
                case 3: reg = registerDesc.getRegisterForType("rdx",type);
                        break;
                case 4: reg = registerDesc.getRegisterForType("rcx",type);
                        break;
                case 5: reg = registerDesc.getRegisterForType("r8",type);
                        break;
                case 6: reg = registerDesc.getRegisterForType("r9",type);
                        break;
            }
            if(assm1 != "")assembly.push_back(assm1);
            paramNumber++;
            std::string assm2 = "mov " + reg + ", " + words[1] +"\n";
            assembly.push_back(assm2);

        }else if(instr.find("call") != std::string::npos){
            paramNumber = 1;
            std::istringstream iss(instr);
            std::vector<std::string> words;
            std::string word;
            while (iss >> word) {
                words.push_back(word);
                std::cout <<word <<"\n";
            }

            std::string funcName = words[3];
            std::string assm = "";
            if(funcName == "printf"){
                assm = "mov eax, 0\n";
            }
            funcName.pop_back();
            assm += "call " + funcName + "\n";
            assembly.push_back(assm);

        }else if(found == true){
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
        }else if(instr.find("=") != std::string::npos){
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
            
        }else{
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
    
    // Add data section
    std::string dataSection = generateDataSection();
    if (!dataSection.empty()) {
        finalCode << dataSection << "\n";
    }
    
    finalCode << "section .text\n";
    finalCode << "global _start\n";
    finalCode << "extern printf\n\n";  // Add external declaration for printf
    finalCode << "extern exit\n\n";  // Add external declaration for exit
    finalCode << "_start:\n";
    finalCode << "and rsp, 0xfffffffffffffff0\n";
    finalCode << "call main\n";
    finalCode << "mov rdi, rax\n";
    finalCode << "call exit\n\n";

    
    // Append blocks in order
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

std::string CodeGenerator::generateDataSection() {
    std::stringstream dataSection;
    
    if (dataSectionMap.empty()) {
        return ""; // No data section needed
    }
    
    dataSection << "section .data\n";
    
    for (const auto& [alias, data] : dataSectionMap) {
        const std::string& type = data.first;
        const std::string& value = data.second;
        
        if (type == "CHAR*") {
            // Parse string with escape sequences
            std::vector<std::string> parts = parseStringWithEscapeSequences(value);
            
            dataSection << alias << " db ";
            
            // Join parts with commas
            for (size_t i = 0; i < parts.size(); i++) {
                dataSection << parts[i];
                if (i < parts.size() - 1) {
                    dataSection << ", ";
                }
            }
            
            // Add null terminator if not already there
            if (parts.empty() || parts.back() != "0") {
                dataSection << ", 0";
            }
            
            dataSection << "\n";
        }
        // Add other data types as needed
    }
    
    return dataSection.str();
}

std::string CodeGenerator::processDataSectionEntries(const std::string& irCode) {
    std::string processedCode = irCode;
    std::regex stringPattern("\"([^\"]*)\"");
    std::smatch match;
    std::string::const_iterator searchStart(irCode.cbegin());
    int literalCount = 0;
    
    while (std::regex_search(searchStart, irCode.cend(), match, stringPattern)) {
        std::string stringLiteral = match[0]; // The complete string with quotes
        
        // Check if we've already created an entry for this string literal
        bool found = false;
        std::string alias;
        
        for (const auto& entry : dataSectionMap) {
            if (entry.second.second == stringLiteral) {
                found = true;
                alias = entry.first;
                break;
            }
        }
        
        if (!found) {
            // Create new alias for this string
            alias = "str" + std::to_string(++literalCount);
            dataSectionMap[alias] = {"CHAR*", stringLiteral};
            
        }
        
        // Move search position
        searchStart = match.suffix().first;
        
        // Replace this occurrence in the code
        size_t pos = processedCode.find(stringLiteral);
        if (pos != std::string::npos) {
            processedCode.replace(pos, stringLiteral.length(), alias);
        }
    }
    
    return processedCode;
}

std::vector<std::string> CodeGenerator::parseStringWithEscapeSequences(const std::string& str) {
    std::vector<std::string> parts;
    std::string currentPart;
    
    // Remove surrounding quotes
    std::string content = str.substr(1, str.length() - 2);
    
    for (size_t i = 0; i < content.length(); i++) {
        if (content[i] == '\\' && i + 1 < content.length()) {
            // If we have a non-empty current part, add it
            if (!currentPart.empty()) {
                parts.push_back("\"" + currentPart + "\"");
                currentPart.clear();
            }
            
            // Handle the escape sequence
            char escapeChar = content[i + 1];
            int asciiValue;
            
            switch (escapeChar) {
                case 'n': asciiValue = 10; break;  // Newline
                case 't': asciiValue = 9; break;   // Tab
                case 'r': asciiValue = 13; break;  // Carriage return
                case '0': asciiValue = 0; break;   // Null character
                case '\\': asciiValue = 92; break; // Backslash
                case '\"': asciiValue = 34; break; // Double quote
                case '\'': asciiValue = 39; break; // Single quote
                case 'a': asciiValue = 7; break;   // Bell
                case 'b': asciiValue = 8; break;   // Backspace
                case 'f': asciiValue = 12; break;  // Form feed
                case 'v': asciiValue = 11; break;  // Vertical tab
                default: asciiValue = escapeChar;  // Just use the character
            }
            
            parts.push_back(std::to_string(asciiValue));
            i++; // Skip the escape sequence
        } else {
            currentPart += content[i];
        }
    }
    
    // Add any remaining part
    if (!currentPart.empty()) {
        parts.push_back("\"" + currentPart + "\"");
    }
    
    return parts;
}