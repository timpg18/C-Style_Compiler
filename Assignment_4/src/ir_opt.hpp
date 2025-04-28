#ifndef IR_OPTIMIZER_HPP
#define IR_OPTIMIZER_HPP

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <sstream>
#include <regex>
#include <iostream>
#include <fstream>

// Forward declaration
class IROptimizer;

// Operand Types
enum class OperandType {
    CONSTANT,
    TEMPORARY,
    VARIABLE,
    ARRAY_ACCESS,
    FUNCTION_CALL,
    LABEL,
    STRING_LITERAL,
    CHAR_LITERAL,
    PARAM,
    UNKNOWN
};

// Instruction Types
enum class InstructionType {
    ASSIGNMENT,
    ARITHMETIC,
    ARRAY_ASSIGNMENT,
    FUNCTION_CALL,
    FUNCTION_BEGIN,
    FUNCTION_END,
    LABEL,
    PARAM,
    COMPARISON,
    JUMP,
    RETURN,
    UNKNOWN
};

// Operand structure
struct Operand {
    std::string name;
    OperandType type;
    std::string value;  // For constants and evaluations
    
    // For array accesses
    std::string baseArray;
    std::string indexExpr;
    
    Operand() : type(OperandType::UNKNOWN) {}
    
    bool isConstant() const {
        return type == OperandType::CONSTANT;
    }
    
    bool isTemporary() const {
        return type == OperandType::TEMPORARY;
    }
    
    bool isVariable() const {
        return type == OperandType::VARIABLE;
    }
    
    bool isArrayAccess() const {
        return type == OperandType::ARRAY_ACCESS;
    }
    
    std::string toString() const {
        if (type == OperandType::ARRAY_ACCESS) {
            return baseArray + "[" + indexExpr + "]";
        }
        return name;
    }
};

// Instruction structure
struct Instruction {
    InstructionType type;
    Operand lhs;
    Operand rhs1;
    Operand rhs2;
    std::string operation;  // +, -, *, /, etc.
    std::vector<Operand> params;  // For function calls
    std::string originalLine;
    std::string indentation;  // Store the indentation for format preservation
    int lineIndex;
    bool isDeleted;  // Mark for deletion in optimization passes
    
    Instruction() : type(InstructionType::UNKNOWN), isDeleted(false), lineIndex(-1) {}
    
    bool hasConstantOperands() const {
        return (rhs1.isConstant() && (operation.empty() || rhs2.isConstant()));
    }
    
    std::string toString() const {
        // Return the original line if not modified
        if (!isDeleted) {
            return indentation + originalLine;
        }
        return ""; // Deleted instruction
    }
    
    // Generate a new optimized line based on updated operands
    std::string generateOptimizedLine() const {
        std::string result = indentation;
        
        switch (type) {
            case InstructionType::ASSIGNMENT:
                result += lhs.toString() + " = " + rhs1.toString();
                break;
                
            case InstructionType::ARITHMETIC:
                result += lhs.toString() + " = " + rhs1.toString() + " " + operation + " " + rhs2.toString();
                break;
                
            case InstructionType::ARRAY_ASSIGNMENT:
                result += lhs.toString() + " = " + rhs1.toString();
                break;
                
            case InstructionType::FUNCTION_CALL:
                result += lhs.toString() + " = call " + rhs1.toString();
                if (!rhs2.name.empty()) {
                    result += ", " + rhs2.toString();
                }
                break;
                
            case InstructionType::PARAM:
                result += "param";
                for (const auto& param : params) {
                    result += " " + param.toString();
                }
                break;
                
            default:
                return indentation + originalLine; // Fallback to original for unhandled types
        }
        
        return result;
    }
};

// Basic Block structure
struct BasicBlock {
    int id;
    std::vector<Instruction> instructions;
    std::set<int> predecessors;
    std::set<int> successors;
    
    BasicBlock() : id(-1) {}
};

// IR Optimizer class
class IROptimizer {
private:
    std::vector<std::string> irLines;
    std::vector<BasicBlock> basicBlocks;
    std::unordered_map<std::string, std::string> constantMap; // For constant propagation
    std::set<std::string> usedTemporaries; // Track which temporaries are actually used
    
    // Helper methods
    OperandType identifyOperandType(const std::string& operand);
    Operand parseOperand(const std::string& operandStr);
    InstructionType identifyInstructionType(const std::string& line);
    Instruction parseInstruction(const std::string& line, int lineIndex);
    void constructBasicBlocks();
    
    // Evaluation methods for constant folding
    std::string evaluateArithmeticExpression(const std::string& op, const std::string& val1, const std::string& val2);
    
    // Optimization passes
    void analyzeUsedTemporaries();
    void performConstantPropagation();
    void eliminateDeadCode();
    
public:
    IROptimizer(const std::string& irCode);
    // Add this declaration at the end of the file, outside the class definition
void optimizeIRFromFile(const std::string& inputFile, const std::string& outputFile);
    // Run all optimization passes
    void optimize();
    
    // User function for custom optimizations
    void userOptimizationPass();
    
    // Output the optimized IR code
    std::string getOptimizedIRCode() const;
    
    // Save to file
    void saveToFile(const std::string& filename) const;
};

#endif