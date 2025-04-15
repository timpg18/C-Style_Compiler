#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <regex>

class BasicBlockConstructor {
private:
    struct Instruction {
        int index;              // Position in the instruction list
        std::string text;       // The actual instruction text
        bool isLabel;           // Is this a label definition
        std::string labelName;  // If it's a label, what's the label name
        bool isBranch;          // Is this a branch instruction (if/goto)
        bool isTarget;          // Is this a target of a branch
        bool isIfGoto;          // Specifically an if-goto instruction
        bool isGoto;            // Specifically a goto instruction
        std::string targetLabel; // Target label if it's a branch
    };

public:
    struct BasicBlock {
        int id;
        std::vector<Instruction> instructions;
        std::unordered_set<int> successors;  // IDs of successor blocks
        std::unordered_set<int> predecessors; // IDs of predecessor blocks
    };

private:
    std::vector<Instruction> parseInstructions(const std::string& code) {
        std::vector<Instruction> instructions;
        std::istringstream codeStream(code);
        std::string line;
        
        std::regex labelRegex("^\\s*label\\s+(@L\\d+|\\w+):\\s*$");
        std::regex ifGotoRegex("\\s*if\\s+.*\\s+goto\\s+(@L\\d+)\\s*");
        std::regex gotoRegex("\\s*goto\\s+(@L\\d+)\\s*");
        
        int index = 0;
        std::string currentLabel = "";
        
        while (std::getline(codeStream, line)) {
            Instruction instr;
            instr.index = index++;
            instr.text = line;
            instr.isLabel = false;
            instr.isBranch = false;
            instr.isTarget = false;
            instr.isIfGoto = false;
            instr.isGoto = false;
            
            std::smatch matches;
            // Check if this is a label definition
            if (std::regex_match(line, matches, labelRegex)) {
                instr.isLabel = true;
                instr.labelName = matches[1].str();
                currentLabel = instr.labelName;
                instructions.push_back(instr);
                continue;
            }
            
            // Check if this is a target (instruction after a label)
            if (!currentLabel.empty()) {
                instr.isTarget = true;
                currentLabel = ""; // Reset current label
            }
            
            // Check if this is an if-goto instruction
            if (std::regex_match(line, matches, ifGotoRegex)) {
                instr.isBranch = true;
                instr.isIfGoto = true;
                instr.targetLabel = matches[1].str();
            }
            
            // Check if this is a goto instruction
            if (std::regex_match(line, matches, gotoRegex)) {
                instr.isBranch = true;
                instr.isGoto = true;
                instr.targetLabel = matches[1].str();
            }
            
            instructions.push_back(instr);
        }
        
        return instructions;
    }

    // Mark targets based on branch instructions
    void markTargets(std::vector<Instruction>& instructions) {
        // First create a map of label names to instruction indices
        std::unordered_map<std::string, int> labelToIndex;
        for (int i = 0; i < instructions.size(); i++) {
            if (instructions[i].isLabel) {
                labelToIndex[instructions[i].labelName] = i + 1; // Target is the instruction after the label
            }
        }
        
        // Mark target instructions based on branches
        for (const auto& instr : instructions) {
            if (instr.isBranch && !instr.targetLabel.empty()) {
                if (labelToIndex.find(instr.targetLabel) != labelToIndex.end()) {
                    int targetIndex = labelToIndex[instr.targetLabel];
                    if (targetIndex < instructions.size()) {
                        instructions[targetIndex].isTarget = true;
                    }
                }
            }
        }
        
        // Also mark the first real instruction as a target (entry point)
        for (int i = 0; i < instructions.size(); i++) {
            if (!instructions[i].isLabel) {
                instructions[i].isTarget = true;
                break;
            }
        }
    }

