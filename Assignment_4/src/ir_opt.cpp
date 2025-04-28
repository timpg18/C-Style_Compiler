#include "ir_opt.hpp"

IROptimizer::IROptimizer(const std::string& irCode) {
    // Split IR code into lines
    std::istringstream stream(irCode);
    std::string line;
    while (std::getline(stream, line)) {
        irLines.push_back(line);
    }
    
    // Construct basic blocks
    constructBasicBlocks();
}

void IROptimizer::optimize() {
    // First analyze which temporaries are used
    analyzeUsedTemporaries();
    
    // Run optimization passes
    performConstantPropagation();
    eliminateDeadCode();
    eliminateTemporaries(); // New optimization pass
    
    // Call user function for additional optimizations
    userOptimizationPass();
}

void IROptimizer::userOptimizationPass() {
    // This is where users can add their own optimizations
    // We've already implemented constant propagation as an example
    
    // Additional optimizations can be added here:
    // - Common subexpression elimination
    // - Loop optimizations
    // - etc.
}

OperandType IROptimizer::identifyOperandType(const std::string& operand) {
    if (operand.empty()) {
        return OperandType::UNKNOWN;
    }
    
    // Check if it's a temporary ($n)
    if (operand[0] == '$' && operand.find_first_not_of("$0123456789") == std::string::npos) {
        return OperandType::TEMPORARY;
    }
    
    // Check if it's a constant (numeric)
    if ((operand[0] >= '0' && operand[0] <= '9') || 
        (operand[0] == '-' && operand.length() > 1 && operand[1] >= '0' && operand[1] <= '9')) {
        // Check if it contains only digits (and possibly a decimal point or minus sign)
        if (operand.find_first_not_of("-0123456789.") == std::string::npos) {
            return OperandType::CONSTANT;
        }
    }
    
    // Check if it's a string literal
    if (operand[0] == '"' && operand[operand.length() - 1] == '"') {
        return OperandType::STRING_LITERAL;
    }
    
    // Check if it's a character literal
    if (operand[0] == '\'' && operand[operand.length() - 1] == '\'') {
        return OperandType::CHAR_LITERAL;
    }
    
    // Check if it's a param
    if (operand == "param") {
        return OperandType::PARAM;
    }
    
    // Check if it's a function call
    if (operand == "call") {
        return OperandType::FUNCTION_CALL;
    }
    
    // Check if it's a label
    if (operand == "label") {
        return OperandType::LABEL;
    }
    
    // Check if it's an array access
    size_t bracketPos = operand.find('[');
    if (bracketPos != std::string::npos && operand.find(']', bracketPos) != std::string::npos) {
        return OperandType::ARRAY_ACCESS;
    }
    
    // Default to variable
    return OperandType::VARIABLE;
}

Operand IROptimizer::parseOperand(const std::string& operandStr) {
    Operand operand;
    operand.name = operandStr;
    operand.type = identifyOperandType(operandStr);
    
    if (operand.type == OperandType::ARRAY_ACCESS) {
        // Parse array access: extract base array and index expression
        size_t bracketPos = operandStr.find('[');
        size_t closeBracketPos = operandStr.find(']');
        
        operand.baseArray = operandStr.substr(0, bracketPos);
        operand.indexExpr = operandStr.substr(bracketPos + 1, closeBracketPos - bracketPos - 1);
    } else if (operand.type == OperandType::CONSTANT) {
        operand.value = operandStr;
    } else if (operand.type == OperandType::CHAR_LITERAL) {
        // Extract character value
        if (operandStr.length() == 3) {  // 'a'
            operand.value = std::to_string(static_cast<int>(operandStr[1]));
        } else if (operandStr.length() == 4 && operandStr[1] == '\\') {  // '\n'
            char escapedChar = operandStr[2];
            int value;
            switch (escapedChar) {
                case 'n': value = 10; break;  // Newline
                case 't': value = 9; break;   // Tab
                case 'r': value = 13; break;  // Carriage return
                case '0': value = 0; break;   // Null
                case '\\': value = 92; break; // Backslash
                case '\'': value = 39; break; // Single quote
                case '\"': value = 34; break; // Double quote
                default: value = static_cast<int>(escapedChar);
            }
            operand.value = std::to_string(value);
        }
    }
    
    return operand;
}

