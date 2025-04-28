#include "AddressAllocationTable.hpp"
#include <iostream>
#include <regex>
#include <sstream>
#include <algorithm>
#include <map>

AddressAllocationTable::AddressAllocationTable(const std::string& irCode, SymbolTable& st)
    : symbolTable(&st) {
    parseIRCode(irCode);
}

void AddressAllocationTable::setSymbolTable(SymbolTable& st) {
    symbolTable = &st;
}

void AddressAllocationTable::identifyFunctionParameters() {
    // if (!symbolTable) return;
    std::cout<<"function parameters\n";
    functionParameters.clear();
    
    // Iterate through all scopes
    for (const auto& scope : symbolTable->scopes_) {
        // Only consider function scopes
        if (scope->scope_name != "global" && scope->parent_scope) {
            std::string functionName = scope->scope_name;
            std::vector<std::pair<std::string, int>> params;
            
            // Two separate parameter indices
            int paramIndexFloatDouble = 1;
            int paramIndexOthers = 1;
            
            // Find parameters in this scope's symbols
            for (const auto& symbolPair : scope->ordered_symbols) {
                const SymbolTable::Symbol& symbol = symbolPair;
                if (symbol.kind == "PARAMETER") {
                    // Format parameter as "name#blockX"
                    std::string paramName = symbol.name + "#block" + std::to_string(scope->block_num);
                    
                    // Use different parameter indexing based on type
                    if (symbol.type.find("FLOAT") != std::string::npos || 
                        symbol.type.find("DOUBLE") != std::string::npos) {
                        // For float/double parameters
                        params.emplace_back(paramName, paramIndexFloatDouble++);
                    } else {
                        // For other parameter types
                        params.emplace_back(paramName, paramIndexOthers++);
                    }
                }
            }
            
            // Store parameters for this function
            if (!params.empty()) {
                functionParameters[functionName] = params;
            }
        }
    }
    
    // Debug: Print the parameter map
    std::cout << "=== Function Parameters Map ===\n";
    for (const auto& func : functionParameters) {
        std::cout << "Function: " << func.first << "\n";
        for (const auto& param : func.second) {
            std::cout << "  Parameter " << param.second << ": " << param.first << "\n";
        }
    }
}

std::string AddressAllocationTable::getVariableType(const std::string& varName) const {
    // if (!symbolTable) return "UNKNOWN";
    
    size_t hashPos = varName.find('#');
    if (hashPos == std::string::npos) return "UNKNOWN";
    
    std::string baseName = varName.substr(0, hashPos);
    std::string blockPart = varName.substr(hashPos + 1);
    
    if (blockPart.substr(0, 5) != "block") return "UNKNOWN";
    int blockNum = std::stoi(blockPart.substr(5));

    for (const auto& scope : symbolTable->scopes_) {
        if (scope->block_num == blockNum) {
            
            auto it = scope->symbol_map.find(baseName);
            std::cout<<scope->scope_name<<std::endl;
            if (it != scope->symbol_map.end()) {
                std::string fullType = it->second->type;
                // List of type qualifiers to remove
                std::vector<std::string> qualifiers = {"CONST", "volatile", "static", "register", "extern"};

                for (const auto& qualifier : qualifiers) {
                    std::string qualifierWithSpace = qualifier + " ";
                    size_t pos = fullType.find(qualifierWithSpace);
                    if (pos != std::string::npos) {
                        fullType.erase(pos, qualifierWithSpace.length());
                    }
                }

                if (!it->second->dimensions.empty()) {
                    // Remove stars based on dimension count
                    // For each dimension, remove one '*' from the end
                    for (size_t i = 0; i < it->second->dimensions.size(); i++) {
                        // Remove one '*' from the end for each dimension
                        if (fullType.back() == '*') {
                            fullType.pop_back();
                        }
                    }
                }

                return fullType;
            }
        }
    }
    return "UNKNOWN";
}

