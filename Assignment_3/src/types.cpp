#include "types.hpp"
using namespace std;

// Helper to remove trailing '*' characters
std::string TypeSet::getBaseType(const std::string& typeName) const {
    std::string base = typeName;
    
    // Remove trailing '*' characters
    while (!base.empty() && base.back() == '*') {
        base.pop_back();
    }
    
    std::vector<std::string> tokens;
    std::stringstream ss(base);
    std::string token;
    // Storage specifiers and qualifiers to remove
    const std::set<std::string> FILTER = {
        "static",  // Storage classes
        "CONST", "VOLATILE",          // Qualifiers
    };

    while (ss >> token) {
        if (!FILTER.count(token)) {
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

// to add user defined types
void TypeSet::addStruct(const std::string& name) { types.insert("struct " + name); }
void TypeSet::addUnion(const std::string& name) { types.insert("union " + name); }
void TypeSet::addEnum(const std::string& name) { types.insert("enum " + name); }
void TypeSet::addClass(const std::string& name) { types.insert("class " + name); }

// Add typedef alias
void TypeSet::addTypedef(const std::string& alias, const std::string& originalType) {
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
std::string TypeSet::getTypedef(const std::string& typedefName){
    return typedefMap[typedefName];
}

// Check if a type exists
bool TypeSet::contains(const std::string& typeName) const {
    return (types.find(getBaseType(typeName)) != types.end());
}

// Check if the type declaration contains a pointer
bool TypeSet::hasPointer(const std::string& typeName) const {
    return typeName.find('*') != std::string::npos;
}

// Check if the type has less than one 'CONST'
bool TypeSet::isValidConstDeclaration(const std::string& typeName) const {
    // Define a regex pattern that matches "CONST" anywhere in the string
    std::regex constPattern("CONST");

    // Use std::sregex_iterator to count occurrences
    auto begin = std::sregex_iterator(typeName.begin(), typeName.end(), constPattern);
    auto end = std::sregex_iterator();

    size_t constCount = std::distance(begin, end);

    // Return true only if there is at most one occurrence of "CONST"
    return (constCount <= 1);
}

// Check if the type has exactly one 'CONST'
bool  TypeSet::oneConst(const std::string& typeName) const {
    // Define a regex pattern that matches "CONST" anywhere in the string
    std::regex constPattern("CONST");

    // Iterator to find all occurrences
    auto begin = std::sregex_iterator(typeName.begin(), typeName.end(), constPattern);
    auto end = std::sregex_iterator();

    size_t constCount = std::distance(begin, end);

    // Return true if exactly one "CONST" exists (in any form)
    return (constCount == 1);
}

// To remove CONST from the declaration
std::string TypeSet::removeConstFromDeclaration(const std::string& typeDecl) const {
    // Step 1: Remove all occurrences of "CONST" (case-insensitive)
    std::string modified = std::regex_replace(
        typeDecl,
        std::regex("\\bCONST\\b", std::regex::icase),
        ""
    );

    // Step 2: Split into tokens (including handling multiple spaces)
    std::vector<std::string> tokens;
    std::stringstream ss(modified);
    std::string token;
    while (ss >> token) {
        tokens.push_back(token);
    }

    // Step 3: Merge tokens, avoiding spaces around '*' and between pointer stars
    std::string result;
    for (size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i] == "*") {
            // Merge consecutive '*' without spaces
            while (i < tokens.size() && tokens[i] == "*") {
                result += "*";
                ++i;
            }
            --i; // Adjust index after loop
        } else {
            if (!result.empty() && result.back() != ' ') {
                result += " ";
            }
            result += tokens[i];
        }
    }
    return result;
}

// To remove static from the declaration
std::string TypeSet::removeStaticFromDeclaration(const std::string& typeDecl) const {
    std::vector<std::string> tokens;
    std::stringstream ss(typeDecl);
    std::string token;
    
    while (ss >> token) {
        if (token != "static") {
            tokens.push_back(token);
        }
    }
    
    std::string result;
    for (size_t i = 0; i < tokens.size(); ++i) {
        if (i > 0 ){
                result += " ";
        } 
        result += tokens[i];
    }
    
    return result;
}

// To count storage Specifiers
int TypeSet::storagespecifercount(const std::string& typeName) const {

    // Count 'static' occurrences
    size_t staticCount = 0;
    
    std::vector<std::string> tokens;
    std::stringstream ss(typeName);
    std::string token;

    // Storage specifiers and qualifiers to remove
    const std::set<std::string> FILTER = {
        "static", "extern", "auto", "register",   // Storage classes
    };

    while (ss >> token) {
        if (FILTER.count(token)) {
            staticCount++;
        }
    }

    return staticCount;
}

// To count CONST
int TypeSet::constCount(const std::string& typeName) const {

    // Count 'CONST' occurrences
    size_t count = 0;
    
    std::vector<std::string> tokens;
    std::stringstream ss(typeName);
    std::string token;

    // Storage specifiers and qualifiers to remove
    const std::set<std::string> FILTER = {
        "CONST"   // Storage classes
    };

    while (ss >> token) {
        if (FILTER.count(token)) {
            count++;
        }
    }

    return count;
}

 // To check for constant pointer
 bool TypeSet::hasConstAfterStar(const std::string& typeDecl) {
    // Convert to uppercase for case-insensitive comparison
    std::string upperDecl;
    for (char c : typeDecl) {
        upperDecl += toupper(c);
    }

    // Split into tokens (including '*' as separate tokens)
    std::vector<std::string> tokens;
    std::stringstream ss(upperDecl);
    std::string token;
    
    while (ss >> token) {
        // Split token into subtokens if it contains '*'
        size_t pos = 0;
        while ((pos = token.find('*')) != std::string::npos) {
            if (pos > 0) tokens.push_back(token.substr(0, pos));
            tokens.push_back("*");
            token = token.substr(pos + 1);
        }
        if (!token.empty()) tokens.push_back(token);
    }

    // Check if any '*' is followed by "CONST" before another '*'
    for (size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i] == "*") {
            for (size_t j = i + 1; j < tokens.size(); ++j) {
                if (tokens[j] == "*") break;  // Next pointer found
                if (tokens[j] == "CONST") return true;
            }
        }
    }
    
    return false;
}