InstructionType IROptimizer::identifyInstructionType(const std::string& line) {
    std::string trimmedLine = line;
    // Remove leading whitespace
    trimmedLine.erase(0, trimmedLine.find_first_not_of(" \t"));
    
    if (trimmedLine.find("label") == 0) {
        return InstructionType::LABEL;
    } else if (trimmedLine.find("func_begin") != std::string::npos) {
        return InstructionType::FUNCTION_BEGIN;
    } else if (trimmedLine.find("func_end") != std::string::npos) {
        return InstructionType::FUNCTION_END;
    } else if (trimmedLine.find("param") == 0) {
        return InstructionType::PARAM;
    } else if (trimmedLine.find("call") != std::string::npos) {
        return InstructionType::FUNCTION_CALL;
    } else if (trimmedLine.find("=") != std::string::npos) {
        // Check for array assignment
        std::string lhs = trimmedLine.substr(0, trimmedLine.find("="));
        lhs.erase(lhs.find_last_not_of(" \t") + 1); // Trim trailing spaces
        
        if (lhs.find("[") != std::string::npos) {
            return InstructionType::ARRAY_ASSIGNMENT;
        }
        
        std::string rhs = trimmedLine.substr(trimmedLine.find("=") + 1);
        rhs.erase(0, rhs.find_first_not_of(" \t")); // Trim leading spaces
        
        // Check for arithmetic operations
        if (rhs.find("+") != std::string::npos || rhs.find("-") != std::string::npos ||
            rhs.find("*") != std::string::npos || rhs.find("/") != std::string::npos) {
            return InstructionType::ARITHMETIC;
        } else if (rhs.find("<") != std::string::npos || rhs.find(">") != std::string::npos ||
                  rhs.find("==") != std::string::npos || rhs.find("!=") != std::string::npos) {
            return InstructionType::COMPARISON;
        }
        
        return InstructionType::ASSIGNMENT;
    }
    
    return InstructionType::UNKNOWN;
}

Instruction IROptimizer::parseInstruction(const std::string& line, int lineIndex) {
    Instruction inst;
    
    // Extract indentation
    size_t firstNonSpace = line.find_first_not_of(" \t");
    if (firstNonSpace != std::string::npos) {
        inst.indentation = line.substr(0, firstNonSpace);
    }
    
    // Trim line for parsing
    std::string trimmedLine = line;
    if (firstNonSpace != std::string::npos) {
        trimmedLine = line.substr(firstNonSpace);
    }
    
    inst.originalLine = trimmedLine;
    inst.lineIndex = lineIndex;
    inst.type = identifyInstructionType(line);
    
    std::istringstream iss(trimmedLine);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    
    if (tokens.empty()) {
        return inst;
    }
    
    switch (inst.type) {
        case InstructionType::LABEL:
            if (tokens.size() >= 2) {
                inst.lhs = parseOperand(tokens[1]);
            }
            break;
            
        case InstructionType::FUNCTION_BEGIN:
        case InstructionType::FUNCTION_END:
            if (tokens.size() >= 2) {
                inst.lhs = parseOperand(tokens[1]);
            }
            break;
            
        case InstructionType::PARAM:
            if (tokens.size() >= 2) {
                Operand param = parseOperand(tokens[1]);
                param.type = OperandType::PARAM;
                inst.params.push_back(param);
                
                // Add additional parameters if present
                for (size_t i = 2; i < tokens.size(); i++) {
                    Operand additionalParam = parseOperand(tokens[i]);
                    additionalParam.type = OperandType::PARAM;
                    inst.params.push_back(additionalParam);
                }
            }
            break;
            
        case InstructionType::FUNCTION_CALL:
            if (tokens.size() >= 4 && tokens[1] == "=") {
                inst.lhs = parseOperand(tokens[0]);
                // Function name is after "call"
                inst.rhs1 = parseOperand(tokens[3]);
                if (tokens.size() >= 5) {
                    // Number of parameters
                    inst.rhs2 = parseOperand(tokens[4]);
                }
            }
            break;
            
        case InstructionType::ARITHMETIC:
            if (tokens.size() >= 5 && tokens[1] == "=") {
                inst.lhs = parseOperand(tokens[0]);
                inst.rhs1 = parseOperand(tokens[2]);
                inst.operation = tokens[3];
                inst.rhs2 = parseOperand(tokens[4]);
            }
            break;
            
        case InstructionType::ASSIGNMENT:
            if (tokens.size() >= 3 && tokens[1] == "=") {
                inst.lhs = parseOperand(tokens[0]);
                inst.rhs1 = parseOperand(tokens[2]);
            }
            break;
            
        case InstructionType::ARRAY_ASSIGNMENT:
            if (tokens.size() >= 3 && tokens[1] == "=") {
                inst.lhs = parseOperand(tokens[0]);
                inst.rhs1 = parseOperand(tokens[2]);
            }
            break;
            
        case InstructionType::COMPARISON:
            if (tokens.size() >= 5 && tokens[1] == "=") {
                inst.lhs = parseOperand(tokens[0]);
                inst.rhs1 = parseOperand(tokens[2]);
                inst.operation = tokens[3];
                inst.rhs2 = parseOperand(tokens[4]);
            }
            break;
            
        default:
            break;
    }
    
    return inst;
}