    std::vector<BasicBlock> constructBlocks(std::vector<Instruction>& instructions) {
        std::vector<BasicBlock> blocks;
        int blockId = 0;
        
        // Create a new block starting with the first instruction
        BasicBlock currentBlock;
        currentBlock.id = blockId++;
        
        for (int i = 0; i < instructions.size(); i++) {
            Instruction& instr = instructions[i];
            
            // Skip label definitions for block creation purposes
            if (instr.isLabel) {
                continue;
            }
            
            // If this is a target and we already have instructions in the current block
            // (and not at the beginning of a new block), finalize the current block and start a new one
            if (instr.isTarget && !currentBlock.instructions.empty()) {
                blocks.push_back(currentBlock);
                currentBlock = BasicBlock();
                currentBlock.id = blockId++;
            }
            
            // Add this instruction to the current block
            currentBlock.instructions.push_back(instr);
            
            // If this is a branch instruction, finalize the block
            // Special case: if this is an if-goto followed by a goto, keep them in the same block
            if (instr.isBranch) {
                bool isIfGotoFollowedByGoto = false;
                
                if (instr.isIfGoto && i + 1 < instructions.size()) {
                    Instruction& nextInstr = instructions[i + 1];
                    if (nextInstr.isGoto && !nextInstr.isLabel) {
                        isIfGotoFollowedByGoto = true;
                        // Add the next instruction (goto) to the current block
                        i++;
                        currentBlock.instructions.push_back(nextInstr);
                    }
                }
                
                // Finalize the block if not the special case or if we just handled the special case
                if (!isIfGotoFollowedByGoto || (isIfGotoFollowedByGoto && i == instructions.size() - 1)) {
                    blocks.push_back(currentBlock);
                    currentBlock = BasicBlock();
                    currentBlock.id = blockId++;
                }
            }
        }
        
        // Add the last block if it has any instructions
        if (!currentBlock.instructions.empty()) {
            blocks.push_back(currentBlock);
        }
        
        return blocks;
    }

