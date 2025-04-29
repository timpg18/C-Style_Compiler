#include "CodeGenerator.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

std::string get64BitRegister(const std::string& reg) {
    static std::unordered_map<std::string, std::vector<std::string>> relatedRegisters = {
        {"rax", {"rax", "eax", "ax", "ah", "al"}},
        {"rcx", {"rcx", "ecx", "cx", "ch", "cl"}},
        {"rdx", {"rdx", "edx", "dx", "dh", "dl"}},
        {"rsi", {"rsi", "esi", "si", "sil"}},
        {"rdi", {"rdi", "edi", "di", "dil"}},
        {"r8",  {"r8", "r8d", "r8w", "r8b"}},
        {"r9",  {"r9", "r9d", "r9w", "r9b"}},
        {"r10", {"r10", "r10d", "r10w", "r10b"}},
        {"r11", {"r11", "r11d", "r11w", "r11b"}}
    };

    for (const auto& [base64, variants] : relatedRegisters) {
        for (const auto& variant : variants) {
            if (reg == variant) {
                return base64; // Return the 64-bit version
            }
        }
    }

    // If not found, return empty or maybe the input itself?
    return "";
}

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
    std::string processedIR1 = processDataSectionEntries(irCode);
    std::string processedIR2 = replaceCharConstants(processedIR1);
    std::string processedIR = processFloatConstants(processedIR2);
    
    // Parse the processed IR code and populate address allocation table
    addressTable.parseIRCode(irCode);
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
                std::string paramType = addressTable.getType(param.first);
                switch (param.second) {
                    case 1: reg = registerDesc.getRegisterForType("rdi",paramType);
                            break;
                    case 2: reg = registerDesc.getRegisterForType("rsi",paramType);
                            break;
                    case 3: reg = registerDesc.getRegisterForType("rdx",paramType);
                            break;
                    case 4: reg = registerDesc.getRegisterForType("rcx",paramType);
                            break;
                    case 5: reg = registerDesc.getRegisterForType("r8",paramType);
                            break;
                    case 6: reg = registerDesc.getRegisterForType("r9",paramType);
                            break;
                }
                std::string to_add = "mov " + getAsmSizeDirective(paramType) + " ["+addressTable.getVariableAddress(param.first)+"], " + reg + "\n";
                if(paramType == "FLOAT"){
                    to_add = "movss " + getAsmSizeDirective(paramType) + " ["+addressTable.getVariableAddress(param.first)+"], xmm" +std::to_string(param.second-1) + "\n";
                }
                if(paramType == "DOUBLE"){
                    to_add = "movsd " + getAsmSizeDirective(paramType) + " ["+addressTable.getVariableAddress(param.first)+"], xmm" +std::to_string(param.second-1) + "\n";
                }
                assembly << to_add;
            }
           
        }

        return assembly.str();
    }
    
    return "";
}

std::string CodeGenerator::generateFunctionEnd(const std::string& line) {
    // Check if the line contains "func_end"
    std::vector<std::string> wr_reg(0);
    if (line.find("func_end") != std::string::npos) {
        wr_reg = write_reg();
        std::string written_reg ="";
        for(auto& i : wr_reg){
            written_reg += i;
        }
        std::stringstream assembly;
        std::string exit_label = "@E" + std::to_string(exit_count) + ":" + "\n";
        assembly << exit_label;
        assembly << written_reg;
        assembly << "leave\n";
        assembly << "ret\n";
        exit_count++;
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
     
    }
    for(auto in = ind.rbegin(); in != ind.rend(); ++in){
        int i = in->first;
        if(isTempOrVar(words[i])){
            if(addressTable.isEmpty(words[i])){
                std::string type = addressTable.getType(words[i]);
              
                std::string reg = registerDesc.getAvailableRegister(type);
                // spill in case all registers are in use
                if(reg == ""){
                    std::vector<std::string> spill = registerDesc.spillRegister(cannot_spill,type);
                 
                    // first is the free reg, second is the spilled reg, third is the variable or temporary it was holding
                    reg = spill[0];
                    cannot_spill.push_back(reg);
                    std::string var_temp = spill[2];
                    std::string reg_spilled = spill[1];
                    std::string spill_assm = "";
                    if(isVar(var_temp)){
                        std::string mov_op = "mov ";
                        if(type == "FLOAT") mov_op = "movss ";
                        if(type == "DOUBLE") mov_op = "movsd ";
                        spill_assm = mov_op + getAsmSizeDirective(type) + " [" + addressTable.getVariableAddress(var_temp) + "], " + reg_spilled + "\n";
                    }
                    else{
                        std::string mov_op = "mov ";
                        if(type == "FLOAT") mov_op = "movss ";
                        if(type == "DOUBLE") mov_op = "movsd ";
                        spill_assm = mov_op + getAsmSizeDirective(type) + " [" + addressTable.getTemporaryAddress(var_temp) + "], " + reg_spilled + "\n";
                    }
                    assembly.push_back(spill_assm);
                    addressTable.removeRegisterFromDescriptor(var_temp,reg_spilled);
                }
                
                mapped.push_back(reg);
                // update address allocation table for future use
                addressTable.addRegisterToDescriptor(words[i],reg,"0");
                
                std::string assm = "";
                if(isVar(words[i])){
                    assm = "mov " + reg + ", " + getAsmSizeDirective(addressTable.getType(words[i])) + " ["+ addressTable.getVariableAddress(words[i]) +"]\n";
                    if(type=="FLOAT"){
                        assm = "movss " + reg + "," + " ["+ addressTable.getVariableAddress(words[i]) +"]\n";
                    }
                    if(type=="DOUBLE"){
                        assm = "movsd " + reg + "," + " ["+ addressTable.getVariableAddress(words[i]) +"]\n";
                    }
                }
                else{
                    assm = "mov " + reg + ", " + getAsmSizeDirective(addressTable.getType(words[i])) + " ["+ addressTable.getTemporaryAddress(words[i]) +"]\n";
                    if(type=="FLOAT"){
                        assm = "movss " + reg + "," + " ["+ addressTable.getTemporaryAddress(words[i]) +"]\n";
                    }
                    if(type=="DOUBLE"){
                        assm = "movsd " + reg + "," + " ["+ addressTable.getTemporaryAddress(words[i]) +"]\n";
                    }
                }
                
                assembly.push_back(assm);
                // update the register descriptor as well
                bool g = registerDesc.allocateRegister(reg,words[i]);
                


            }
            else{
              
                // std::cout <<words[i] <<"\n";
                // addressTable.printTable();
                auto it = addressTable.getRegisterDescriptor(words[i]);
                auto it2 = it.begin();
                // considering that only 1 register per address
                // removed because this thing is not possible
                mapped.push_back(it2->first);
                cannot_spill.push_back(it2->first);
               

                // now doing some optimization - if the temporary is used on the right side then it wont be used further so lets delete it form the address table adn register table.

                
            }
        }
        else{
            mapped.push_back(words[i]);
        }
    }
    std::reverse(mapped.begin(),mapped.end());
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
    
    std::string code;
    if(registers.size() == 3){
        //ie type a = b op c
            //3 operand instruction 
            std::istringstream iss(line);
            std::vector<std::string> words;
            std::string word;
            while (iss >> word) {
                words.push_back(word); 
                
            }
            std::string type = "";
            if(isTempOrVar(words[0]))type = addressTable.getType(words[0]);
            std::string mov_ins = "mov ";
            std::string reg1 = registers[0];
            std::string reg2 = registers[1];
            std::string reg3 = registers[2];
            if(type == "FLOAT"){
                mov_ins = "movss ";
                if(instruction_op == "add"){
                    instruction_op = "addss";
                }
                else if(instruction_op == "sub"){
                    instruction_op = "subss ";
                }
                else if( instruction_op == "imul"){
                    instruction_op = "mulss";
                }
                else if(instruction_op == "idiv"){
                    instruction_op = "divss";
                }
            }
            if(type == "DOUBLE"){
                mov_ins = "movsd ";
                if(instruction_op == "add"){
                    instruction_op = "addsd ";
                }
                else if(instruction_op == "sub"){
                    instruction_op = "subsd ";
                }
                else if( instruction_op == "imul"){
                    instruction_op = "mulsd ";
                }
                else if(instruction_op == "idiv"){
                    instruction_op = "divsd ";
                }
            }
            auto it1 = dataSectionMap.find(reg2);
            auto it2 = dataSectionMap.find(reg3);
            if(it1!= dataSectionMap.end()){
                if(it1->second.first == "FLOAT"){
                    reg2 = "[" + reg2 + "]";
                }
            }
            if(it2!= dataSectionMap.end()) {
                if(it2->second.first == "FLOAT"){
                    reg3 = "[" + reg3 + "]";
                }
            }           
            

            code = mov_ins + reg1 + ", " + reg2 +"\n";
            std::string typ = registerDesc.get_type(reg1);
            
            code += instruction_op + " " + reg1 + ", " + reg3 + "\n";
    }
    assembly.push_back(code);
    return assembly;
    
}