void IROptimizer::constructBasicBlocks() {
    // Create a basic block for each function
    int blockId = 0;
    BasicBlock* currentBlock = nullptr;
    
    for (size_t i = 0; i < irLines.size(); i++) {
        const std::string& line = irLines[i];
        if (line.empty()) {
            continue;
        }
        
        Instruction inst = parseInstruction(line, i);
        
        // If this is a label or function begin, start a new basic block
        if (inst.type == InstructionType::LABEL || inst.type == InstructionType::FUNCTION_BEGIN) {
            // Add current block if it exists
            if (currentBlock) {
                basicBlocks.push_back(*currentBlock);
                delete currentBlock;
            }
            
            // Create new block
            currentBlock = new BasicBlock();
            currentBlock->id = blockId++;
        }
        
        // If there's no current block, create one
        if (!currentBlock) {
            currentBlock = new BasicBlock();
            currentBlock->id = blockId++;
        }
        
        // Add instruction to current block
        currentBlock->instructions.push_back(inst);
        
        // If this is a function end, terminate the block
        if (inst.type == InstructionType::FUNCTION_END) {
            basicBlocks.push_back(*currentBlock);
            delete currentBlock;
            currentBlock = nullptr;
        }
    }
    
    // Add the last block if it exists
    if (currentBlock) {
        basicBlocks.push_back(*currentBlock);
        delete currentBlock;
    }
    
    // If no blocks were created, create a single default block
    if (basicBlocks.empty()) {
        BasicBlock block;
        block.id = 0;
        for (size_t i = 0; i < irLines.size(); i++) {
            const std::string& line = irLines[i];
            if (!line.empty()) {
                block.instructions.push_back(parseInstruction(line, i));
            }
        }
        basicBlocks.push_back(block);
    }
}

std::string IROptimizer::evaluateArithmeticExpression(const std::string& op, const std::string& val1, const std::string& val2) {
    try {
        int num1 = std::stoi(val1);
        int num2 = std::stoi(val2);
        
        if (op == "+") {
            return std::to_string(num1 + num2);
        } else if (op == "-") {
            return std::to_string(num1 - num2);
        } else if (op == "*") {
            return std::to_string(num1 * num2);
        } else if (op == "/" && num2 != 0) {
            return std::to_string(num1 / num2);
        }
    } catch (const std::exception& e) {
        // Handle non-integer values or other errors
        std::cerr << "Error evaluating arithmetic: " << e.what() << std::endl;
    }
    
    return ""; // Unable to evaluate
}

void IROptimizer::analyzeUsedTemporaries() {
    // Reset used temporaries set
    usedTemporaries.clear();
    finalUsedTemporaries.clear();
    
    // First pass: identify all temporaries that are used in non-temporary assignments
    for (const auto& block : basicBlocks) {
        for (const auto& inst : block.instructions) {
            // Check LHS if it's not a temporary, then we need to preserve any RHS temporaries
            if (!inst.lhs.isTemporary() && !inst.isDeleted) {
                // RHS temporaries are used in final outputs
                if (inst.rhs1.isTemporary()) {
                    finalUsedTemporaries.insert(inst.rhs1.name);
                }
                
                if (inst.rhs2.isTemporary()) {
                    finalUsedTemporaries.insert(inst.rhs2.name);
                }
                
                // Check array index expressions
                if (inst.rhs1.isArrayAccess() && inst.rhs1.indexExpr.find('$') != std::string::npos) {
                    finalUsedTemporaries.insert(inst.rhs1.indexExpr);
                }
                
                if (inst.lhs.isArrayAccess() && inst.lhs.indexExpr.find('$') != std::string::npos) {
                    finalUsedTemporaries.insert(inst.lhs.indexExpr);
                }
            }
            
            // Check all other usages for non-optimized dependencies
            // Check RHS operands for temporary usage
            if (inst.rhs1.isTemporary()) {
                usedTemporaries.insert(inst.rhs1.name);
            }
            
            if (inst.rhs2.isTemporary()) {
                usedTemporaries.insert(inst.rhs2.name);
            }
            
            // Check array index expressions
            if (inst.rhs1.isArrayAccess() && inst.rhs1.indexExpr.find('$') != std::string::npos) {
                usedTemporaries.insert(inst.rhs1.indexExpr);
            }
            
            if (inst.lhs.isArrayAccess() && inst.lhs.indexExpr.find('$') != std::string::npos) {
                usedTemporaries.insert(inst.lhs.indexExpr);
            }
            
            // Check parameters for temporaries
            for (const auto& param : inst.params) {
                if (param.isTemporary()) {
                    usedTemporaries.insert(param.name);
                    // Parameters are considered final uses
                    finalUsedTemporaries.insert(param.name);
                }
            }
        }
    }
}