std::string AddressAllocationTable::getTempType(const std::string& tempName, const std::string& contextLine){
    // Check for explicit cast
    std::string line =contextLine;
    if (line.find("cast:") != std::string::npos) {
        size_t arrowPos = line.find("->");
        if (arrowPos != std::string::npos) {
            size_t typeStart = arrowPos + 2;
            while (typeStart < line.size() && line[typeStart] == ' ') typeStart++; // skip spaces
            size_t typeEnd = line.find(' ', typeStart);
            if (typeEnd == std::string::npos) typeEnd = line.length();
            std::string type = line.substr(typeStart, typeEnd - typeStart);
            
            // Convert type to uppercase
            std::transform(type.begin(), type.end(), type.begin(), [](unsigned char c) {
                return std::toupper(c);
            });
            
            return type;
        }
    }
    
    
    // Check for relational operators which result in boolean
    const std::vector<std::string> relops = {"<", ">", "<=", ">=", "==", "!="};
    for (const auto& op : relops) {
        if (line.find(op) != std::string::npos) {
            return "BOOL";
        }
    }
    std::cout<<line<<std::endl;
    // Check for floating point constants in the context line
    std::regex floatPattern(R"(\b\d+\.\d*|\.\d+\b)");
    if (std::regex_search(line, floatPattern)) {
        return "FLOAT";  // Or "DOUBLE" depending on your default floating point type
    }
    
    // Try to infer type from other operands
    std::regex operandPattern(R"((?:^|\s)(\$?\d+|[a-zA-Z_][\w#]*))");
    std::sregex_iterator it(line.begin(), line.end(), operandPattern);
    bool isAddress = false;
    bool isPointer = false;
    std::istringstream iss(contextLine);
    std::vector<std::string> words;
    std::string word;
    while (iss >> word) {
        words.push_back(word);
        std::cout <<word <<"\n";
    }

    if(line.find("&") != std::string::npos){
        isAddress = true;
        line = std::regex_replace(line, std::regex("&"), "");

    }
    if(line.find("*") != std::string::npos && words.size()!=5){
        isPointer = true;
        line = std::regex_replace(line, std::regex("\\*"), "");
    }
    for (; it != std::sregex_iterator(); ++it) {
        std::string operand = it->str();
        operand = std::regex_replace(operand, std::regex("^\\s+"), ""); // Remove leading whitespace
        std::cout<<"finding temp types"<<operand<<"  "<<tempName<<std::endl;
        
        if (operand == tempName) continue;
        std::cout<<"finding temp types"<<operand<<"  "<<tempName<<std::endl;
        if (operand[0] == '$') {
            for (const auto& tmp : temporaries) {
                if (tmp.name == operand) {
                    if(isPointer == true){
                        std::string type = tmp.type;
                        type.pop_back(); // Remove the last '*'
                        return type;
                    }
                    if(isAddress == true){
                        std::string type = tmp.type + "*";
                        return type;
                    }
                    return tmp.type;
                }
            }
        } else {
            for (const auto& var : variables) {
                if (var.name == operand){ 
                    
                    if(isPointer == true){
                        std::string type = var.type;
                        type.pop_back(); // Remove the last '*'
                        return type;
                    }
                    if(isAddress == true){
                        std::string type = var.type + "*";
                        return type;
                    }
                    return var.type;
                }
            }
        }
    }
    
    // Default to INT if no other type can be determined
    return "INT";
}

void AddressAllocationTable::parseIRCode(const std::string& irCode) {
    clear();
    if (!symbolTable) throw std::runtime_error("Symbol table not initialized");
    
    // First, identify and add function parameters
    identifyFunctionParameters();
    
    // Add all parameters to variables collection before parsing IR
    for (const auto& funcEntry : functionParameters) {
        for (const auto& param : funcEntry.second) {
            addVariable(param.first);
        }
    }
    
    // Continue with regular parsing
    std::map<std::string, std::string> tempContexts;
    std::regex varPattern(R"((\w+)#block\d+)");
    std::regex tempDefPattern(R"((\$\d+)\s*=)");

    // First pass: collect temporary definitions
    std::istringstream stream(irCode);
    std::string line;
    while (std::getline(stream, line)) {
        std::smatch match;
        if (std::regex_search(line, match, tempDefPattern)) {
            std::string tempName = match[1];
            if (tempContexts.find(tempName) == tempContexts.end()) {
                tempContexts[tempName] = line;
            }
        }
    }

    // Second pass: process variables and temporaries
    stream.clear();
    stream.str(irCode);
    while (std::getline(stream, line)) {
        // Process variables
        std::sregex_iterator varIt(line.begin(), line.end(), varPattern);
        for (; varIt != std::sregex_iterator(); ++varIt) {
            std::string varName = (*varIt)[0];  // Get the full match, e.g., "a#block0"
            addVariable(varName);
        }

        // Process temporaries
        std::sregex_iterator tempIt(line.begin(), line.end(), tempDefPattern);
        for (; tempIt != std::sregex_iterator(); ++tempIt) {
            std::string tempName = (*tempIt)[1];
            addTemporary(tempName, tempContexts[tempName]);
        }
    }
    
    // After collecting all variables, calculate their addresses
    calculateAddresses(irCode);
}