std::vector<std::string> CodeGenerator::generateBitwise(const std::string& line){
    std::string instruction_op;
    std::vector<std::string> assembly;
    if (line.find("&") != std::string::npos) {
        instruction_op = "and";
    }else if (line.find("|") != std::string::npos) {
        instruction_op = "or";
    }else if (line.find("^") != std::string::npos) {
        instruction_op = "xor";
    }
    
    std::map<int,int> req;

    req[0] = 1;
    req[2] = 1;
    req[4] = 1;
    std::vector<std::string> registers  = getReg(line,assembly, req);
    //we also push the extra instructions in getReg only
    
    std::string code;
    if(registers.size() == 3){
        //ie type a = b op c
        //3 operand instruction 
        std::string mov_ins = "mov ";
        std::string reg1 = registers[0];
        std::string reg2 = registers[1];
        std::string reg3 = registers[2];      

        code = mov_ins + reg1 + ", " + reg2 +"\n";
        code += instruction_op + " " + reg1 + ", " + reg3 + "\n";
        assembly.push_back(code);
    }
    
    
    return assembly;
    
}

std::vector<std::string> CodeGenerator::generateShifts(const std::string& line){
    std::string instruction_op;
    std::vector<std::string> assembly;
    if (line.find("<<") != std::string::npos) {
        instruction_op = "shl";
    }else if (line.find(">>") != std::string::npos) {
        instruction_op = "sar";
    }
    
    std::map<int,int> req;

    req[0] = 1;
    req[2] = 1;
    req[4] = 1;
    std::vector<std::string> registers  = getReg(line,assembly, req);
    //we also push the extra instructions in getReg only
    
    std::string code;
    if(registers.size() == 3){
        //ie type a = b op c
        //3 operand instruction 
        std::string mov_ins = "mov ";
        std::string reg1 = registers[0];
        std::string reg2 = registers[1];
        std::string reg3 = registers[2];      

        code = mov_ins + reg1 + ", " + reg2 +"\n";
        code += instruction_op + " " + reg1 + ", " + reg3 + "\n";
        assembly.push_back(code);
    }
    
    
    return assembly;
    
}

void keyword_init( std::map<std::string,std::string> &keywords){
    
    for (const auto& op : {"+", "*", "/", "-"}) {
        keywords[op] = "arithmetic";
    }

    for (const auto& op : {"<", ">", "<=", ">=", "==", "!="}) {
        keywords[op] = "cmp";
    }

    for (const auto& op : {"|", "^", "!", "&"}) {
        keywords[op] = "bitwise";
    }

    for (const auto& op : {"<<", ">>"}) {
        keywords[op] = "shifts";
    }
}



