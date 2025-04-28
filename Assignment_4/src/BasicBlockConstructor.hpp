#ifndef BASICBLOCKCONSTRUCTOR_HPP
#define BASICBLOCKCONSTRUCTOR_HPP

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <regex>

class BasicBlockConstructor {
public:
    struct Instruction {
        int index;              
        std::string text;       
        bool isLabel;           
        std::string labelName;  
        bool isBranch;          
        bool isTarget;          
        bool isIfGoto;          
        bool isGoto;            
        bool isFunctionLabel;   
        std::string targetLabel; 
        
        // New fields for dependency analysis
        std::string operation;           // The operation being performed (e.g., '+', '-', '*', 'call', etc.)
        std::vector<std::string> lhs;    // Left-hand side variables (assignment targets)
        std::vector<std::string> rhs;    // Right-hand side variables (operands, parameters)
    };

    struct BasicBlock {
        int id;
        int functionId;  
        std::vector<Instruction> instructions;
        std::unordered_set<int> successors;  
        std::unordered_set<int> predecessors; 
    };

    std::vector<BasicBlock> constructBasicBlocks(const std::string& code);
    void printBasicBlocks(const std::vector<BasicBlock>& blocks);

private:
    std::vector<Instruction> parseInstructions(const std::string& code);
    void markTargets(std::vector<Instruction>& instructions);
    std::vector<BasicBlock> constructBlocks(std::vector<Instruction>& instructions);
    void buildCFG(std::vector<BasicBlock>& blocks, const std::vector<Instruction>& instructions);
    
    // New method to extract dependencies from instructions
    void extractDependencies(std::vector<Instruction>& instructions);
    
    // Helper method to parse a single instruction for dependencies
    void parseInstructionDependencies(Instruction& instr);
};

#endif // BASICBLOCKCONSTRUCTOR_HPP