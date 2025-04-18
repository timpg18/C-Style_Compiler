#include "RegisterDescriptor.hpp"
#include <iostream>
#include <algorithm>

RegisterDescriptor::RegisterDescriptor() {
    // Initialize available registers for x86-64
    availableRegisters = {
        "rax", "rcx", "rdx", "rsi", "rdi", 
        "r8", "r9", "r10", "r11"
    };
    
    std::set<std::string> all_reg = {
        "rax", "rcx", "rdx", "rsi", "rdi", 
        "r8", "r9", "r10", "r11", "eax", "ax", "ah", "al",
        "ecx", "cx", "ch", "cl",
        "edx", "dx", "dh", "dl",
        "esi", "si", "sil",
        "edi", "di", "dil",
        "r8d", "r8w", "r8b",
        "r9d", "r9w", "r9b",
        "r10d", "r10w", "r10b",
        "r11d", "r11w", "r11b"
    };

    // Initialize register content map with empty vectors
    for (const auto& reg : all_reg) {
        registerContent[reg] = std::vector<std::string>();
    }

    
    // Define related registers (different sizes of same register)
    relatedRegisters["rax"] = {"rax", "eax", "ax", "ah", "al"};
    relatedRegisters["rcx"] = {"rcx", "ecx", "cx", "ch", "cl"};
    relatedRegisters["rdx"] = {"rdx", "edx", "dx", "dh", "dl"};
    relatedRegisters["rsi"] = {"rsi", "esi", "si", "sil"};
    relatedRegisters["rdi"] = {"rdi", "edi", "di", "dil"};
    relatedRegisters["r8"] = {"r8", "r8d", "r8w", "r8b"};
    relatedRegisters["r9"] = {"r9", "r9d", "r9w", "r9b"};
    relatedRegisters["r10"] = {"r10", "r10d", "r10w", "r10b"};
    relatedRegisters["r11"] = {"r11", "r11d", "r11w", "r11b"};
    
    // Define mapping from type to register size
    typeSizeMap["INT"] = "32";     // Using 64-bit for integers
    typeSizeMap["BOOL"] = "8";     // Using 8-bit for booleans
    typeSizeMap["CHAR"] = "8";     // Using 8-bit for chars
    typeSizeMap["FLOAT"] = "32";   // Using 64-bit for floats
    typeSizeMap["DOUBLE"] = "64";  // Using 64-bit for doubles
    typeSizeMap["SHORT"] = "16";   // Using 16-bit for shorts
    typeSizeMap["LONG"] = "64";    // Using 64-bit for longs
}
bool RegisterDescriptor::isreg(const std::string& arg){
    for(auto &it: relatedRegisters){
        for(auto &it2: it.second){
            if(it2 == arg){
                return true;
            }
        }
    }
    return false;
}
bool RegisterDescriptor::isRegisterFree(const std::string& regName) const {
    auto it = registerContent.find(regName);
    return (it != registerContent.end() && it->second.empty());
}

std::vector<std::string> RegisterDescriptor::getRegisterContent(const std::string& regName) const {
    auto it = registerContent.find(regName);
    return (it != registerContent.end()) ? it->second : std::vector<std::string>();
}

bool RegisterDescriptor::allocateRegister(const std::string& regName, const std::string& content) {
    //Check if this register or any related register is already in use
    if (areRelatedRegistersInUse(regName)) {
        //We can still add content to a register that's already in use
        auto it = registerContent.find(regName);
        if (it != registerContent.end()) {
            //Check if the content is already in the register to avoid duplicates
            if (std::find(it->second.begin(), it->second.end(), content) == it->second.end()) {
                it->second.push_back(content);
            }
            return true;
        }
        return false;
    }
    
    // Allocate the register
    auto it = registerContent.find(regName);
    if (it != registerContent.end()) {
        it->second.push_back(content);
        return true;
    }
    return false;
}

void RegisterDescriptor::freeRegister(const std::string& regName) {
    auto it = registerContent.find(regName);
    if (it != registerContent.end()) {
        it->second.clear();
    }
}

