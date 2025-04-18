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
                size_t lastSpace = fullType.rfind(' ');
                return (lastSpace != std::string::npos) ? fullType.substr(lastSpace + 1) : fullType;
            }
        }
    }
    return "UNKNOWN";
}

std::string AddressAllocationTable::getTempType(const std::string& tempName, const std::string& contextLine) const {
    if (contextLine.find("cast :") != std::string::npos) {
        size_t castPos = contextLine.find("cast :");
        size_t typeEnd = contextLine.find(' ', castPos + 6);
        if (typeEnd == std::string::npos) typeEnd = contextLine.length();
        return contextLine.substr(castPos + 6, typeEnd - (castPos + 6));
    }
    
    const std::vector<std::string> relops = {"<", ">", "<=", ">=", "==", "!="};
    for (const auto& op : relops) {
        if (contextLine.find(op) != std::string::npos) {
            return "BOOL";
        }
    }
    
    std::regex operandPattern(R"((?:^|\s)(\$?\d+|[a-zA-Z_][\w#]*))");
    std::sregex_iterator it(contextLine.begin(), contextLine.end(), operandPattern);
    
    for (; it != std::sregex_iterator(); ++it) {
        std::string operand = it->str();
        if (operand == tempName) continue;
        
        if (operand[0] == '$') {
            for (const auto& tmp : temporaries) {
                if (tmp.name == operand) return tmp.type;
            }
        } else {
            for (const auto& var : variables) {
                if (var.name == operand) return var.type;
            }
        }
    }
    
    return "INT";
}

void AddressAllocationTable::parseIRCode(const std::string& irCode) {
    clear();
    if (!symbolTable) throw std::runtime_error("Symbol table not initialized");
    
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
    
    // First, identify function boundaries and variables within each function
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
        
        // Collect variables in the current function
        if (inFunction) {
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
        }
    }
    
    // Calculate addresses for variables in each function
    for (const auto& funcEntry : functionVars) {
        std::string func = funcEntry.first;
        int offset = 0;
        
        for (const auto& varName : funcEntry.second) {
            std::string type = getVariableType(varName);
            int typeSize = symbolTable->getTypeSize(type);
            
            offset += typeSize;
            std::string address = "rbp - " + std::to_string(offset);
            
            // Update the variable's address
            setVariableAddress(varName, address);
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