void AddressAllocationTable::calculateAddresses(const std::string& irCode) {
    if (!symbolTable) return;
    
    std::istringstream stream(irCode);
    std::string line;
    std::string currentFunction;
    std::map<std::string, int> offsetMap;  // Track offset within each function
    std::map<std::string, std::vector<std::string>> functionVars;  // Variables in each function
    std::map<std::string, std::vector<std::string>> functionTemps;  // Temporaries in each function
    
    // First, identify function boundaries and variables/temporaries within each function
    bool inFunction = false;
    while (std::getline(stream, line)) {
        // Check for function labels
        std::regex labelPattern(R"(label\s+(\w+):)");
        std::smatch labelMatch;
        if (std::regex_search(line, labelMatch, labelPattern)) {
            currentFunction = labelMatch[1];
            inFunction = false;
            continue;
        }
        
        // Check for function begin
        if (line.find("func_begin") != std::string::npos) {
            inFunction = true;
            offsetMap[currentFunction] = 0;
            continue;
        }
        
        // Check for function end
        if (line.find("func_end") != std::string::npos) {
            inFunction = false;
            continue;
        }
        
        // Collect variables and temporaries in the current function
        if (inFunction) {
            // Collect variables
            std::regex varPattern(R"((\w+#block\d+))");
            std::sregex_iterator varIt(line.begin(), line.end(), varPattern);
            for (; varIt != std::sregex_iterator(); ++varIt) {
                std::string varName = (*varIt)[0];
                if (std::find(functionVars[currentFunction].begin(), 
                              functionVars[currentFunction].end(), 
                              varName) == functionVars[currentFunction].end()) {
                    functionVars[currentFunction].push_back(varName);
                }
            }
            
            // Collect temporaries
            std::regex tempPattern(R"((\$\d+))");
            std::sregex_iterator tempIt(line.begin(), line.end(), tempPattern);
            for (; tempIt != std::sregex_iterator(); ++tempIt) {
                std::string tempName = (*tempIt)[0];
                if (std::find(functionTemps[currentFunction].begin(), 
                              functionTemps[currentFunction].end(), 
                              tempName) == functionTemps[currentFunction].end()) {
                    functionTemps[currentFunction].push_back(tempName);
                }
            }
        }
    }
    
    // Add parameters to functionVars if they weren't found in the IR
    for (const auto& funcEntry : functionParameters) {
        std::string funcName = funcEntry.first;
        for (const auto& param : funcEntry.second) {
            std::string paramName = param.first;
            // Check if this parameter is already in the function's variables
            auto& vars = functionVars[funcName];
            if (std::find(vars.begin(), vars.end(), paramName) == vars.end()) {
                // Parameter not found in IR, add it to function variables
                vars.push_back(paramName);
            }
        }
    }
    
    // Calculate addresses for variables and temporaries in each function
    for (const auto& funcEntry : functionVars) {
        std::string func = funcEntry.first;
        int offset = 0;
        std::vector<std::string> varsToProcess = funcEntry.second;
        std::vector<std::string> orderedVars;
        
        // First, add parameters in the correct order
        if (functionParameters.find(func) != functionParameters.end()) {
            // Sort parameters by their position
            std::vector<std::pair<std::string, int>> params = functionParameters[func];
            std::sort(params.begin(), params.end(), 
                [](const auto& a, const auto& b) { return a.second < b.second; });
            
            // Add parameters to the front of the processing queue
            for (const auto& param : params) {
                orderedVars.push_back(param.first);
                // Remove from the original list to avoid duplicates
                varsToProcess.erase(std::remove(varsToProcess.begin(), varsToProcess.end(), param.first), 
                                   varsToProcess.end());
            }
        }
        
        // Then add all other variables
        orderedVars.insert(orderedVars.end(), varsToProcess.begin(), varsToProcess.end());
        
        // Now process all variables in order (parameters first, then locals)
        for (const auto& varName : orderedVars) {
            std::string type = getVariableType(varName);
            int typeSize = symbolTable->getTypeSize(type);

            // Parse variable name to get base name and block number
            size_t hashPos = varName.find('#');
            if (hashPos != std::string::npos) {
                std::string baseName = varName.substr(0, hashPos);
                std::string blockPart = varName.substr(hashPos + 1);
                
                if (blockPart.substr(0, 5) == "block") {
                    int blockNum = std::stoi(blockPart.substr(5));
                    
                    // Find the variable in the symbol table to get dimensions
                    for (const auto& scope : symbolTable->scopes_) {
                        if (scope->block_num == blockNum) {
                            auto it = scope->symbol_map.find(baseName);
                            if (it != scope->symbol_map.end() && !it->second->dimensions.empty()) {
                                // Calculate total size for array: type size * product of dimensions
                                int totalSize = typeSize;
                                for (const auto& dim : it->second->dimensions) {
                                    totalSize *= dim;
                                }
                                typeSize = totalSize;
                            }
                            break;
                        }
                    }
                }
            }
            
            // Add padding if necessary to align to 4-byte boundary
            int padding = (4 - (offset % 4)) % 4;
            offset += padding;
            
            offset += typeSize;
            std::string address = "rbp - " + std::to_string(offset);
            
            // Update the variable's address
            setVariableAddress(varName, address);
        }
        
        // Next, process all temporaries for this function (after variables)
        auto tempIt = functionTemps.find(func);
        if (tempIt != functionTemps.end()) {
            for (const auto& tempName : tempIt->second) {
                // Find the temporary in our set
                auto tempInfoIt = temporaries.find({tempName, ""});
                if (tempInfoIt != temporaries.end()) {
                    std::string type = tempInfoIt->type;
                    int typeSize = symbolTable->getTypeSize(type);
                    
                    // Add padding if necessary to align to 4-byte boundary
                    int padding = (4 - (offset % 4)) % 4;
                    offset += padding;
                    
                    offset += typeSize;
                    std::string address = "rbp - " + std::to_string(offset);
                    
                    // Update the temporary's address
                    setTemporaryAddress(tempName, address);
                }
            }
        }
    }
}