std::vector<std::string> RegisterDescriptor::spillRegister(const std::vector<std::string>& protectedRegisters) {
    // Find a register to spill (one that is not in the protected list)
    for (const auto& reg : availableRegisters) {
        // Skip if this register is protected
        if (std::find(protectedRegisters.begin(), protectedRegisters.end(), reg) != protectedRegisters.end()) {
            continue;
        }
        
        // Skip if this register is empty
        auto it = registerContent.find(reg);
        if (it == registerContent.end() || it->second.empty()) {
            continue;
        }
        
        // This register can be spilled
        std::vector<std::string> result;
        result.push_back(reg);  // First element is the register name
        
        // Add all variables/temporaries in the register
        result.insert(result.end(), it->second.begin(), it->second.end());
        
        // Clear the register after spilling
        it->second.clear();
        
        return result;
    }
    
    // If we get here, no suitable register was found to spill
    return std::vector<std::string>();
}

std::string RegisterDescriptor::getAvailableRegister(const std::string& type) {
    // Try to find a free register
    for (const auto& reg : availableRegisters) {
        if (isRegisterFree(reg) && !areRelatedRegistersInUse(reg)) {
            return getRegisterForType(reg, type);
        }
    }
    return ""; // No register available
}

bool RegisterDescriptor::areRelatedRegistersInUse(const std::string& regName) const {
    // Find the base register
    std::string baseReg = regName;
    for (const auto& relGroup : relatedRegisters) {
        if (relGroup.second.find(regName) != relGroup.second.end()) {
            baseReg = relGroup.first;
            break;
        }
    }
    
    // Check if any related register is in use
    auto relatedRegsIt = relatedRegisters.find(baseReg);
    if (relatedRegsIt != relatedRegisters.end()) {
        for (const auto& relReg : relatedRegsIt->second) {
            auto contentIt = registerContent.find(relReg);
            if (contentIt != registerContent.end() && !contentIt->second.empty()) {
                return true;
            }
        }
    }
    
    return false;
}

std::string RegisterDescriptor::getRegisterForType(const std::string& regBase, const std::string& type) const {
    // Get the size based on type
    auto sizeIt = typeSizeMap.find(type);
    std::string size = (sizeIt != typeSizeMap.end()) ? sizeIt->second : "64"; // Default to 64-bit
    
    // Find the base register group
    auto relatedRegsIt = relatedRegisters.find(regBase);
    if (relatedRegsIt == relatedRegisters.end()) {
        return regBase; // Default to the base register
    }
    
    // Select the appropriate register size
    if (size == "64") {
        // 64-bit register (rax, rcx, etc.)
        return regBase;
    } else if (size == "32") {
        // 32-bit register (eax, ecx, etc.)
        for (const auto& reg : relatedRegsIt->second) {
            if (reg.find('e') == 0 || reg.find('d', 1) != std::string::npos) {
                return reg;
            }
        }
    } else if (size == "16") {
        // 16-bit register (ax, cx, etc.)
        for (const auto& reg : relatedRegsIt->second) {
            if (reg.find('x') != std::string::npos && reg.length() <= 2 || 
                reg.find('w', 1) != std::string::npos) {
                return reg;
            }
        }
    } else if (size == "8") {
        // 8-bit register (al, cl, etc.)
        for (const auto& reg : relatedRegsIt->second) {
            if (reg.find('l') != std::string::npos) {
                return reg;
            }
        }
    }
    
    // Default to the base register if we couldn't find an appropriate size
    return regBase;
}

void RegisterDescriptor::printDescriptor() const {
    std::cout << "=== Register Descriptor ===\n";
    for (const auto& reg : availableRegisters) {
        std::cout << reg << " : [";
        const auto& contents = registerContent.at(reg);
        for (size_t i = 0; i < contents.size(); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << contents[i];
        }
        std::cout << "]\n";
    }
}

std::vector<std::string> RegisterDescriptor::getRegistersForContent(const std::string& content) const {
    std::vector<std::string> result;
    for (const auto& regPair : registerContent) {
        const auto& contents = regPair.second;
        if (std::find(contents.begin(), contents.end(), content) != contents.end()) {
            result.push_back(regPair.first);
        }
    }
    return result;
}

void RegisterDescriptor::clear() {
    for (auto& reg : registerContent) {
        reg.second.clear();
    }
}