    void buildCFG(std::vector<BasicBlock>& blocks, const std::vector<Instruction>& instructions) {
        // Create maps for quick lookups
        std::unordered_map<std::string, int> labelToInstrIndex;
        std::unordered_map<int, int> instrIndexToBlockId;
        
        // Map labels to instruction indices
        for (int i = 0; i < instructions.size(); i++) {
            if (instructions[i].isLabel) {
                labelToInstrIndex[instructions[i].labelName] = i + 1; // Target is the instruction after the label
            }
        }
        
        // Map instruction indices to block IDs
        for (int blockId = 0; blockId < blocks.size(); blockId++) {
            for (const auto& instr : blocks[blockId].instructions) {
                instrIndexToBlockId[instr.index] = blockId;
            }
        }
        
        // Build CFG edges
        for (int blockId = 0; blockId < blocks.size(); blockId++) {
            BasicBlock& block = blocks[blockId];
            if (block.instructions.empty()) continue;
            
            // Check if this block contains the special case of if-goto followed by goto
            bool hasIfGotoFollowedByGoto = false;
            std::string ifGotoTarget;
            std::string gotoTarget;
            
            if (block.instructions.size() >= 2) {
                const Instruction& secondLast = block.instructions[block.instructions.size() - 2];
                const Instruction& last = block.instructions.back();
                
                if (secondLast.isIfGoto && last.isGoto) {
                    hasIfGotoFollowedByGoto = true;
                    ifGotoTarget = secondLast.targetLabel;
                    gotoTarget = last.targetLabel;
                }
            }
            
            if (hasIfGotoFollowedByGoto) {
                // Handle the if-goto branch
                int ifTargetInstrIndex = labelToInstrIndex[ifGotoTarget];
                if (instrIndexToBlockId.find(ifTargetInstrIndex) != instrIndexToBlockId.end()) {
                    int ifTargetBlockId = instrIndexToBlockId[ifTargetInstrIndex];
                    block.successors.insert(ifTargetBlockId);
                    blocks[ifTargetBlockId].predecessors.insert(blockId);
                }
                
                // Handle the goto branch
                int gotoTargetInstrIndex = labelToInstrIndex[gotoTarget];
                if (instrIndexToBlockId.find(gotoTargetInstrIndex) != instrIndexToBlockId.end()) {
                    int gotoTargetBlockId = instrIndexToBlockId[gotoTargetInstrIndex];
                    block.successors.insert(gotoTargetBlockId);
                    blocks[gotoTargetBlockId].predecessors.insert(blockId);
                }
            } else {
                // Handle standard cases
                const Instruction& lastInstr = block.instructions.back();
                
                if (lastInstr.isIfGoto) {
                    // For if-goto, add edges to the target and the fall-through
                    int targetInstrIndex = labelToInstrIndex[lastInstr.targetLabel];
                    if (instrIndexToBlockId.find(targetInstrIndex) != instrIndexToBlockId.end()) {
                        int targetBlockId = instrIndexToBlockId[targetInstrIndex];
                        block.successors.insert(targetBlockId);
                        blocks[targetBlockId].predecessors.insert(blockId);
                    }
                    
                    // Add fall-through edge
                    if (blockId + 1 < blocks.size()) {
                        block.successors.insert(blockId + 1);
                        blocks[blockId + 1].predecessors.insert(blockId);
                    }
                } else if (lastInstr.isGoto) {
                    // For goto, just add an edge to the target
                    int targetInstrIndex = labelToInstrIndex[lastInstr.targetLabel];
                    if (instrIndexToBlockId.find(targetInstrIndex) != instrIndexToBlockId.end()) {
                        int targetBlockId = instrIndexToBlockId[targetInstrIndex];
                        block.successors.insert(targetBlockId);
                        blocks[targetBlockId].predecessors.insert(blockId);
                    }
                } else {
                    // For non-branch instructions, add a fall-through edge
                    if (blockId + 1 < blocks.size()) {
                        block.successors.insert(blockId + 1);
                        blocks[blockId + 1].predecessors.insert(blockId);
                    }
                }
            }
        }
    }

public:
    std::vector<BasicBlock> constructBasicBlocksFromFile(const std::string& filename) {
        // Read the file content
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return {};
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string code = buffer.str();
        file.close();
        
        return constructBasicBlocks(code);
    }

    std::vector<BasicBlock> constructBasicBlocks(const std::string& code) {
        // Parse the code into instructions
        std::vector<Instruction> instructions = parseInstructions(code);
        
        // Mark target instructions based on branch instructions
        markTargets(instructions);
        
        // Construct basic blocks
        std::vector<BasicBlock> blocks = constructBlocks(instructions);
        
        // Build control flow graph
        buildCFG(blocks, instructions);
        
        return blocks;
    }

    void printBasicBlocks(const std::vector<BasicBlock>& blocks) {
        for (const auto& block : blocks) {
            std::cout << "Basic Block " << block.id << ":" << std::endl;
            std::cout << "Predecessors: ";
            for (int predId : block.predecessors) {
                std::cout << predId << " ";
            }
            std::cout << std::endl;
            
            std::cout << "Successors: ";
            for (int succId : block.successors) {
                std::cout << succId << " ";
            }
            std::cout << std::endl;
            
            std::cout << "Instructions:" << std::endl;
            for (const auto& instr : block.instructions) {
                std::cout << "  " << instr.text << std::endl;
            }
            std::cout << std::endl;
        }
    }
};

// Main function that reads from a file
int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <3ac_file>" << std::endl;
        return 1;
    }
    
    std::string filename = argv[1];
    
    BasicBlockConstructor constructor;
    std::vector<BasicBlockConstructor::BasicBlock> blocks = constructor.constructBasicBlocksFromFile(filename);
    
    if (blocks.empty()) {
        std::cerr << "Failed to construct basic blocks or no blocks found." << std::endl;
        return 1;
    }
    
    constructor.printBasicBlocks(blocks);
    
    return 0;
}