void AddressAllocationTable::addVariable(const std::string& varName) {
    VarTempInfo info;
    info.name = varName;
    info.type = getVariableType(varName);
    info.address = "";  // Address will be set later in calculateAddresses
    
    variables.insert(info);
}

void AddressAllocationTable::addTemporary(const std::string& tempName, const std::string& contextLine) {
    VarTempInfo info;
    info.name = tempName;
    info.type = getTempType(tempName, contextLine);
    info.address = "";  // Temporaries typically don't have memory addresses
    
    temporaries.insert(info);
}

void AddressAllocationTable::setVariableAddress(const std::string& varName, const std::string& address) {
    auto it = variables.find({varName, ""});
    if (it != variables.end()) {
        // Create a new VarTempInfo with the updated address
        VarTempInfo updated = *it;
        updated.address = address;
        
        // Replace the old entry with the updated one
        variables.erase(it);
        variables.insert(updated);
    }
}

void AddressAllocationTable::setTemporaryAddress(const std::string& tempName, const std::string& address) {
    auto it = temporaries.find({tempName, ""});
    if (it != temporaries.end()) {
        // Create a new VarTempInfo with the updated address
        VarTempInfo updated = *it;
        updated.address = address;
        
        // Replace the old entry with the updated one
        temporaries.erase(it);
        temporaries.insert(updated);
    }
}

std::string AddressAllocationTable::getVariableAddress(const std::string& varName) const {
    auto it = variables.find({varName, ""});
    return (it != variables.end()) ? it->address : "";
}

std::string AddressAllocationTable::getTemporaryAddress(const std::string& tempName) const {
    auto it = temporaries.find({tempName, ""});
    return (it != temporaries.end()) ? it->address : "";
}

std::string AddressAllocationTable::getType(const std::string& Name) const {
    auto varIt = variables.find({Name, ""});
    if (varIt != variables.end()) {
        return varIt->type;
    }
    
    auto tempIt = temporaries.find({Name, ""});
    if (tempIt != temporaries.end()) {
        return tempIt->type;
    }
}
void AddressAllocationTable::set_relop(const std::string& Name, std::string op){
    //always a temporary in X = a > b
    auto tempIt = temporaries.find({Name,""});
    if(tempIt != temporaries.end()){
        VarTempInfo x = *(tempIt);
        temporaries.erase(x);
        x.relational_op = op;
        temporaries.insert(x);
    }
    else{
        std::cout <<"TEMPORARY NOT FOUND to set_relop" <<"\n";
    }
}

std::string AddressAllocationTable::get_relop(const std::string& Name){
    auto tempIt = temporaries.find({Name,""});
    if(tempIt != temporaries.end()){
        return tempIt->relational_op;
    }
    else{
        std::cout <<"TEMPORARY NOT FOUND in get_relop \n";
    }
}


