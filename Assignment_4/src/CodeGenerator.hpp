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
    std::vector<std::string> generateArithmetic(const std::string& line);
    std::vector<std::string> generateCMP(const std::string& line, std::string op);
    //for cmp inst
    std::vector<std::string> getReg(const std::string& line, std::vector<std::string>& assembly, std::map<int ,int> ind);
    bool isTempOrVar(const std::string& line);
    bool isTemp(const std::string& line);
    bool isVar(const std::string& line);
    
    // Process a single basic block
    void processBasicBlock(const BasicBlockConstructor::BasicBlock& block);
    
    // Combine the block code into the final assembly
    std::string combineBlockCode();

    
    // type to ASM directive
    std::string getAsmSizeDirective(const std::string& type) const;

    std::vector<std::string> write_reg();
    // type to the PTR for addressing
    std::unordered_map<std::string, std::string> typeToAsmSize = {
        // Character types (1 byte)
        {"CHAR", "BYTE"},
        {"SIGNED CHAR", "BYTE"},
        {"UNSIGNED CHAR", "BYTE"},
    
        // Short integer types (2 bytes)
        {"SHORT", "WORD"},
        {"UNSIGNED SHORT", "WORD"},
        {"SHORT INT", "WORD"},
        {"SIGNED SHORT INT", "WORD"},
        {"UNSIGNED SHORT INT", "WORD"},
    
        // Regular integer types (4 bytes)
        {"INT", "DWORD"},
        {"UNSIGNED INT", "DWORD"},
    
        // Long integer types (8 bytes on most modern systems)
        {"LONG", "QWORD"},
        {"UNSIGNED LONG", "QWORD"},
        {"LONG INT", "QWORD"},
        {"SIGNED LONG INT", "QWORD"},
        {"UNSIGNED LONG INT", "QWORD"},
    
        // Long long integer types (8 bytes)
        {"LONG LONG", "QWORD"},
        {"UNSIGNED LONG LONG", "QWORD"},
        {"LONG LONG INT", "QWORD"},
        {"SIGNED LONG LONG INT", "QWORD"},
        {"UNSIGNED LONG LONG INT", "QWORD"},
    
        // Floating point types
        {"FLOAT", "DWORD"},         // 4 bytes
        {"DOUBLE", "QWORD"},        // 8 bytes
        {"LONG DOUBLE", "TBYTE"},   // 10 bytes (80 bits)
    
        // VOID is special – usually not directly addressable but can be mapped
        {"VOID", "BYTE"}            // Fallback, often used as generic pointer
    };

    // for data section
    std::map<std::string, std::pair<std::string, std::string>> dataSectionMap; // Maps alias -> {type, value}
    std::string processDataSectionEntries(const std::string& irCode);
    std::vector<std::string> parseStringWithEscapeSequences(const std::string& str);
    std::string generateDataSection();
    

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