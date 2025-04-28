#ifndef REGISTER_DESCRIPTOR_HPP
#define REGISTER_DESCRIPTOR_HPP
#include <map>
#include <string>
#include <vector>
#include <set>

class RegisterDescriptor {
private:
    // List of available registers (in order of preference)
    std::vector<std::string> availableRegisters;
    
    // List of available floating point registers (in order of preference)
    std::vector<std::string> availableFloatRegisters;
   
    // Sets of related registers (different sizes of the same register)
    std::map<std::string, std::set<std::string>> relatedRegisters;
   
    // Register size based on type
    std::map<std::string, std::string> typeSizeMap;

public:
    // Maps register name to variables/temporaries it holds (allows multiples)
    std::map<std::string, std::vector<std::string>> registerContent;
    RegisterDescriptor();
    //check if the argument is a register or not
    bool isreg(const std::string& arg);
    // Check if a register is free
    bool isRegisterFree(const std::string& regName) const;
   
    // Get content of a register
    std::vector<std::string> getRegisterContent(const std::string& regName) const;
   
    // Allocate a register to a variable/temporary
    bool allocateRegister(const std::string& regName, const std::string& content);
   
    // Free a register
    void freeRegister(const std::string& regName);

    // Spill a register and return its contents
    std::vector<std::string> spillRegister(const std::vector<std::string>& protectedRegisters,const std::string& type);
   
    // Get an available register for a type
    std::string getAvailableRegister(const std::string& type);
   
    // Check if related registers are in use
    bool areRelatedRegistersInUse(const std::string& regName) const;
   
    // Get appropriate register size for a type
    std::string getRegisterForType(const std::string& regBase, const std::string& type) const;
   
    // Print the register descriptor
    void printDescriptor() const;
   
    // Get all registers containing a specific variable/temporary
    std::vector<std::string> getRegistersForContent(const std::string& content) const;
   
    // Clear the descriptor
    void clear();

    // Check if register is a floating point register
    bool isFloatRegister(const std::string& regName) const;
};
#endif // REGISTER_DESCRIPTOR_HPP