void AddressAllocationTable::addRegisterToDescriptor(const std::string& varOrTempName, const std::string& regName,const std::string& value) {
    auto varIt = variables.find({varOrTempName, ""});
    if (varIt != variables.end()) {
        VarTempInfo updated = *varIt;
        updated.registerMap[regName] = value;
        variables.erase(varIt);
        variables.insert(updated);
        return;
    }
    
    auto tempIt = temporaries.find({varOrTempName, ""});
    if (tempIt != temporaries.end()) {
        VarTempInfo updated = *tempIt;
        updated.registerMap[regName] = value;
        temporaries.erase(tempIt);
        temporaries.insert(updated);
    }
}

void AddressAllocationTable::removeRegisterFromDescriptor(const std::string& varOrTempName, 
                                                         const std::string& regName) {
    auto varIt = variables.find({varOrTempName, ""});
    if (varIt != variables.end()) {
        VarTempInfo updated = *varIt;
        updated.registerMap.erase(regName);
        variables.erase(varIt);
        variables.insert(updated);
        return;
    }
    
    auto tempIt = temporaries.find({varOrTempName, ""});
    if (tempIt != temporaries.end()) {
        VarTempInfo updated = *tempIt;
        updated.registerMap.erase(regName);
        temporaries.erase(tempIt);
        temporaries.insert(updated);
    }
}

bool AddressAllocationTable::isInRegister(const std::string& varOrTempName,const std::string& regName) const {
    auto varIt = variables.find({varOrTempName, ""});
    if (varIt != variables.end()) {
        return varIt->registerMap.find(regName) != varIt->registerMap.end();
    }
    
    auto tempIt = temporaries.find({varOrTempName, ""});
    if (tempIt != temporaries.end()) {
        return tempIt->registerMap.find(regName) != tempIt->registerMap.end();
    }
    
    return false;
}

bool AddressAllocationTable::isEmpty(const std::string& varOrTempName) const{
    auto varIt = variables.find({varOrTempName, ""});
    if (varIt != variables.end()) {
        std::cout <<varOrTempName <<" \n" <<varIt->registerMap.size() <<"\n" <<"upd \n";
        return (varIt->registerMap.size())==0;
    }

    auto tempIt = temporaries.find({varOrTempName, ""});
    if (tempIt != temporaries.end()) {
        return (tempIt->registerMap.size())==0;
    }

}

std::map<std::string, std::string> AddressAllocationTable::getRegisterDescriptor(
    const std::string& varOrTempName) const {
    auto varIt = variables.find({varOrTempName, ""});
    if (varIt != variables.end()) {
        return varIt->registerMap;
    }
    
    auto tempIt = temporaries.find({varOrTempName, ""});
    if (tempIt != temporaries.end()) {
        return tempIt->registerMap;
    }
    
    return {};
}

void AddressAllocationTable::clearRegisterDescriptor(const std::string& varOrTempName) {
    auto varIt = variables.find({varOrTempName, ""});
    if (varIt != variables.end()) {
        VarTempInfo updated = *varIt;
        updated.registerMap.clear();
        variables.erase(varIt);
        variables.insert(updated);
        return;
    }
    
    auto tempIt = temporaries.find({varOrTempName, ""});
    if (tempIt != temporaries.end()) {
        VarTempInfo updated = *tempIt;
        updated.registerMap.clear();
        temporaries.erase(tempIt);
        temporaries.insert(updated);
    }
}

const std::set<VarTempInfo>& AddressAllocationTable::getVariables() const {
    return variables;
}

const std::set<VarTempInfo>& AddressAllocationTable::getTemporaries() const {
    return temporaries;
}

std::vector<VarTempInfo> AddressAllocationTable::getVariablesVector() const {
    return {variables.begin(), variables.end()};
}

std::vector<VarTempInfo> AddressAllocationTable::getTemporariesVector() const {
    return {temporaries.begin(), temporaries.end()};
}