void IROptimizer::performConstantPropagation() {
    // Track which temporaries can be eliminated
    temporaryValues.clear(); // Maps temporaries to their constant values
    
    // First pass: identify constant assignments and arithmetic operations
    for (auto& block : basicBlocks) {
        for (auto& inst : block.instructions) {
            // Skip deleted instructions
            if (inst.isDeleted) continue;
            
            // Handle constant assignments to temporaries
            if (inst.type == InstructionType::ASSIGNMENT && 
                inst.lhs.isTemporary() && 
                inst.rhs1.isConstant()) {
                
                temporaryValues[inst.lhs.name] = inst.rhs1.name;
            }
            
            // Handle arithmetic with constant operands
            if (inst.type == InstructionType::ARITHMETIC && 
                inst.lhs.isTemporary()) {
                
                // Replace known constant temporaries
                if (inst.rhs1.isTemporary() && temporaryValues.find(inst.rhs1.name) != temporaryValues.end()) {
                    inst.rhs1.name = temporaryValues[inst.rhs1.name];
                    inst.rhs1.type = OperandType::CONSTANT;
                }
                
                if (inst.rhs2.isTemporary() && temporaryValues.find(inst.rhs2.name) != temporaryValues.end()) {
                    inst.rhs2.name = temporaryValues[inst.rhs2.name];
                    inst.rhs2.type = OperandType::CONSTANT;
                }
                
                // If both operands are now constants, evaluate the expression
                if (inst.rhs1.isConstant() && inst.rhs2.isConstant()) {
                    std::string result = evaluateArithmeticExpression(
                        inst.operation, inst.rhs1.name, inst.rhs2.name);
                    
                    if (!result.empty()) {
                        // Store result for future propagation
                        temporaryValues[inst.lhs.name] = result;
                        
                        // Update the instruction to a simple assignment
                        inst.originalLine = inst.lhs.name + " = " + result;
                        inst.type = InstructionType::ASSIGNMENT;
                        inst.rhs1.name = result;
                        inst.rhs1.type = OperandType::CONSTANT;
                        inst.rhs2 = Operand();
                        inst.operation = "";
                    }
                }
            }
        }
    }
}

void IROptimizer::eliminateDeadCode() {
    // Mark assignments to unused temporaries for deletion
    for (auto& block : basicBlocks) {
        for (auto& inst : block.instructions) {
            // Skip instructions that are already deleted
            if (inst.isDeleted) continue;
            
            // Delete assignments to temporaries that aren't used elsewhere
            if ((inst.type == InstructionType::ASSIGNMENT || inst.type == InstructionType::ARITHMETIC) && 
                inst.lhs.isTemporary() && 
                usedTemporaries.find(inst.lhs.name) == usedTemporaries.end()) {
                
                // Check if this temporary is used for its side effects
                bool hasSideEffects = false;
                
                // Function calls have side effects
                if (inst.type == InstructionType::FUNCTION_CALL) {
                    hasSideEffects = true;
                }
                
                // If there are no side effects, delete the instruction
                if (!hasSideEffects) {
                    inst.isDeleted = true;
                }
            }
        }
    }
}