std::vector<std::string> CodeGenerator::generateCMP(const std::string& line, std::string op){
    std::istringstream iss(line);
    std::vector<std::string> words;
    std::string word; 
    
    while (iss >> word) {
        words.push_back(word);
        
    }
    std::vector<std::string> assembly;
    //of form $x = a op b
    //op is !=, == etc
    op = words[3];
    std::map<int,int> req;
    req[2] = 1;
    req[4] = 1;
    //handle 0 $x alag se
    std::vector<std::string> reg = getReg(line ,assembly, req);
    bool isfloat = 0;
    auto it = dataSectionMap.find(reg[0]);
    std::string may_be_used = "";
    if(it!=dataSectionMap.end()){
        if(it->second.first == "FLOAT"){
            may_be_used = "movss xmm7, [" + reg[0] + "]\n";
            reg[0] = "xmm7";
            isfloat =true;
        }
    }
    auto it2 = dataSectionMap.find(reg[1]);
    if(it!=dataSectionMap.end()){
        if(it->second.first == "FLOAT"){
            reg[1] = "[" + reg[1] + "]";
            isfloat =true;
        }
    }
    std::string cmpop = "cmp ";
    if(isfloat)cmpop = "ucomiss ";
    std::string cd = may_be_used;
    cd += cmpop + reg[0] + ", " + reg[1] + "\n";
    // std::cout <<"rly?" <<"\n";
    // std::cout <<cd <<"\n" <<"broski \n";
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
        // need to add for float
        //p.first is reg.name
        //p.second is vector of string of variables it has
        for(auto &s: p.second){
            //s is string of var
            auto varIt = addressTable.variables.find({s, ""});
            if(varIt != addressTable.variables.end()){
                //just write the reg val onto memory. now it'll be consistent
                std::string type = varIt->type;
                std::string cd = "mov ";
                if(type == "FLOAT"){
                    cd = "movss ";
                }
                if(type == "DOUBLE"){
                    cd = "movsd ";
                }
                cd += getAsmSizeDirective(type) +" ["+ varIt->address + "]";
                cd +=", ";
                std::string conv = registerDesc.convertRegisterForType(p.first, varIt->type);
                cd +=  conv + "\n";
                assembly.push_back(cd);
                rem.push_back({conv, s});
            }
            auto varIt1 = addressTable.temporaries.find({s,""});
            if(varIt1 != addressTable.temporaries.end()){
                //just write the reg val onto memory. now it'll be consistent
                std::string type = varIt1->type;
                std::string cd = "mov ";
                if(type == "FLOAT"){
                    cd = "movss ";
                }
                if(type == "DOUBLE"){
                    cd = "movsd ";
                }
                cd += getAsmSizeDirective(type) +" ["+ varIt1->address + "]";
                cd +=", ";
                std::string conv = registerDesc.convertRegisterForType(p.first, varIt1->type);
                cd += conv + "\n";
                assembly.push_back(cd);
                rem.push_back({conv, s});
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

bool contains_sq(std::string s){
    for(auto &i: s){
        if(i == '['){
            return true;
        }
    }
    return false;
}

std::vector<std::string> splitIndexedAccess(const std::string& input) {
    std::vector<std::string> result;

    size_t openBracket = input.find('[');
    size_t closeBracket = input.find(']');

    if (openBracket != std::string::npos && closeBracket != std::string::npos && closeBracket > openBracket) {
        std::string base = input.substr(0, openBracket);                         // before '['
        std::string index = input.substr(openBracket + 1, closeBracket - openBracket - 1); // between '[' and ']'

        result.push_back(base);
        result.push_back(index);
    } else {
        result.push_back(input); // fallback: return input as is
    }

    return result;
}


void CodeGenerator::processBasicBlock(const BasicBlockConstructor::BasicBlock& block) {
    std::stringstream blockCode;
    int paramNumber=1;
    int floatParamCount = -1 ;
    for (const auto& instruction : block.instructions) {
        std::string instr = instruction.text;
       
        std::vector<std::string> assembly;
        if(instr.empty() == true)continue;
        std::map<std::string,std::string> keywords;
        keyword_init(keywords);
        std::string type;
        std::string op = "";

        // doing this since * is used for both multiplication and dereferencing so if size if 5 then it is multiplication
        // and if size is 3 then it is dereferencing
        std::istringstream iss(instr);
        std::vector<std::string> precheck;
        std::string word;
        while (iss >> word) {
            precheck.push_back(word);
           //std::cout <<word <<" ";
        }
        
        bool found = std::any_of(keywords.begin(),keywords.end(),[&](auto &p){
            if(instr.find(p.first) != std::string::npos){
                if(p.first.length() > op.length()){
                    op = p.first;
                    type = p.second;
                }
                return false;
            }
        });
        if(op != "") found = true;
        //std::cout <<op <<" " <<type <<"\n";

        if(precheck.size() == 3)found = false;
        // Apply mapping for the specific IR instructions
        if (instr.find("label") == 0 && instr.find(":") != std::string::npos) {
            assembly.push_back(generateFunctionLabel(instr));
        }else if(instr.find("goto") != std::string::npos){

            std::istringstream iss(instr);
            std::vector<std::string> words;
            std::string word;
            while (iss >> word) {
                words.push_back(word);
                //std::cout <<word <<"\n";
            }
            //first write/store all variables to their address
            //from registers
            assembly = write_reg();
            if(instr.find("if") != std::string::npos){
                // needs handling for switch case as switch case doesnt have a temporary
                //get the relational op
                //depending on relational op, jne/jeq etc to label @L1

                // this one is non swtich case
                if(instr.find("$") != std::string::npos){
                    std::string op = addressTable.get_relop(words[1]);
                    std::string jmp;
                    std::map<std::string,std::string> op_to_j;
                    op_to_j = jump_init();
                   
                    jmp = op_to_j[op];
                    jmp += " " + words[3];
                    jmp += "\n";
                    assembly.push_back(jmp);
                }
                // now for switch case
                else{
                    std::string assembly_generated = "";
                    assembly_generated += "cmp " + getAsmSizeDirective(words[1]) + "["+addressTable.getVariableAddress(words[1])+"], " + words[3] + "\n";
                    assembly_generated += "je " + words[5] + "\n";
                    assembly.push_back(assembly_generated);
                }
                
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
                //std::cout <<word <<"\n";
            }
            // first only return is written (void)
            if(words.size() == 1){
                std::string assm = "";
                assm+= "jmp @E" + std::to_string(exit_count) + "\n";
                assembly.push_back(assm);
            }
            // other cases are handled here
            else{
                std::string return_value = words[1];
                if(!isTempOrVar(return_value)){
                    // directly return the value
                }
                else {
                    if(!addressTable.isEmpty(words[1])){
                        auto it = addressTable.getRegisterDescriptor(words[1]);
                        auto it2 = it.begin();
                        std::string reg = it2->first;
                        return_value = reg;
                    }
                    else{
                        if(isVar(return_value)){
                            return_value = getAsmSizeDirective(addressTable.getType(words[1])) + " ["+ addressTable.getVariableAddress(words[1]) +"]";
                        }
                        else{
                            return_value = getAsmSizeDirective(addressTable.getType(words[1])) + " ["+ addressTable.getTemporaryAddress(words[1]) +"]";
                        }
                    }
                }
    
                std::string assm = "mov eax, " + return_value + "\n";
                assm+= "jmp @E" + std::to_string(exit_count) + "\n";
                assembly.push_back(assm);
            }

           

        }else if (instr.find("func_begin") != std::string::npos) {
            assembly.push_back(generateFunctionBegin(instr));
        }else if (instr.find("func_end") != std::string::npos) {
            assembly.push_back(generateFunctionEnd(instr));
        }else if(instr.find("param") != std::string::npos){
            assembly = write_reg();
            // need to handle floats/doubles
            std::istringstream iss(instr);
            std::vector<std::string> words;
            std::string word;
            while (iss >> word) {
                words.push_back(word);
               
            }
            std::string reg = "";
            std::string type = "";
            std::string assm1 = "";
            bool isfloat = false;
            bool isdouble = false;
            // for float types and data section members
            

            auto it = dataSectionMap.find(words[1]);
            if(it != dataSectionMap.end()){
                type = it->second.first;
            }
            else{
                if(isTempOrVar(words[1]))type = addressTable.getType(words[1]);
                else{
                    //currently considering int operand; addition - floats will be in data section 
                    type = "INT";
                }
            }
            if(type == "DOUBLE")isdouble = true;

            // parameter no to regsiter for that call
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
            // if the reg is comming empty then its sure that it should be a floating reg
            if(reg == ""){
                if(floatParamCount == -1) floatParamCount = 0;
                reg = "xmm" + std::to_string(floatParamCount++);
                isfloat = true;
            }
            else{
                paramNumber++;
            }
            
            std::string reg2 = words[1];
            if(isTempOrVar(words[1])){
                if(addressTable.isEmpty(words[1])){
                    if(isVar(words[1])){
                        reg2 = getAsmSizeDirective(addressTable.getType(words[1])) + " ["+ addressTable.getVariableAddress(words[1]) +"]";
                    }
                    else{
                        reg2 = getAsmSizeDirective(addressTable.getType(words[1])) + " ["+ addressTable.getTemporaryAddress(words[1]) +"]";
                    }
                    
                }
                // if already in the register
                else{
                    auto it = addressTable.getRegisterDescriptor(words[1]);
                    auto it2 = it.begin();
                    reg2 = it2->first;
                }
            }else{
                if(it != dataSectionMap.end()){
                    // handling for float
                    if(it->second.first =="FLOAT"){
                        reg2 = "[" + reg2 + "]";
                    }
                }   
                else{
                    // else for not in data section
                }
            }
            std::string moveop = "mov ";
            if(isdouble == true) moveop ="movsd ";
            else if(isfloat  == true) moveop ="cvtss2sd ";
            std::string assm2 = moveop + reg + ", " + reg2 +"\n";
            assembly.push_back(assm2);

        }else if(instr.find("call") != std::string::npos){
            paramNumber = 1;
            std::istringstream iss(instr);
            std::vector<std::string> words;
            std::string word;
            while (iss >> word) {
                words.push_back(word);

            }

            std::string funcName = words[3];
            std::string assm = "";
            // need to free eax
            if(!registerDesc.isRegisterFree("eax") || registerDesc.areRelatedRegistersInUse("eax")){
            
            }
            // for var args we need to specify the no.of float operands
            funcName.pop_back();
            if((funcName == "printf")){
                if(floatParamCount == -1) floatParamCount =0;
                assm = "mov eax, " + std::to_string(floatParamCount) + "\n";
            }
            floatParamCount = -1;
            assm += "call " + funcName + "\n";
            // return value to temporary assignment
            // first is always temporary and second is always related to eax (for INT)
            
            std::string returnType = addressTable.getSymbolType(funcName);
            std::cout<<"function return type "<<returnType<<std::endl;
            if(returnType != "VOID"){
                std::string returnReg = registerDesc.getRegisterForType("rax",returnType);
                assm += "mov " + getAsmSizeDirective(addressTable.getType(words[0])) + " ["+ addressTable.getTemporaryAddress(words[0]) +"], " + returnReg + "\n"; 
            }
            
            assembly.push_back(assm);

        }else if(instr.find("cast") != std::string::npos){
            // tokenizing input
            std::istringstream iss(instr);
            std::vector<std::string> words;
            std::string word;
            while (iss >> word) {
                words.push_back(word);
                
            }
            // statement of form $1 = cast: float -> int $0
            std::string typeold = "";
            std::string typenew = "";
            size_t arrowPos = instr.find("->");

            if (arrowPos != std::string::npos) {
                // Extract the part before '->' (type before cast)
                std::string beforeCast = instr.substr(instr.find("cast:") + 6, arrowPos - (instr.find("cast:") + 6));

                // Extract the part after '->' (type after cast)
                std::string afterCast = instr.substr(arrowPos + 3);
                
                // Trim any extra spaces from the extracted strings
                beforeCast = beforeCast.substr(0, beforeCast.find_last_not_of(" \t") + 1);
                afterCast = afterCast.substr(0, afterCast.find_first_of(" \t") != std::string::npos ? afterCast.find_first_of(" \t") : afterCast.length());

                // Output the extracted types
                typeold = beforeCast;
                typenew = afterCast;
            } else {
                std::cout << "No valid cast found in the string." << std::endl;
            }
            std::transform(typeold.begin(), typeold.end(), typeold.begin(), ::tolower);
            std::transform(typenew.begin(), typenew.end(), typenew.begin(), ::tolower);

            std::string reg_var_const_2 = words[words.size()-1];
           
            bool isval = 0;
            if(isTempOrVar(reg_var_const_2)){
                if(addressTable.isEmpty(reg_var_const_2)){
                    if(isVar(reg_var_const_2)){
                        reg_var_const_2 = getAsmSizeDirective(addressTable.getType(reg_var_const_2)) + " ["+ addressTable.getVariableAddress(reg_var_const_2) +"]";
                    }
                    else{
                        reg_var_const_2 = getAsmSizeDirective(addressTable.getType(reg_var_const_2)) + " ["+ addressTable.getTemporaryAddress(reg_var_const_2) +"]";
                    }
                }
                // if already in the register
                else{
                    auto it = addressTable.getRegisterDescriptor(reg_var_const_2);
                    auto it2 = it.begin();
                    reg_var_const_2 = it2->first;
                }
            }
            else{
                auto it = dataSectionMap.find(reg_var_const_2);
                if(it!= dataSectionMap.end()){
                    if(it->second.first == "FLOAT"){
                        reg_var_const_2 = "[" + reg_var_const_2 + "]";
                    }
                }
                isval = 1;
            }
           
            // the type to convert to
            std::string type_to_convert_to = typenew;
            // the assembly that will be generated
            std::string assm ="";
            // storing the converted type directly in memory
            std::string temporary_operand  = getAsmSizeDirective(addressTable.getType(words[0])) + " ["+ addressTable.getTemporaryAddress(words[0]) +"]";
            // from float to something
            if(typeold == "float"){
                if(type_to_convert_to == "double"){
                    assm =  "movss xmm7, " + reg_var_const_2 + "\n";
                    assm += "cvtss2sd xmm7, xmm7\n";
                    assm += "movsd "+ temporary_operand + ", xmm7\n";
                }
                // all the int types will be handled here
                else if(type_to_convert_to == "int"){
                    std::string result = type_to_convert_to;
                    std::transform(result.begin(), result.end(), result.begin(),    
                                [](unsigned char c) { return std::toupper(c); });
                    std::string normal_reg_used = registerDesc.getRegisterForType("r11",result);
                    assm = "movss xmm7, " + reg_var_const_2 + "\n";
                    assm += "cvttss2si " + normal_reg_used + ", xmm7\n";
                    assm += "mov " + temporary_operand + ", " + normal_reg_used + "\n";
                }
                else if(type_to_convert_to == "long"){
                    std::string result = type_to_convert_to;
                    std::transform(result.begin(), result.end(), result.begin(),    
                                [](unsigned char c) { return std::toupper(c); });
                    std::string normal_reg_used = registerDesc.getRegisterForType("r11",result);
                    assm = "movss xmm7, " + reg_var_const_2 + "\n";
                    assm += "cvttss2si " + normal_reg_used + ", xmm7\n";
                    assm += "mov " + temporary_operand + ", " + normal_reg_used + "\n";
                }
                else if(type_to_convert_to == "short"){
                    std::string result = type_to_convert_to;
                    std::transform(result.begin(), result.end(), result.begin(),    
                                [](unsigned char c) { return std::toupper(c); });
                    std::string normal_reg_used = registerDesc.getRegisterForType("r11",result);
                    assm = "movss xmm7, " + reg_var_const_2 + "\n";
                    assm += "cvttss2si r11d, xmm7\n";
                    assm += "mov " + normal_reg_used +", " + normal_reg_used+"\n";
                    assm += "mov " + temporary_operand + ", " + normal_reg_used + "\n";
                }
                else if(type_to_convert_to == "char"){
                    std::string result = type_to_convert_to;
                    std::transform(result.begin(), result.end(), result.begin(),    
                                [](unsigned char c) { return std::toupper(c); });
                    std::string normal_reg_used = registerDesc.getRegisterForType("r11",result);
                    assm = "movss xmm7, " + reg_var_const_2 + "\n";
                    assm += "cvttss2si r11d, xmm7\n";
                    assm += "mov " + normal_reg_used +", " + normal_reg_used+"\n";
                    assm += "mov " + temporary_operand + ", " + normal_reg_used + "\n";
                }
                
            }
            // from int to something
            else if(typeold == "int"){
                if(type_to_convert_to == "float"){
                    assm = "mov r11d, " + reg_var_const_2 + "\n";
                    assm += "cvtsi2ss xmm7, r11d\n";
                    assm += "movss " + temporary_operand + ", xmm7\n";
                }
                else if(type_to_convert_to == "double"){
                    assm = "mov r11d, " + reg_var_const_2 + "\n";
                    assm += "cvtsi2sd xmm7, r11d\n";
                    assm += "movsd " + temporary_operand + ", xmm7\n";
                }
                else if(type_to_convert_to == "char"){
                    assm = "mov r11d, " + reg_var_const_2 + "\n";
                    assm += "movsx r11d, r11b\n";
                    assm += "mov DWORD ["+ addressTable.getTemporaryAddress(words[0]) +"]" + ", r11d\n";
                }
                else if(type_to_convert_to == "short"){
                    assm = "mov r11d, " + reg_var_const_2 + "\n";
                    assm += "movsx r11d, r11w\n";
                    assm += "mov DWORD ["+ addressTable.getTemporaryAddress(words[0]) +"]" + ", r11d\n";
                }
                else if(type_to_convert_to == "long"){
                    assm = "mov r11d, " + reg_var_const_2 + "\n";
                    assm = "movsx r11, r11d\n";
                    assm += "mov " + temporary_operand + ",r11\n";
                }
            }
            // from double to something
            else if(typeold == "double"){
                if(type_to_convert_to == "float"){
                    assm =  "movsd xmm7, " + reg_var_const_2 + "\n";
                    assm += "cvtsd2ss xmm7, xmm7\n";
                    assm += "movss "+ temporary_operand + ", xmm7\n";
                }
                // all the int types will be handled here
                else if(type_to_convert_to == "int"){
                    std::string result = type_to_convert_to;
                    std::transform(result.begin(), result.end(), result.begin(),    
                                [](unsigned char c) { return std::toupper(c); });
                    std::string normal_reg_used = registerDesc.getRegisterForType("r11",result);
                    assm = "movsd xmm7, " + reg_var_const_2 + "\n";
                    assm += "cvttsd2si " + normal_reg_used + ", xmm7\n";
                    assm += "mov " + temporary_operand + ", " + normal_reg_used + "\n";
                }
                else if(type_to_convert_to == "long"){
                    std::string result = type_to_convert_to;
                    std::transform(result.begin(), result.end(), result.begin(),    
                                [](unsigned char c) { return std::toupper(c); });
                    std::string normal_reg_used = registerDesc.getRegisterForType("r11",result);
                    assm = "movsd xmm7, " + reg_var_const_2 + "\n";
                    assm += "cvttsd2si " + normal_reg_used + ", xmm7\n";
                    assm += "mov " + temporary_operand + ", " + normal_reg_used + "\n";
                }
                else if(type_to_convert_to == "short"){
                    std::string result = type_to_convert_to;
                    std::transform(result.begin(), result.end(), result.begin(),    
                                [](unsigned char c) { return std::toupper(c); });
                    std::string normal_reg_used = registerDesc.getRegisterForType("r11",result);
                    assm = "movsd xmm7, " + reg_var_const_2 + "\n";
                    assm += "cvttsd2si r11d, xmm7\n";
                    assm += "mov " + normal_reg_used +", " + normal_reg_used+"\n";
                    assm += "mov " + temporary_operand + ", " + normal_reg_used + "\n";
                }
                else if(type_to_convert_to == "char"){
                    std::string result = type_to_convert_to;
                    std::transform(result.begin(), result.end(), result.begin(),    
                                [](unsigned char c) { return std::toupper(c); });
                    std::string normal_reg_used = registerDesc.getRegisterForType("r11",result);
                    assm = "movsd xmm7, " + reg_var_const_2 + "\n";
                    assm += "cvttsd2si r11d, xmm7\n";
                    assm += "mov " + normal_reg_used +", " + normal_reg_used+"\n";
                    assm += "mov " + temporary_operand + ", " + normal_reg_used + "\n";
                }
            }
            // from double to something
            else if(typeold == "long"){
                if(type_to_convert_to == "float"){
                    assm = "mov r11, " + reg_var_const_2 + "\n";
                    assm += "cvtss2ss xmm7, r11\n";
                    assm += "movss " + temporary_operand + ", xmm7\n";
                }
                else if(type_to_convert_to == "double"){
                    assm = "mov r11, " + reg_var_const_2 + "\n";
                    assm += "cvtsd2ss xmm7, r11\n";
                    assm += "movsd " + temporary_operand + ", xmm7\n";
                }
                else if(type_to_convert_to == "char"){
                    assm = "mov r11, " + reg_var_const_2 + "\n";
                    assm += "movsx r11, r11b\n";
                    assm += "mov QWORD ["+ addressTable.getTemporaryAddress(words[0]) +"]" + ", r11\n";
                }
                else if(type_to_convert_to == "short"){
                    assm = "mov r11, " + reg_var_const_2 + "\n";
                    assm += "movsx r11, r11w\n";
                    assm += "mov QWORD ["+ addressTable.getTemporaryAddress(words[0]) +"]" + ", r11\n";
                }
                else if(type_to_convert_to == "int"){
                    assm = "mov r11, " + reg_var_const_2 + "\n";
                    assm = "mov r11d, r11\n";
                    assm += "mov " + temporary_operand + ",r11d\n";
                }
            }
            else if(typeold == "char"){
                if(type_to_convert_to == "float"){
                    assm = "movzx r11d, " + reg_var_const_2 + "\n";
                    if(isval) assm = "mov r11d, " + reg_var_const_2 + "\n";
                    assm += "cvtsi2ss xmm7, r11d\n";
                    assm += "movss " + temporary_operand + ", xmm7\n";
                }
                else if(type_to_convert_to == "double"){
                    assm = "movzx r11d, " + reg_var_const_2 + "\n";
                    if(isval) assm = "mov r11d, " + reg_var_const_2 + "\n";
                    assm += "cvtsi2sd xmm7, r11d\n";
                    assm += "movsd " + temporary_operand + ", xmm7\n";
                }
                else if(type_to_convert_to == "int"){
                    assm = "movsx r11d, " + reg_var_const_2 + "\n";
                    if(isval) assm = "mov r11d, " + reg_var_const_2 + "\n";
                    assm += "mov " + temporary_operand + ", r11d\n";
                }
                else if(type_to_convert_to == "short"){
                    assm = "movsx r11d, " + reg_var_const_2 + "\n";
                    if(isval) assm = "mov r11d, " + reg_var_const_2 + "\n";
                    assm += "mov " + temporary_operand + ", r11w\n";
                }
                else if(type_to_convert_to == "long"){
                    assm = "movsx r11, " + reg_var_const_2 + "\n";
                    if(isval) assm = "mov r11, " + reg_var_const_2 + "\n";
                    assm += "mov " + temporary_operand + ", r11\n";
                }
            }
            else{
                // this will be case of pointers
                assm = "mov r11, " + reg_var_const_2 + "\n";
                assm += "mov " + temporary_operand + ", r11\n";
            }

            assembly.push_back(assm);
        }else if(found == true){
            if(type == "arithmetic"){
                
                assembly = generateArithmetic(instr);
            }
            else if(type == "cmp"){
                
                assembly = generateCMP(instr, op);
            }
            else if(type == "bitwise"){

                assembly = generateBitwise(instr);
            }
            else if(type == "shifts" ){

                assembly = generateShifts(instr);
            }
        }else if(instr.find("=") != std::string::npos){
            //assignment
            
            std::istringstream iss(instr);
            std::vector<std::string> words;
            std::string word;
            while (iss >> word) {
                words.push_back(word);
                
            }
            // assignment for variable with constant value
            // a = b
            
            if(!isTempOrVar(words[2])){
                //rhs is const
               
                
                std::string assm ="";
                // if its an array type opearnd
                if(contains_sq(words[0])){
                    std::vector<std::string> str = splitIndexedAccess(words[0]);
                    //x#block1 , second is $0
                    //i need to store at rbp - X + $0
                    std::string add = addressTable.getVariableAddress(str[0]);
                   
                    std::string type = addressTable.getType(str[0]);
                    
                    ///rhs is const
                    //dont do getreg, it sends const i need address.
                    
                    std::string reg = registerDesc.getAvailableRegister("LONG");
                    //use 8byte reg for address calc.
                    std::vector<std::string> cannot_spill;
                    
                    // spill in case all registers are in use
                    if(reg == ""){
                        std::vector<std::string> spill = registerDesc.spillRegister(cannot_spill,type);
                        
                        // first is the free reg, second is the spilled reg, third is the variable or temporary it was holding
                        reg = spill[0];
                        cannot_spill.push_back(reg);
                        std::string var_temp = spill[2];
                        std::string reg_spilled = spill[1];
                        std::string spill_assm = "";
                        if(isVar(var_temp)){
                            std::string mov_op = "mov ";
                            if(type == "FLOAT") mov_op = "movss ";
                            if(type == "DOUBLE") mov_op = "movsd ";
                            spill_assm = mov_op + getAsmSizeDirective(type) + " [" + addressTable.getVariableAddress(var_temp) + "], " + reg_spilled + "\n";
                        }
                        else{
                            std::string mov_op = "mov ";
                            if(type == "FLOAT") mov_op = "movss ";
                            if(type == "DOUBLE") mov_op = "movsd ";
                            spill_assm = mov_op + getAsmSizeDirective(type) + " [" + addressTable.getTemporaryAddress(var_temp) + "], " + reg_spilled + "\n";
                        }
                        assembly.push_back(spill_assm);
                        addressTable.removeRegisterFromDescriptor(var_temp,reg_spilled);
                    }
                    addressTable.addRegisterToDescriptor(str[0],reg,"0");
                    std::string cd1 = "lea " + reg + ", [" + addressTable.getVariableAddress(str[0]) + "] \n";
                    cannot_spill.push_back(reg);
                    std::string reg1 = reg;
                    reg = registerDesc.getAvailableRegister("LONG");
                    if(reg == ""){
                        std::vector<std::string> spill = registerDesc.spillRegister(cannot_spill,type);
                        
                        // first is the free reg, second is the spilled reg, third is the variable or temporary it was holding
                        reg = spill[0];
                        cannot_spill.push_back(reg);
                        std::string var_temp = spill[2];
                        std::string reg_spilled = spill[1];
                        std::string spill_assm = "";
                        if(isVar(var_temp)){
                            std::string mov_op = "mov ";
                            if(type == "FLOAT") mov_op = "movss ";
                            if(type == "DOUBLE") mov_op = "movsd ";
                            spill_assm = mov_op + getAsmSizeDirective(type) + " [" + addressTable.getVariableAddress(var_temp) + "], " + reg_spilled + "\n";
                        }
                        else{
                            std::string mov_op = "mov ";
                            if(type == "FLOAT") mov_op = "movss ";
                            if(type == "DOUBLE") mov_op = "movsd ";
                            spill_assm = mov_op + getAsmSizeDirective(type) + " [" + addressTable.getTemporaryAddress(var_temp) + "], " + reg_spilled + "\n";
                        }
                        assembly.push_back(spill_assm);
                        addressTable.removeRegisterFromDescriptor(var_temp,reg_spilled);
                    }
                    //the final temporary will always have a register since it is being written just above.
                    auto it = addressTable.getRegisterDescriptor(str[1]);
                    auto it2 = it.begin();
                    std::string g = it2->first;
                    std::string rrr = get64BitRegister(g); //ill use the 64bit for LEA
                    cd1 += "mov r11, " + rrr + "\n";  
                    addressTable.addRegisterToDescriptor(str[0],reg,"0");
                    cd1 += "lea " + reg + ", [" + reg1 + " + " + "r11" + "] \n";
                    cd1 += "mov " +  getAsmSizeDirective(addressTable.getType(str[0])) + " [" + reg + "] , " + words[2] +"\n";
                    assm = cd1;


                    //NOW , for reg 
                }
                else{
                    std::string type = addressTable.getType(words[0]);
                    // if already not in the register
                    if(addressTable.isEmpty(words[0])){
                        if(isVar(words[0])){
                            assm = "mov " + getAsmSizeDirective(type) + " ["+ addressTable.getVariableAddress(words[0]) +"], " + words[2] + "\n";
                            if(type == "FLOAT" ){
                                assm = "movss xmm7, [" + words[2] + "]" + "\n";
                                assm += "movss [" + addressTable.getVariableAddress(words[0]) +"], " + "xmm7" + "\n";
                            }
                            
                        }
                        else{
                            assm = "mov " + getAsmSizeDirective(addressTable.getType(words[0])) + " ["+ addressTable.getTemporaryAddress(words[0]) +"], " + words[2] + "\n";
                            if(addressTable.getType(words[0]) == "FLOAT" ){
                                assm = "movss xmm7, [" + words[2] + "]" + "\n";
                                assm += "movss [" + addressTable.getTemporaryAddress(words[0]) +"], " + "xmm7" + "\n";
                            }
                        }
                        
                    }
                    else{
                            // if already in the register
                        auto it = addressTable.getRegisterDescriptor(words[0]);
                        auto it2 = it.begin();
                        std::string reg = it2->first;
                        assm = "mov " + reg + ", " + words[2] + "\n";
                        if(isVar(words[0])){
                            if(type == "FLOAT" ){
                                assm = "movss " + reg  + "[" + words[2] + "]" + "\n";
                            }
                        }
                        else{
                            if(addressTable.getType(words[0]) == "FLOAT" ){
                                assm = "movss " + reg  + "[" + words[2] + "]" + "\n";
                            }
                        }
                    }
                }
                
                
                assembly.push_back(assm);
            }
            else{
                //rhs is var too
                // first lets get the reg or address for the 2nd operand
                bool float_  = false;
                bool double_ = false;
                std::vector<std::string> cannot_spill;
                std::string reg2 = "";
                std::string fir ;
                std::string sec;
                if(contains_sq(words[2]) == false){
                    // isme p chudh gaya p#block1
                    // new addition
                    // handling the case for address assignment only in case of variables
                    // first check if assigning an address(assuming that we will not assign the pointer address with array operand for now)
                    fir = words[2];
                    if(words[2].find("&") != std::string::npos){
                        std::string actual_variable = words[2].substr(1,words[2].length()-1);
                        std::string type1 = addressTable.getType(actual_variable);
                        std::string command_ = "";
                        if(isVar(words[2])){
                            command_ = "lea r11, [" + addressTable.getVariableAddress(actual_variable) + "]\n";
                        }
                        else{
                            command_ = "lea r11, [" + addressTable.getTemporaryAddress(actual_variable) + "]\n";
                        }
                        assembly.push_back(command_);
                        reg2 = "r11";
                        
                    }
                    else if(words[2].find("*") != std::string::npos){
                       
                        std::string actual_word = words[2].substr(1,words[2].length()-1);
                        std::string type1 = addressTable.getType(actual_word);
                        std::string command_ = "";
                        if(isVar(words[2])){
                            command_ = "mov r11, QWORD [" + addressTable.getVariableAddress(actual_word) + "]\n";
                        }
                        else{
                            command_ = "mov r11, QWORD [" + addressTable.getTemporaryAddress(actual_word) + "]\n";
                        }
                        assembly.push_back(command_);

                        type1.pop_back();
                        std::string asmType = getAsmSizeDirective((type1));
                        reg2 = "QWORD [r11]";
                    }
                    else if(addressTable.isEmpty(words[2])){
                        std::string type = addressTable.getType(words[2]);
                        if(isVar(words[2])){
                            reg2 = getAsmSizeDirective(type) + " ["+ addressTable.getVariableAddress(words[2]) +"]";
                            if(type == "FLOAT")float_ = true;
                            if(type == "DOUBLE")double_ = true;
                        }
                        else{
                            reg2 = getAsmSizeDirective(addressTable.getType(words[2])) + " ["+ addressTable.getTemporaryAddress(words[2]) +"]";
                            if(type == "FLOAT")float_ = true;
                            if(type == "DOUBLE")double_ = true;
                        }
                    }
                    // if already in the register
                    else{
                        std::string words2_type = addressTable.getType(words[2]);
                        auto it = addressTable.getRegisterDescriptor(words[2]);
                        auto it2 = it.begin();
                        reg2 = it2->first;
                        if(words2_type == "FLOAT")double_ = true;
                        if(words2_type == "DOUBLE")double_ = true;
                    }
                  
                }
                else{
                    //indexin case
                    std::vector<std::string> str = splitIndexedAccess(words[2]);
                    //x#block1 , second is $0
                    //i need to store at rbp - X + $0
                    std::string add = addressTable.getVariableAddress(str[0]);
                   
                    fir = str[0];
                    sec=  str[1];
                    ///rhs is const
                    //dont do getreg, it sends const i need address.
                  
                    std::string reg = registerDesc.getAvailableRegister("LONG");
                    
                    // spill in case all registers are in use
                    if(reg == ""){
                        std::vector<std::string> spill = registerDesc.spillRegister(cannot_spill,"LONG");
                        
                        // first is the free reg, second is the spilled reg, third is the variable or temporary it was holding
                        reg = spill[0];
                        cannot_spill.push_back(reg);
                        std::string var_temp = spill[2];
                        std::string reg_spilled = spill[1];
                        std::string spill_assm = "";
                        if(isVar(var_temp)){
                            std::string mov_op = "mov ";
                            spill_assm = mov_op + getAsmSizeDirective("LONG") + " [" + addressTable.getVariableAddress(var_temp) + "], " + reg_spilled + "\n";
                        }
                        else{
                            std::string mov_op = "mov ";
                            spill_assm = mov_op + getAsmSizeDirective("LONG") + " [" + addressTable.getTemporaryAddress(var_temp) + "], " + reg_spilled + "\n";
                        }
                        assembly.push_back(spill_assm);
                        addressTable.removeRegisterFromDescriptor(var_temp,reg_spilled);
                    }
                    addressTable.addRegisterToDescriptor(str[0],reg,"0");
                    std::string cd1 = "lea " + reg + ", [" + addressTable.getVariableAddress(str[0]) + "] \n";
                    cannot_spill.push_back(reg);
                    std::string reg1 = reg;
                    reg = registerDesc.getAvailableRegister("LONG");
                    if(reg == ""){
                        std::vector<std::string> spill = registerDesc.spillRegister(cannot_spill,"LONG");
                        
                        // first is the free reg, second is the spilled reg, third is the variable or temporary it was holding
                        reg = spill[0];
                        cannot_spill.push_back(reg);
                        std::string var_temp = spill[2];
                        std::string reg_spilled = spill[1];
                        std::string spill_assm = "";
                        if(isVar(var_temp)){
                            std::string mov_op = "mov ";
                            spill_assm = mov_op + getAsmSizeDirective("LONG") + " [" + addressTable.getVariableAddress(var_temp) + "], " + reg_spilled + "\n";
                        }
                        else{
                            std::string mov_op = "mov ";
                            spill_assm = mov_op + getAsmSizeDirective("LONG") + " [" + addressTable.getTemporaryAddress(var_temp) + "], " + reg_spilled + "\n";
                        }
                        assembly.push_back(spill_assm);
                        addressTable.removeRegisterFromDescriptor(var_temp,reg_spilled);
                    }
                    //the final temporary will always have a register since it is being written just above.
                    auto it = addressTable.getRegisterDescriptor(str[1]);
                    auto it2 = it.begin();
                    addressTable.addRegisterToDescriptor(str[0],reg,"0");
                   //it2->first ko make r11d se r11
                   std::string rrr = get64BitRegister(it2->first);
                    cd1 += "lea " + reg + ", [" + reg1 + " + " +rrr+ "] \n";
                    cannot_spill.push_back(reg);
                    //reg contains effective address of opn
                    
                    std::string reg3 = reg;
                    std::string type = addressTable.getType(str[0]);
                    reg = registerDesc.getAvailableRegister(addressTable.getType(str[0]));

                    //assume rhs is 8byte and it'll adapt to any size of LHS.
                    
                    if(reg == ""){
                        std::vector<std::string> spill = registerDesc.spillRegister(cannot_spill,addressTable.getType(str[0]));
                      
                        // first is the free reg, second is the spilled reg, third is the variable or temporary it was holding
                        reg = spill[0];
                        cannot_spill.push_back(reg);
                        std::string var_temp = spill[2];
                        std::string reg_spilled = spill[1];
                        std::string spill_assm = "";
                        if(isVar(var_temp)){
                            std::string mov_op = "mov ";
                            if(addressTable.getType(str[0]) == "FLOAT") mov_op = "movss ";
                            if(addressTable.getType(str[0]) == "DOUBLE") mov_op = "movsd ";
                            spill_assm = mov_op + getAsmSizeDirective(addressTable.getType(str[0])) + " [" + addressTable.getVariableAddress(var_temp) + "], " + reg_spilled + "\n";
                        }
                        else{
                            std::string mov_op = "mov ";
                            if(addressTable.getType(str[0]) == "FLOAT") mov_op = "movss ";
                            if(addressTable.getType(str[0]) == "DOUBLE") mov_op = "movsd ";
                            spill_assm = mov_op + getAsmSizeDirective(addressTable.getType(str[0])) + " [" + addressTable.getTemporaryAddress(var_temp) + "], " + reg_spilled + "\n";
                        }
                        assembly.push_back(spill_assm);
                        addressTable.removeRegisterFromDescriptor(var_temp,reg_spilled);
                    }
                    cannot_spill.push_back(reg);
                    //reg3 is 64 bit register.
                    cd1 += "mov " + reg + ", " + getAsmSizeDirective(addressTable.getType(str[0])) + " [" + reg3 + "]" + "\n";
                    reg2 = reg;
                    assembly.push_back(cd1);
                }
               
                
                // now for the 1st operand
                std::string reg1 = "";
                if(contains_sq(words[0]) == false){
                    if(addressTable.isEmpty(words[0])){
                        std::string type = addressTable.getType(words[0]);
                        if(isVar(words[0])){
                            reg1 = getAsmSizeDirective(addressTable.getType(words[0])) + " ["+ addressTable.getVariableAddress(words[0]) +"]";
                            if(type == "FLOAT")float_ = true;
                            if(type == "DOUBLE")double_ = true;
                        }
                        else{
                            reg1 = getAsmSizeDirective(addressTable.getType(words[0])) + " ["+ addressTable.getTemporaryAddress(words[0]) +"]";
                            if(type == "FLOAT")float_ = true;
                            if(type == "DOUBLE")double_ = true;
                        }
                    }
                    // if already in the register
                    else{
                        auto it = addressTable.getRegisterDescriptor(words[0]);
                        std::string words0_type = addressTable.getType(words[0]);
                        auto it2 = it.begin();
                        reg1 = it2->first;
                        if(words0_type == "FLOAT")float_ =true;
                        if(words0_type == "DOUBLE")double_ =true;

                    }
                    
                    std::string regg = registerDesc.convertRegisterForType(reg2, addressTable.getType(words[0]));
                    std::string assm = "mov " + reg1 + ", " +regg + "\n";
                    
                  
                    if(float_){
                        assm = "movss " + reg1 + ", " + reg2 + "\n";
                        if((reg1.find("[") != std::string::npos)  && (reg2.find("[") != std::string::npos)){
                            assm = "movss xmm7, " + reg2 +"\n";
                            assm += "movss " + reg1 +", xmm7 \n";
                        }
                    }
                    else if(double_){
                        assm = "movsd " + reg1 + ", " + reg2 + "\n";
                        if((reg1.find("[") != std::string::npos)  && (reg2.find("[") != std::string::npos)){
                            assm = "movsd xmm7, " + reg2 +"\n";
                            assm += "movsd " + reg1 +", xmm7 \n";
                        }
                    }
                    else{
                        if((reg1.find("[") != std::string::npos)  && (reg2.find("[") != std::string::npos)){
                            std::string type = addressTable.getType(words[2]);
                           
                            std::string regused  = "r11";
                            regused = registerDesc.convertRegisterForType("r11", addressTable.getType(fir));
                            assm = "mov " + regused + ", " + reg2 +"\n";
                            regused = registerDesc.convertRegisterForType("r11", addressTable.getType(words[0]));
                            assm += "mov " + reg1 +", " + regused + "\n";
                        }
                    }
                    assembly.push_back(assm);
                }
                else{
                    //first is indexed
                    //indexin case
                    std::vector<std::string> str = splitIndexedAccess(words[0]);
                    //x#block1 , second is $0
                    //i need to store at rbp - X + $0
                    std::string add = addressTable.getVariableAddress(str[0]);
                   
                    std::string type = addressTable.getType(str[0]);
                    ///rhs is const
                    //dont do getreg, it sends const i need address.
               
                    std::string reg = registerDesc.getAvailableRegister("LONG");
                    // spill in case all registers are in use
                    if(reg == ""){
                        std::vector<std::string> spill = registerDesc.spillRegister(cannot_spill,"LONG");
                       
                        // first is the free reg, second is the spilled reg, third is the variable or temporary it was holding
                        reg = spill[0];
                        cannot_spill.push_back(reg);
                        std::string var_temp = spill[2];
                        std::string reg_spilled = spill[1];
                        std::string spill_assm = "";
                        if(isVar(var_temp)){
                            std::string mov_op = "mov ";
                            spill_assm = mov_op + getAsmSizeDirective("LONG") + " [" + addressTable.getVariableAddress(var_temp) + "], " + reg_spilled + "\n";
                        }
                        else{
                            std::string mov_op = "mov ";
                            spill_assm = mov_op + getAsmSizeDirective("LONG") + " [" + addressTable.getTemporaryAddress(var_temp) + "], " + reg_spilled + "\n";
                        }
                        assembly.push_back(spill_assm);
                        addressTable.removeRegisterFromDescriptor(var_temp,reg_spilled);
                    }
                    addressTable.addRegisterToDescriptor(str[0],reg,"0");
                    std::string cd1 = "lea " + reg + ", [" + addressTable.getVariableAddress(str[0]) + "] \n";
                    cannot_spill.push_back(reg);
                    std::string reg1 = reg;
                    reg = registerDesc.getAvailableRegister("LONG");
                    if(reg == ""){
                        std::vector<std::string> spill = registerDesc.spillRegister(cannot_spill,"LONG");
                       
                        // first is the free reg, second is the spilled reg, third is the variable or temporary it was holding
                        reg = spill[0];
                        cannot_spill.push_back(reg);
                        std::string var_temp = spill[2];
                        std::string reg_spilled = spill[1];
                        std::string spill_assm = "";
                        if(isVar(var_temp)){
                            std::string mov_op = "mov ";
                            spill_assm = mov_op + getAsmSizeDirective("LONG") + " [" + addressTable.getVariableAddress(var_temp) + "], " + reg_spilled + "\n";
                        }
                        else{
                            std::string mov_op = "mov ";
                            spill_assm = mov_op + getAsmSizeDirective("LONG") + " [" + addressTable.getTemporaryAddress(var_temp) + "], " + reg_spilled + "\n";
                        }
                        assembly.push_back(spill_assm);
                        addressTable.removeRegisterFromDescriptor(var_temp,reg_spilled);
                        
                    }
                    addressTable.addRegisterToDescriptor(str[0],reg,"0");
                    //the final temporary will always have a register since it is being written just above.
                    auto it = addressTable.getRegisterDescriptor(str[1]);
                    auto it2 = it.begin();
                    std::string rrr = get64BitRegister(it2->first);
                    cd1 += "mov r11, " + rrr + "\n";

                    cd1 += "lea " + reg + ", [" + reg1 + " + " + "r11" + "] \n";
                    assembly.push_back(cd1);
                    //reg contains effective address of opn
                    reg = getAsmSizeDirective(addressTable.getType(str[0])) + " ["+ reg +"]";
                    std::string conv_reg = registerDesc.convertRegisterForType(reg2,addressTable.getType(str[0]));
                    std::string assm = "mov " + reg+", " + conv_reg +"\n";
                    
                    
                    reg1 = reg;
                    if((reg1.find("[") != std::string::npos)  && (reg2.find("[") != std::string::npos)){
                        std::string regused = registerDesc.convertRegisterForType("r11",addressTable.getType(str[0]));
                        assm = "mov " + regused + ", " + reg2 +"\n";
                        assm += "mov " + reg1 +", " + regused + "\n";
                      
                    }
                    assembly.push_back(assm);
                }
                
                
                //if case of array, i think you should write in memaddress immediately
                //ie in the above else, just write it completely inside.
               
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
    finalCode << "\tglobal _start\n";
    finalCode << "\textern printf\n";  // Add external declaration for printf
    finalCode << "\textern scanf\n";   // Add external declaration for scanf
    finalCode << "\textern free\n";    // Add external declaration for free
    finalCode << "\textern malloc\n";  // Add external declaration for malloc
    finalCode << "\textern exit\n\n";  // Add external declaration for exit
    finalCode << "_start:\n";
    finalCode << "\tand rsp, 0xfffffffffffffff0\n";
    finalCode << "\tcall main\n";
    finalCode << "\tmov rdi, rax\n";
    finalCode << "\tcall exit\n\n";

    
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
    return "QWORD";  // Most common fallback
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
        else if (type == "FLOAT") {
            // For floating point values
            dataSection << alias << " dd " << value << "\n";
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

std::string CodeGenerator::replaceCharConstants(const std::string& irCode) {
    std::istringstream stream(irCode);
    std::string line;
    std::ostringstream result;
    
    std::regex charConstantPattern(R"('(.)')");  // Basic pattern for single char
    std::regex escapedCharPattern(R"('(\\[nrt\'\"\\])')");  // Pattern for escaped chars
    
    while (std::getline(stream, line)) {
        std::string processedLine = line;
        
        // First handle escaped characters
        auto startEsc = std::sregex_iterator(processedLine.begin(), processedLine.end(), escapedCharPattern);
        auto endEsc = std::sregex_iterator();
        
        // Process matches from end to start to avoid invalidating positions
        std::vector<std::pair<std::smatch, int>> escapedMatches;
        for (auto it = startEsc; it != endEsc; ++it) {
            escapedMatches.push_back({*it, 0});
        }
        
        // Process from end to start
        for (int i = escapedMatches.size() - 1; i >= 0; i--) {
            std::smatch match = escapedMatches[i].first;
            std::string escapedChar = match[1];
            int asciiValue = 0;
            
            if (escapedChar == "\\n") asciiValue = 10;      // newline
            else if (escapedChar == "\\r") asciiValue = 13; // carriage return
            else if (escapedChar == "\\t") asciiValue = 9;  // tab
            else if (escapedChar == "\\'") asciiValue = 39; // single quote
            else if (escapedChar == "\\\"") asciiValue = 34; // double quote
            else if (escapedChar == "\\\\") asciiValue = 92; // backslash
            
            processedLine.replace(match.position(), match.length(), std::to_string(asciiValue));
        }
        
        // Then handle regular characters
        auto start = std::sregex_iterator(processedLine.begin(), processedLine.end(), charConstantPattern);
        auto end = std::sregex_iterator();
        
        // Process matches from end to start to avoid invalidating positions
        std::vector<std::pair<std::smatch, int>> matches;
        for (auto it = start; it != end; ++it) {
            matches.push_back({*it, 0});
        }
        
        // Process from end to start
        for (int i = matches.size() - 1; i >= 0; i--) {
            std::smatch match = matches[i].first;
            char c = match[1].str()[0];
            int asciiValue = static_cast<int>(c);
            
            processedLine.replace(match.position(), match.length(), std::to_string(asciiValue));
        }
        
        result << processedLine << "\n";
    }
    
    return result.str();
}

std::string CodeGenerator::processFloatConstants(const std::string& irCode) {
    std::string processedCode = irCode;
    // Regular expression to match floating point numbers (e.g., 1.0, 12.23, .5, etc.)
    std::regex floatPattern(R"((\d+\.\d*|\.\d+)([eE][+-]?\d+)?)");
    std::smatch match;
    std::string::const_iterator searchStart(irCode.cbegin());
    int floatCount = 0;
    
    while (std::regex_search(searchStart, irCode.cend(), match, floatPattern)) {
        std::string floatLiteral = match[0]; // The complete float value
        
        // Check if we've already created an entry for this float literal
        bool found = false;
        std::string alias;
        
        for (const auto& entry : dataSectionMap) {
            if (entry.second.second == floatLiteral) {
                found = true;
                alias = entry.first;
                break;
            }
        }
        
        if (!found) {
            // Create new alias for this float
            alias = "float" + std::to_string(++floatCount);
            dataSectionMap[alias] = {"FLOAT", floatLiteral};
        }
        
        // Move search position
        searchStart = match.suffix().first;
        
        // Replace all occurrences in the code that are standalone literals
        // (Not part of variable names or other identifiers)
        std::string pattern = "\\b" + std::regex_replace(floatLiteral, std::regex("\\."), "\\.") + "\\b";
        std::regex replacePattern(pattern);
        processedCode = std::regex_replace(processedCode, replacePattern, alias);
    }
    
    return processedCode;
}

