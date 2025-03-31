#ifndef CTYPESET_H
#define CTYPESET_H

#include <iostream>
#include <set>
#include <string>
#include <unordered_map>

class TypeSet {
private:
    std::set<std::string> types;
    std::unordered_map<std::string, std::string> typedefMap;
    // Helper to remove trailing '*' characters
    std::string getBaseType(const std::string& typeName) const {
        std::string base = typeName;
        
        // Step 1: Remove trailing '*' characters
        while (!base.empty() && base.back() == '*') {
            base.pop_back();
        }
        
        // Step 2: Split into tokens and remove 'CONST'
        std::vector<std::string> tokens;
        std::stringstream ss(base);
        std::string token;
        while (ss >> token) {
            if (token != "CONST") { // Case-sensitive check
                tokens.push_back(token);
            }
        }
        
        // Step 3: Rebuild the base type string
        base.clear();
        for (size_t i = 0; i < tokens.size(); ++i) {
            if (i > 0) base += " ";
            base += tokens[i];
        }
        
        return base;
    }

public:
    // Initialize with all basic C types
    TypeSet() {
        // Primitive types
        types = {
            "VOID",
            "CHAR", "SIGNED CHAR", "UNSIGNED CHAR",
            "SHORT", "UNSIGNED SHORT", "SHORT INT", "SIGNED SHORT INT" , "UNSIGNED SHORT INT",
            "INT", "UNSIGNED INT",
            "LONG", "UNSIGNED LONG", "LONG INT", "SIGNED LONG INT" ,"UNSIGNED LONG INT",
            "LONG LONG", "UNSIGNED LONG LONG", "SIGNED LONG LONG",
            "LONG LONG INT", "UNSIGNED LONG LONG INT", "SIGNED LONG LONG INT",
            "FLOAT", "DOUBLE", "LONG DOUBLE",
            "BOOL","auto"
        };
    }

    // Add user-defined types
    void addStruct(const std::string& name) { types.insert("struct " + name); }
    void addUnion(const std::string& name) { types.insert("union " + name); }
    void addEnum(const std::string& name) { types.insert("enum " + name); }
    void addClass(const std::string& name) { types.insert("class " + name); }

    // Add typedef alias
    void addTypedef(const std::string& alias, const std::string& originalType) {
        std::string base = originalType;
        std::stringstream ss(base);
        std::string token;
        std::vector<std::string> tokens;
        while (ss >> token) {
            if (token != "typedef") {
                tokens.push_back(token);
            }
        }
        
        // Rebuild base type
        base.clear();
        for (const auto& t : tokens) {
            if (!base.empty()) base += " ";
            base += t;
        }
        std::stringstream sss(alias);
        while (sss >> token) {
            types.insert(token);
            typedefMap[token] = base;
        }
        
    }

    // Getter for the Typedef mapping
    std::string getTypedef(const std::string& typedefName){
        return typedefMap[typedefName];
    }

    // Check if a type exists
    bool contains(const std::string& typeName) const {
        return (types.find(getBaseType(typeName)) != types.end());
    }

    // Check if the type declaration contains a pointer
    bool hasPointer(const std::string& typeName) const {
        return typeName.find('*') != std::string::npos;
    }

    // Check if the type has less than one 'CONST'
    bool isValidConstDeclaration(const std::string& typeName) const {

        // Count 'CONST' occurrences
        size_t constCount = 0;
        std::stringstream ss(typeName);
        std::string token;
        
        while (ss >> token) {
            if (token == "CONST") {
                constCount++;
            }
        }

        // Return true only if less than one 'CONST' exists
        return (constCount <= 1);
    }

    // Check if the type has exactly one 'CONST'
    bool oneConst(const std::string& typeName) const {

        // Count 'CONST' occurrences
        size_t constCount = 0;
        std::stringstream ss(typeName);
        std::string token;
        
        while (ss >> token) {
            if (token == "CONST") {
                constCount++;
            }
        }

        // Return true only if less than one 'CONST' exists
        return (constCount == 1);
    }

    // To remove CONST from the declaration
    std::string removeConstFromDeclaration(const std::string& typeDecl) const {
        std::vector<std::string> tokens;
        std::stringstream ss(typeDecl);
        std::string token;
        
        while (ss >> token) {
            if (token != "CONST") {
                tokens.push_back(token);
            }
        }
        
        std::string result;
        for (size_t i = 0; i < tokens.size(); ++i) {
            if (i > 0) result += " ";
            result += tokens[i];
        }
        
        return result;
    }

    // Print all types in the set
    void printAllTypes() const {
        for (const auto& t : types) {
            std::cout << t << std::endl;
        }
    }

    // Print all typedef mappings
    void printTypedefs() const {
        for (const auto& [alias, original] : typedefMap) {
            std::cout << "typedef " << alias << " -> " << original << "\n";
        }
    }
};

#endif