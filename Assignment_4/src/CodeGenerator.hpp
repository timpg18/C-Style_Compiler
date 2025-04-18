#ifndef CODE_GENERATOR_HPP
#define CODE_GENERATOR_HPP

#include "AddressAllocationTable.hpp"
#include "RegisterDescriptor.hpp"
#include "BasicBlockConstructor.hpp"
#include <string>
#include <vector>
#include <map>

class CodeGenerator {
private:
    AddressAllocationTable addressTable;
    RegisterDescriptor registerDesc;
    std::vector<BasicBlockConstructor::BasicBlock> basicBlocks;
    std::string irCode;
    
    // Maps to store generated code for each block
    std::map<int, std::string> blockCodeMap;
    
    // Helper methods for code generation
    std::string generateFunctionLabel(const std::string& line);
    std::string generateFunctionBegin(const std::string& line);
    std::string generateFunctionEnd(const std::string& line);
    std::string generateArithmetic(const std::string& line);
    std::vector<std::string> getReg(const std::string& line);
    bool isTempOrVar(const std::string& line);
    
    // Process a single basic block
    void processBasicBlock(const BasicBlockConstructor::BasicBlock& block);
    
    // Combine the block code into the final assembly
    std::string combineBlockCode();

public:
    // Constructor that takes IR code and symbol table
    CodeGenerator(const std::string& irCode, SymbolTable& symbolTable);
    
    // Initialize the code generator components
    void initialize();
    
    // Print information from all components
    void printComponentInfo() const;
    
    // Generate assembly code
    std::string generateCode();
};

#endif 