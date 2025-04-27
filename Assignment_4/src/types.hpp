#pragma once

#include <iostream>
#include <set>
#include <string>
#include <unordered_map>
#include <map>
#include <regex>

class TypeSet {
private:
    std::set<std::string> types;
    std::unordered_map<std::string, std::string> typedefMap;
    std::map<std::string, std::vector<std::string>> functionStaticVars;

    // Helper to remove trailing '*' characters
    std::string getBaseType(const std::string& typeName) const;

public:
    // Initialize with all basic C types
    TypeSet() {
        // Primitive types
        types = {
            "VOID",
            "CHAR", "UNSIGNED CHAR",
            "SHORT", "UNSIGNED SHORT",
            "INT", "UNSIGNED INT",
            "LONG", "UNSIGNED LONG",
            "FLOAT", "DOUBLE",
            "BOOL"
        };
    }

    // Add user-defined types
    void addStruct(const std::string& name);
    void addUnion(const std::string& name);
    void addEnum(const std::string& name);
    void addClass(const std::string& name);

    // Add typedef alias
    void addTypedef(const std::string& alias, const std::string& originalType);
    // Getter for the Typedef mapping
    std::string getTypedef(const std::string& typedefName);
    // Check if a type exists
    bool contains(const std::string& typeName) const;
    // Check if the type declaration contains a pointer
    bool hasPointer(const std::string& typeName) const;
    // Check if the type has less than one 'CONST'
    bool isValidConstDeclaration(const std::string& typeName) const;
    // Check if the type has exactly one 'CONST'
    bool oneConst(const std::string& typeName) const;
    // To remove CONST from the declaration
    std::string removeConstFromDeclaration(const std::string& typeDecl) const;
    // To remove static from the declaration
    std::string removeStaticFromDeclaration(const std::string& typeDecl) const;
    // To count storage Specifiers
    int storagespecifercount(const std::string& typeName) const;
    // To count CONST
    int constCount(const std::string& typeName) const;
    // To check for constant pointer
    bool hasConstAfterStar(const std::string& typeDecl);
    // TO give a string where const and static is removed
    std::string prepareString(const std::string& type);

    // Functions to handle storing of static variable names;
    // Add a static variable to a function's list
    void addStaticVariable(const std::string& functionName, const std::string& varName);
    // Get the list of static variables for a function
    std::vector<std::string> getStaticVariables(const std::string& functionName) const;

    // Print Functions
    // Print all static variables for all functions
    void printStaticVariables() const;
    // Print all types in the set
    void printAllTypes() const;
    // Print all typedef mappings
    void printTypedefs() const;
};
