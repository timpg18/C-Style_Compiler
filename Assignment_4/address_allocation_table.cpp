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
    if (!symbolTable) return "UNKNOWN";
    
    size_t hashPos = varName.find('#');
    if (hashPos == std::string::npos) return "UNKNOWN";
    
    std::string baseName = varName.substr(0, hashPos);
    std::string blockPart = varName.substr(hashPos + 1);
    
    if (blockPart.substr(0, 5) != "block") return "UNKNOWN";
    int blockNum = std::stoi(blockPart.substr(5));

    for (const auto& scope : symbolTable->scopes_) {
        if (scope->block_num == blockNum) {
            auto it = scope->symbol_map.find(baseName);
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
            std::string varName = (*varIt)[1];
            addVariable(varName);
        }

        // Process temporaries
        std::sregex_iterator tempIt(line.begin(), line.end(), tempDefPattern);
        for (; tempIt != std::sregex_iterator(); ++tempIt) {
            std::string tempName = (*tempIt)[1];
            addTemporary(tempName, tempContexts[tempName]);
        }
    }
}

void AddressAllocationTable::addVariable(const std::string& varName) {
    variables.insert({varName, getVariableType(varName)});
}

void AddressAllocationTable::addTemporary(const std::string& tempName, const std::string& contextLine) {
    temporaries.insert({tempName, getTempType(tempName, contextLine)});
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
        std::cout << var.name << " : " << var.type << "\n";
    }
    
    std::cout << "\n=== Temporaries ===\n";
    for (const auto& tmp : temporaries) {
        std::cout << tmp.name << " : " << tmp.type << "\n";
    }
}

void AddressAllocationTable::clear() {
    variables.clear();
    temporaries.clear();
}