// New optimization function to eliminate temporaries completely
void IROptimizer::eliminateTemporaries() {
    // Second pass: propagate constants to all instructions
    for (auto& block : basicBlocks) {
        for (auto& inst : block.instructions) {
            // Skip deleted instructions
            if (inst.isDeleted) continue;
            
            // Replace temporary variables with their constant values in all expressions
            if (inst.rhs1.isTemporary() && temporaryValues.find(inst.rhs1.name) != temporaryValues.end()) {
                std::string oldName = inst.rhs1.name;
                inst.rhs1.name = temporaryValues[inst.rhs1.name];
                inst.rhs1.type = OperandType::CONSTANT;
                
                // Update the original line
                if (inst.type == InstructionType::ASSIGNMENT) {
                    inst.originalLine = inst.lhs.name + " = " + inst.rhs1.name;
                } else if (inst.type == InstructionType::ARITHMETIC) {
                    inst.originalLine = inst.lhs.name + " = " + inst.rhs1.name + " " + 
                                       inst.operation + " " + inst.rhs2.name;
                }
            }
            
            if (inst.rhs2.isTemporary() && temporaryValues.find(inst.rhs2.name) != temporaryValues.end()) {
                std::string oldName = inst.rhs2.name;
                inst.rhs2.name = temporaryValues[inst.rhs2.name];
                inst.rhs2.type = OperandType::CONSTANT;
                
                // Update the original line
                if (inst.type == InstructionType::ARITHMETIC) {
                    inst.originalLine = inst.lhs.name + " = " + inst.rhs1.name + " " + 
                                       inst.operation + " " + inst.rhs2.name;
                }
            }
            
            // Handle array accesses
            if (inst.lhs.isArrayAccess() && 
                !inst.lhs.indexExpr.empty() && 
                temporaryValues.find(inst.lhs.indexExpr) != temporaryValues.end()) {
                
                inst.lhs.indexExpr = temporaryValues[inst.lhs.indexExpr];
                
                // Update original line
                size_t openBracket = inst.originalLine.find('[');
                size_t closeBracket = inst.originalLine.find(']');
                if (openBracket != std::string::npos && closeBracket != std::string::npos) {
                    inst.originalLine = inst.originalLine.substr(0, openBracket + 1) + 
                                      inst.lhs.indexExpr + 
                                      inst.originalLine.substr(closeBracket);
                }
            }
            
            // Handle parameters
            for (auto& param : inst.params) {
                if (param.isTemporary() && temporaryValues.find(param.name) != temporaryValues.end()) {
                    std::string oldName = param.name;
                    param.name = temporaryValues[param.name];
                    param.type = OperandType::CONSTANT;
                    
                    // Update original line - replace the specific parameter
                    size_t pos = inst.originalLine.find(oldName);
                    if (pos != std::string::npos) {
                        inst.originalLine = inst.originalLine.substr(0, pos) + 
                                          param.name + 
                                          inst.originalLine.substr(pos + oldName.length());
                    }
                }
            }
            
            // Mark temporary assignments for deletion if they're not final uses
            if (inst.type == InstructionType::ASSIGNMENT && 
                inst.lhs.isTemporary() && 
                finalUsedTemporaries.find(inst.lhs.name) == finalUsedTemporaries.end()) {
                inst.isDeleted = true;
            }
        }
    }
}

std::string IROptimizer::getOptimizedIRCode() const {
    std::stringstream result;
    
    // Reconstruct the IR code from basic blocks
    for (const auto& block : basicBlocks) {
        for (const auto& inst : block.instructions) {
            if (!inst.isDeleted) {
                if (inst.originalLine.empty()) {
                    result << inst.generateOptimizedLine() << std::endl;
                } else {
                    result << inst.indentation << inst.originalLine << std::endl;
                }
            }
        }
    }
    
    return result.str();
}

void IROptimizer::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << getOptimizedIRCode();
        file.close();
        std::cout << "Optimized IR code saved to " << filename << std::endl;
    } else {
        std::cerr << "Error: Could not open file " << filename << " for writing." << std::endl;
    }
}
// Add this function to the IR_opt.cpp file, outside the class

// Helper function to optimize IR code from file to file
void optimizeIRFromFile(const std::string& inputFile, const std::string& outputFile) {
    // Read the IR code from the input file
    std::string irCode = readFileContents(inputFile);
    if (irCode.empty()) {
        std::cerr << "Error: No IR code to optimize." << std::endl;
        return;
    }
    
    // Create optimizer and perform optimizations
    IROptimizer optimizer(irCode);
    optimizer.optimize();
    
    // Save the optimized IR code to the output file
    optimizer.saveToFile(outputFile);
    
    std::cout << "Optimized IR saved to: " << outputFile << std::endl;
}