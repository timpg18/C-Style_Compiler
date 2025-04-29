#include "BasicBlockConstructor.hpp"
#include <iostream>
#include <fstream>

using namespace std;

vector<BasicBlockConstructor::Instruction> BasicBlockConstructor::parseInstructions(const string& code) {
    vector<Instruction> instructions;
    istringstream codeStream(code);
    string line;
    
    regex functionLabelRegex(R"(^\s*label\s+(\w+):\s*$)");
    regex blockLabelRegex(R"(^\s*label\s+(@L\d+):\s*$)");
    regex ifGotoRegex(R"(\s*if\s+.*\s+goto\s+(@L\d+)\s*)");
    regex gotoRegex(R"(\s*goto\s+(@L\d+)\s*)");
    
    int index = 0;
    string currentLabel = "";
    
    while (getline(codeStream, line)) {
        Instruction instr;
        instr.index = index++;
        instr.text = line;
        instr.isLabel = false;
        instr.isBranch = false;
        instr.isTarget = false;
        instr.isIfGoto = false;
        instr.isGoto = false;
        instr.isFunctionLabel = false;
        instr.operation = "";  // Initialize new fields
        
        smatch matches;
        if (regex_match(line, matches, functionLabelRegex)) {
            instr.isLabel = true;
            instr.isFunctionLabel = true;
            instr.labelName = matches[1].str();
            currentLabel = instr.labelName;
            instructions.push_back(instr);
            continue;
        }
        
        if (regex_match(line, matches, blockLabelRegex)) {
            instr.isLabel = true;
            instr.labelName = matches[1].str();
            currentLabel = instr.labelName;
            instructions.push_back(instr);
            continue;
        }
        
        if (!currentLabel.empty()) {
            instr.isTarget = true;
            currentLabel = "";
        }
        
        if (regex_match(line, matches, ifGotoRegex)) {
            instr.isBranch = true;
            instr.isIfGoto = true;
            instr.targetLabel = matches[1].str();
        }
        
        if (regex_match(line, matches, gotoRegex)) {
            instr.isBranch = true;
            instr.isGoto = true;
            instr.targetLabel = matches[1].str();
        }
        
        instructions.push_back(instr);
    }
    
    // Extract dependencies for each instruction
    
    
    return instructions;
}

void BasicBlockConstructor::markTargets(vector<Instruction>& instructions) {
    unordered_map<string, int> labelToIndex;
    for (int i = 0; i < instructions.size(); i++) {
        if (instructions[i].isLabel) {
            labelToIndex[instructions[i].labelName] = i + 1;
        }
    }
    
    for (const auto& instr : instructions) {
        if (instr.isBranch && !instr.targetLabel.empty()) {
            if (labelToIndex.count(instr.targetLabel)) {
                int targetIndex = labelToIndex[instr.targetLabel];
                if (targetIndex < instructions.size()) {
                    instructions[targetIndex].isTarget = true;
                }
            }
        }
    }
    
    for (int i = 0; i < instructions.size(); i++) {
        if (!instructions[i].isLabel) {
            instructions[i].isTarget = true;
            break;
        }
    }
}

vector<BasicBlockConstructor::BasicBlock> BasicBlockConstructor::constructBlocks(vector<Instruction>& instructions) {
    vector<BasicBlock> blocks;
    int blockId = 0;
    int currentFunction = -1;
    
    BasicBlock currentBlock;
    currentBlock.id = blockId++;
    currentBlock.functionId = -1;
    
    for (int i = 0; i < instructions.size(); i++) {
        Instruction& instr = instructions[i];
        
        if (instr.isFunctionLabel) {
            if (!currentBlock.instructions.empty()) {
                blocks.push_back(currentBlock);
            }
            
            currentFunction++;
            currentBlock = BasicBlock();
            currentBlock.id = blockId++;
            currentBlock.functionId = currentFunction;
            currentBlock.instructions.push_back(instr);
            continue;
        }
        
        if (instr.isLabel) {
            if (!currentBlock.instructions.empty()) {
                blocks.push_back(currentBlock);
                currentBlock = BasicBlock();
                currentBlock.id = blockId++;
                currentBlock.functionId = currentFunction;
            }
            currentBlock.instructions.push_back(instr);
            continue;
        }
        
        if (instr.isTarget && !currentBlock.instructions.empty() && 
            !(currentBlock.instructions.size() == 1 && currentBlock.instructions[0].isLabel)) {
            blocks.push_back(currentBlock);
            currentBlock = BasicBlock();
            currentBlock.id = blockId++;
            currentBlock.functionId = currentFunction;
        }
        
        currentBlock.instructions.push_back(instr);
        
        if (instr.isBranch) {
            bool isIfGotoFollowedByGoto = false;
            
            if (instr.isIfGoto && i + 1 < instructions.size()) {
                Instruction& nextInstr = instructions[i + 1];
                if (nextInstr.isGoto && !nextInstr.isLabel) {
                    isIfGotoFollowedByGoto = true;
                    i++;
                    currentBlock.instructions.push_back(nextInstr);
                }
            }
            
            if (!isIfGotoFollowedByGoto || (isIfGotoFollowedByGoto && i == instructions.size() - 1)) {
                blocks.push_back(currentBlock);
                currentBlock = BasicBlock();
                currentBlock.id = blockId++;
                currentBlock.functionId = currentFunction;
            }
        }
    }
    
    if (!currentBlock.instructions.empty()) {
        blocks.push_back(currentBlock);
    }
    
    return blocks;
}