int AddressAllocationTable::getFunctionStackSize(const std::string& functionName, const std::string& irCode) const {
    int totalSize = 0;
    
    // First, add size for parameters which we already know
    auto paramIt = functionParameters.find(functionName);
    if (paramIt != functionParameters.end()) {
        for (const auto& param : paramIt->second) {
            std::string type = getVariableType(param.first);
            int typeSize = symbolTable->getTypeSize(type);
            totalSize += typeSize;
        }
    }
    
    // Parse IR to find all variables and temporaries in this function
    std::istringstream stream(irCode);
    std::string line;
    bool inTargetFunction = false;
    std::set<std::string> functionVars;  // Use a set to avoid duplicates
    std::set<std::string> functionTemps; // Set for temporaries
    
    while (std::getline(stream, line)) {
        // Check for function label
        std::regex labelPattern("label\\s+" + functionName + ":");
        if (std::regex_search(line, labelPattern)) {
            inTargetFunction = true;
            continue;
        }
        
        // Check for function begin
        if (inTargetFunction && line.find("func_begin") != std::string::npos) {
            continue;
        }
        
        // Check for function end
        if (inTargetFunction && line.find("func_end") != std::string::npos) {
            inTargetFunction = false;
            break;
        }
        
        // If we're in the target function, collect variables and temporaries
        if (inTargetFunction) {
            // Collect variables
            std::regex varPattern(R"((\w+#block\d+))");
            std::sregex_iterator varIt(line.begin(), line.end(), varPattern);
            for (; varIt != std::sregex_iterator(); ++varIt) {
                std::string varName = (*varIt)[0];
                functionVars.insert(varName);
            }
            
            // Collect temporaries
            std::regex tempPattern(R"((\$\d+))");
            std::sregex_iterator tempIt(line.begin(), line.end(), tempPattern);
            for (; tempIt != std::sregex_iterator(); ++tempIt) {
                std::string tempName = (*tempIt)[0];
                functionTemps.insert(tempName);
            }
        }
    }
    
    // Calculate size for all local variables
    for (const auto& varName : functionVars) {
        // Skip parameters as we've already counted them
        bool isParam = false;
        if (paramIt != functionParameters.end()) {
            for (const auto& param : paramIt->second) {
                if (param.first == varName) {
                    isParam = true;
                    break;
                }
            }
        }
        
        if (!isParam) {
            std::string type = getVariableType(varName);
            int typeSize = symbolTable->getTypeSize(type);
            totalSize += typeSize;
        }
    }
    
    // Calculate size for all temporaries
    for (const auto& tempName : functionTemps) {
        auto tempInfoIt = temporaries.find({tempName, ""});
        if (tempInfoIt != temporaries.end()) {
            std::string type = tempInfoIt->type;
            int typeSize = symbolTable->getTypeSize(type);
            totalSize += typeSize;
        }
    }
    
    // Ensure 16-byte alignment
    int remainder = totalSize % 16;
    if (remainder == 0) {
        // If already aligned, add another 16 bytes as requested
        totalSize += 16;
    } else {
        // Round up to next multiple of 16
        totalSize += (16 - remainder);
    }
    
    return totalSize;
}

std::vector<std::pair<std::string, int>> AddressAllocationTable::getFunctionParameters(const  std::string funcName){
    return functionParameters[funcName];
}

void AddressAllocationTable::printTable() const {
    std::cout << "=== Variables ===\n";
    for (const auto& var : variables) {
        std::cout << var.name << " : " << var.type << " @ " << var.address << "\n";
        
        if (!var.registerMap.empty()) {
            std::cout << "  Register descriptors:\n";
            for (const auto& reg : var.registerMap) {
                std::cout << "    " << reg.first << " = " << reg.second << "\n";
            }
        }
    }
    
    std::cout << "\n=== Temporaries ===\n";
    for (const auto& tmp : temporaries) {
        std::cout << tmp.name << " : " << tmp.type;
        if (!tmp.address.empty()) {
            std::cout << " @ " << tmp.address;
        }
        std::cout << "\n";
        
        if (!tmp.registerMap.empty()) {
            std::cout << "  Register descriptors:\n";
            for (const auto& reg : tmp.registerMap) {
                std::cout << "    " << reg.first << " = " << reg.second << "\n";
            }
        }
    }
}

void AddressAllocationTable::clear() {
    variables.clear();
    temporaries.clear();
}

std::string AddressAllocationTable::getSymbolType(const std::string& name) {
    // Search through all scopes in the symbol table
    for (const auto& scope_ptr : (*symbolTable).scopes_) {
        // Use the symbol_map for faster lookup
        auto it = scope_ptr->symbol_map.find(name);
        if (it != scope_ptr->symbol_map.end()) {
            // Found the symbol - return its type
            return it->second->type;
        }
    }
    
    // Symbol not found in any scope
    return ""; // Or return a specific string indicating "not found"
}