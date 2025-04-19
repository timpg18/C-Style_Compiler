#ifndef ADDRESS_ALLOCATION_TABLE_HPP
#define ADDRESS_ALLOCATION_TABLE_HPP

#include "symbol.hpp"
#include <string>
#include <set>
#include <vector>
#include <map>

struct VarTempInfo {
    std::string name;
    std::string type;
    std::string address;  // Address relative to RBP
    std::map<std::string, std::string> registerMap;  // Address descriptor
    //I am adding the indicator if X = A > B, mark X as jump
    //since it will follow if $X goto Y
    std::string relational_op = "";
    bool operator<(const VarTempInfo& other) const {
        return name < other.name;
    }
};

class AddressAllocationTable {
private:
    std::set<VarTempInfo> variables;
    std::set<VarTempInfo> temporaries;
    SymbolTable* symbolTable;

    std::string getVariableType(const std::string& varName) const;
    std::string getTempType(const std::string& tempName, const std::string& contextLine) const;
    void calculateAddresses(const std::string& irCode);

public:
    AddressAllocationTable() : symbolTable(nullptr) {}
    explicit AddressAllocationTable(const std::string& irCode, SymbolTable& st);
    
    void parseIRCode(const std::string& irCode);
    void setSymbolTable(SymbolTable& st);
    
    
    void addVariable(const std::string& varName);
    void addTemporary(const std::string& tempName, const std::string& contextLine);
    
    // New methods for managing address descriptors
    void setVariableAddress(const std::string& varName, const std::string& address);
    void setTemporaryAddress(const std::string& tempName, const std::string& address);
    std::string getVariableAddress(const std::string& varName) const;
    std::string getTemporaryAddress(const std::string& tempName) const;
    
    // Address descriptor register management
    void addRegisterToDescriptor(const std::string& varOrTempName, 
                                const std::string& regName,
                                const std::string& value);
    void removeRegisterFromDescriptor(const std::string& varOrTempName, 
                                     const std::string& regName);
    // to check if a specific register contains the var or temp
    bool isInRegister(const std::string& varOrTempName, const std::string& regName) const;
    // to check if it contains any register or not
    bool isEmpty(const std::string& varOrTempName) const;
    std::map<std::string, std::string> getRegisterDescriptor(const std::string& varOrTempName) const;
    void clearRegisterDescriptor(const std::string& varOrTempName);
    
    const std::set<VarTempInfo>& getVariables() const;
    const std::set<VarTempInfo>& getTemporaries() const;
    std::string getType(const std::string& Name) const ;
    void set_relop(const std::string& Name, std::string op);
    std::vector<VarTempInfo> getVariablesVector() const;
    std::vector<VarTempInfo> getTemporariesVector() const;
    
    void printTable() const;
    void clear();
};

#endif // ADDRESS_ALLOCATION_TABLE_HPP