// TO give a string where const and static is removed
std::string TypeSet::prepareString(const std::string& type){
    std::string tmp = removeStaticFromDeclaration(type);
    tmp = removeConstFromDeclaration(tmp);
    return tmp;
}

// Add a static variable to a function's list
void TypeSet::addStaticVariable(const std::string& functionName, const std::string& varName) {
    std::stringstream ss(varName);
    std::string token;
    
    while (ss >> token) {
        functionStaticVars[functionName].push_back(varName);
    }
}

// Get the list of static variables for a function
std::vector<std::string> TypeSet::getStaticVariables(const std::string& functionName) const {
    auto it = functionStaticVars.find(functionName);
    if (it != functionStaticVars.end()) {
        return it->second;
    }
    return {}; // Return empty vector if not found
}

// Print all static variables for all functions
void TypeSet::printStaticVariables() const {
    for (const auto& [func, vars] : functionStaticVars) {
        std::cout << "Function: " << func << "\n";
        std::cout << "Static variables:\n";
        for (const auto& var : vars) {
            std::cout << " - " << var << "\n";
        }
        std::cout << "-----------------\n";
    }
}

// Print all types in the set
void TypeSet::printAllTypes() const {
    for (const auto& t : types) {
        std::cout << t << std::endl;
    }
}

// Print all typedef mappings
void TypeSet::printTypedefs() const {
    for (const auto& [alias, original] : typedefMap) {
        std::cout << "typedef " << alias << " -> " << original << "\n";
    }
}