void BasicBlockConstructor::buildCFG(vector<BasicBlock>& blocks, const vector<Instruction>& instructions) {
    unordered_map<string, int> labelToInstrIndex;
    unordered_map<int, int> instrIndexToBlockId;
    
    for (int i = 0; i < instructions.size(); i++) {
        if (instructions[i].isLabel) {
            labelToInstrIndex[instructions[i].labelName] = i + 1;
        }
    }
    
    for (int blockId = 0; blockId < blocks.size(); blockId++) {
        for (const auto& instr : blocks[blockId].instructions) {
            instrIndexToBlockId[instr.index] = blockId;
        }
    }
    
    for (int blockId = 0; blockId < blocks.size(); blockId++) {
        BasicBlock& block = blocks[blockId];
        if (block.instructions.empty()) continue;
        
        bool hasIfGotoFollowedByGoto = false;
        string ifGotoTarget;
        string gotoTarget;
        
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
            int ifTargetInstrIndex = labelToInstrIndex[ifGotoTarget];
            if (instrIndexToBlockId.count(ifTargetInstrIndex)) {
                int ifTargetBlockId = instrIndexToBlockId[ifTargetInstrIndex];
                if (blocks[ifTargetBlockId].functionId == block.functionId) {
                    block.successors.insert(ifTargetBlockId);
                    blocks[ifTargetBlockId].predecessors.insert(blockId);
                }
            }
            
            int gotoTargetInstrIndex = labelToInstrIndex[gotoTarget];
            if (instrIndexToBlockId.count(gotoTargetInstrIndex)) {
                int gotoTargetBlockId = instrIndexToBlockId[gotoTargetInstrIndex];
                if (blocks[gotoTargetBlockId].functionId == block.functionId) {
                    block.successors.insert(gotoTargetBlockId);
                    blocks[gotoTargetBlockId].predecessors.insert(blockId);
                }
            }
        } else {
            const Instruction& lastInstr = block.instructions.back();
            
            if (lastInstr.isIfGoto) {
                int targetInstrIndex = labelToInstrIndex[lastInstr.targetLabel];
                if (instrIndexToBlockId.count(targetInstrIndex)) {
                    int targetBlockId = instrIndexToBlockId[targetInstrIndex];
                    if (blocks[targetBlockId].functionId == block.functionId) {
                        block.successors.insert(targetBlockId);
                        blocks[targetBlockId].predecessors.insert(blockId);
                    }
                }
                
                if (blockId + 1 < blocks.size() && blocks[blockId + 1].functionId == block.functionId) {
                    block.successors.insert(blockId + 1);
                    blocks[blockId + 1].predecessors.insert(blockId);
                }
            } else if (lastInstr.isGoto) {
                int targetInstrIndex = labelToInstrIndex[lastInstr.targetLabel];
                if (instrIndexToBlockId.count(targetInstrIndex)) {
                    int targetBlockId = instrIndexToBlockId[targetInstrIndex];
                    if (blocks[targetBlockId].functionId == block.functionId) {
                        block.successors.insert(targetBlockId);
                        blocks[targetBlockId].predecessors.insert(blockId);
                    }
                }
            } else {
                if (blockId + 1 < blocks.size() && blocks[blockId + 1].functionId == block.functionId) {
                    block.successors.insert(blockId + 1);
                    blocks[blockId + 1].predecessors.insert(blockId);
                }
            }
        }
    }
}

vector<BasicBlockConstructor::BasicBlock> BasicBlockConstructor::constructBasicBlocks(const string& code) {
    vector<Instruction> instructions = parseInstructions(code);
    markTargets(instructions);
    vector<BasicBlock> blocks = constructBlocks(instructions);
    buildCFG(blocks, instructions);
    return blocks;
}

void BasicBlockConstructor::printBasicBlocks(const vector<BasicBlock>& blocks) {
    for (const auto& block : blocks) {
        cout << "Basic Block " << block.id << ":" << endl;
        cout << "Predecessors: ";
        for (int predId : block.predecessors) {
            cout << predId << " ";
        }
        cout << endl;
        
        cout << "Successors: ";
        for (int succId : block.successors) {
            cout << succId << " ";
        }
        cout << endl;
        
        cout << "Instructions:" << endl;
        for (const auto& instr : block.instructions) {
            cout << "  " << instr.text << endl;
            
            // Print dependency information
            if (!instr.isLabel) {
                cout << "    Operation: " << instr.operation << endl;
                
                if (!instr.lhs.empty()) {
                    cout << "    LHS: ";
                    for (const auto& var : instr.lhs) {
                        cout << var << " ";
                    }
                    cout << endl;
                }
                
                if (!instr.rhs.empty()) {
                    cout << "    RHS: ";
                    for (const auto& var : instr.rhs) {
                        cout << var << " ";
                    }
                    cout << endl;
                }
            }
        }
        cout << endl;
    }
}