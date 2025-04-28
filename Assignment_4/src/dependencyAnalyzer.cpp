#include "BasicBlockConstructor.hpp"
#include <iostream>
#include <regex>
#include <string>

// Extract dependencies from all instructions
void BasicBlockConstructor::extractDependencies(std::vector<Instruction>& instructions) {
    for (auto& instr : instructions) {
        parseInstructionDependencies(instr);
    }
}

// Parse a single instruction for dependencies
void BasicBlockConstructor::parseInstructionDependencies(Instruction& instr) {
    if (instr.isLabel) {
        // Labels don't have dependencies
        return;
    }
    
    std::string text = instr.text;
    std::istringstream iss(text);
    std::string token;
    
    // Assignment regex patterns
    std::regex assignRegex(R"(^\s*(\w+)\s*=\s*(.+))");
    std::regex binaryOpRegex(R"(^\s*(\w+)\s*=\s*(\w+|\d+)\s*([\+\-\*\/\%])\s*(\w+|\d+)\s*$)");
    std::regex unaryOpRegex(R"(^\s*(\w+)\s*=\s*([\+\-\!])\s*(\w+|\d+)\s*$)");
    std::regex copyRegex(R"(^\s*(\w+)\s*=\s*(\w+|\d+)\s*$)");
    std::regex callRegex(R"(^\s*(?:(\w+)\s*=\s*)?call\s+(\w+)\s*(.*))");
    std::regex paramRegex(R"(^\s*param\s+(\w+|\d+))");
    std::regex ifGotoRegex(R"(^\s*if\s+(\w+|\d+)\s*([=!<>]+)\s*(\w+|\d+)\s+goto\s+(@L\d+))");
    std::regex gotoRegex(R"(^\s*goto\s+(@L\d+))");
    std::regex returnRegex(R"(^\s*return(?:\s+(\w+|\d+))?)");
    std::regex indexAccessRegex(R"(^\s*(\w+)\s*=\s*(\w+)\s*\[\s*(\w+|\d+)\s*\])");
    std::regex indexAssignRegex(R"(^\s*(\w+)\s*\[\s*(\w+|\d+)\s*\]\s*=\s*(\w+|\d+))");
    
    std::smatch matches;
    
    // Binary operations (a = b + c)
    if (std::regex_match(text, matches, binaryOpRegex)) {
        instr.lhs.push_back(matches[1].str());
        std::string op1 = matches[2].str();
        instr.operation = matches[3].str();
        std::string op2 = matches[4].str();
        
        if (!std::isdigit(op1[0])) instr.rhs.push_back(op1);
        if (!std::isdigit(op2[0])) instr.rhs.push_back(op2);
    }
    // Unary operations (a = -b)
    else if (std::regex_match(text, matches, unaryOpRegex)) {
        instr.lhs.push_back(matches[1].str());
        instr.operation = matches[2].str();
        std::string op = matches[3].str();
        
        if (!std::isdigit(op[0])) instr.rhs.push_back(op);
    }
    // Simple copy (a = b)
    else if (std::regex_match(text, matches, copyRegex)) {
        instr.lhs.push_back(matches[1].str());
        instr.operation = "copy";
        std::string op = matches[2].str();
        
        if (!std::isdigit(op[0])) instr.rhs.push_back(op);
    }
    // Function calls (a = call func b c d)
    else if (std::regex_match(text, matches, callRegex)) {
        if (matches[1].length() > 0) {
            instr.lhs.push_back(matches[1].str());
        }
        instr.operation = "call";
        instr.rhs.push_back(matches[2].str()); // Function name as first rhs
        
        // Parse parameters
        std::string params = matches[3].str();
        std::istringstream paramStream(params);
        std::string param;
        while (paramStream >> param) {
            if (!std::isdigit(param[0])) instr.rhs.push_back(param);
        }
    }
    // Parameter passing (param a)
    else if (std::regex_match(text, matches, paramRegex)) {
        instr.operation = "param";
        std::string param = matches[1].str();
        
        if (!std::isdigit(param[0])) instr.rhs.push_back(param);
    }
    // Conditional branch (if a < b goto L1)
    else if (std::regex_match(text, matches, ifGotoRegex)) {
        instr.operation = "if";
        std::string op1 = matches[1].str();
        std::string cond = matches[2].str();
        std::string op2 = matches[3].str();
        
        if (!std::isdigit(op1[0])) instr.rhs.push_back(op1);
        if (!std::isdigit(op2[0])) instr.rhs.push_back(op2);
    }
    // Unconditional branch (goto L1)
    else if (std::regex_match(text, matches, gotoRegex)) {
        instr.operation = "goto";
        // No dependencies
    }
    // Return statement (return a)
    else if (std::regex_match(text, matches, returnRegex)) {
        instr.operation = "return";
        if (matches[1].length() > 0) {
            std::string retVal = matches[1].str();
            if (!std::isdigit(retVal[0])) instr.rhs.push_back(retVal);
        }
    }
    // Array access (a = b[c])
    else if (std::regex_match(text, matches, indexAccessRegex)) {
        instr.lhs.push_back(matches[1].str());
        instr.operation = "array_access";
        std::string array = matches[2].str();
        std::string index = matches[3].str();
        
        instr.rhs.push_back(array);
        if (!std::isdigit(index[0])) instr.rhs.push_back(index);
    }
    // Array assignment (a[b] = c)
    else if (std::regex_match(text, matches, indexAssignRegex)) {
        std::string array = matches[1].str();
        std::string index = matches[2].str();
        std::string value = matches[3].str();
        
        instr.operation = "array_assign";
        instr.lhs.push_back(array);
        
        if (!std::isdigit(index[0])) instr.rhs.push_back(index);
        if (!std::isdigit(value[0])) instr.rhs.push_back(value);
    }
    // Generic assignment (catch-all)
    else if (std::regex_match(text, matches, assignRegex)) {
        instr.lhs.push_back(matches[1].str());
        instr.operation = "assign";
        
        // Extract RHS variables (assuming identifiers are alphanumeric and start with a letter)
        std::string rhs = matches[2].str();
        std::regex varRegex(R"(([a-zA-Z]\w*))");
        std::sregex_iterator it(rhs.begin(), rhs.end(), varRegex);
        std::sregex_iterator end;
        
        while (it != end) {
            instr.rhs.push_back((*it)[1].str());
            ++it;
        }
    }
    // No recognized pattern
    else {
        instr.operation = "unknown";
    }
}