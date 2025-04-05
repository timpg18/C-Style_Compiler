#ifndef SYMBOL_H
#define SYMBOL_H
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <cstring>
#include <regex>
#include <iomanip>
#include <locale>
int yylex();
void yyerror(const char *s);
#include <stdio.h>



char *currentType = NULL;
int parser_error=0;
int token_count=0;
extern int line_no;
extern void print_errors();
extern void print_symtab();
extern int error_count;
extern void update_symtab(char* ttoken);
extern int symcount;
extern char* yytext;


#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <list>
#include <cstring>
#include <sstream>
#include <set>

class SymbolTable {
public:
    struct Symbol {
        std::string name;
        std::string type;
        std::string kind;
        int scope_level;
        int size;      // Added: size of the symbol in bytes
        int offset;    // Added: offset from scope base address
        std::vector<int> dimensions;  // Store array dimensions, empty for non-arrays
        std::unique_ptr<SymbolTable> child_table;

        Symbol(const std::string& n, const std::string& t, 
            const std::string& k, int lvl, int sz = 0, int off = 0)
            : name(n), type(t), kind(k), scope_level(lvl), size(sz), offset(off), dimensions() {}
    };

    struct TokenEntry {
        std::string token;
        std::string token_type;
        std::string kind;
        int scope_level;
        int size;     // Added
        int offset;   // Added
        std::vector<int> dimensions; // Store array dimensions
    };

    struct Scope {
        std::list<Symbol> ordered_symbols;
        std::unordered_map<std::string, std::list<Symbol>::iterator> symbol_map;
        Scope* parent_scope;
        std::string scope_name;
        int scope_level;
        int total_size;// Added: total size of all symbols in this scope
        bool jump[2] = {false, false}; //0 for continue 1 for break;
        bool contains_break_or_continue = false;
        std::vector<Scope*> children;
        
        Scope(Scope* parent = nullptr, 
             std::string name = "global", 
             int level = 0)
            : parent_scope(parent), 
              scope_name(name), 
              scope_level(level),
              total_size(0) {}
    };

    std::vector<TokenEntry> token_table_;

    SymbolTable() {
        
        global_scope_ = new Scope(nullptr, "global", 0);
        current_scope_ = global_scope_;
        scopes_.emplace_back(global_scope_);
        insert_symbol("printf","INT","PROCEDURE ( ARG DEPENDENT )");
        insert_symbol("scanf","INT","PROCEDURE ( ARG DEPENDENT )");
        insert_symbol("malloc","INT*","PROCEDURE ( INT )");
        updateProcedureSize("printf");
        updateProcedureSize("scanf");
        updateProcedureSize("malloc");
    }

    void push_scope(const std::string& parent_symbol_name = "") {
        std::string scope_name = parent_symbol_name.empty() ? 
            "anonymous" : parent_symbol_name;
        
        Scope* new_scope = new Scope(
            current_scope_,
            scope_name,
            current_scope_->scope_level + 1
        );
        
        current_scope_->children.push_back(new_scope);
        scopes_.emplace_back(new_scope);
        
        if (!parent_symbol_name.empty()) {
            Scope* search_scope = current_scope_;
            while (search_scope) {
                auto it = search_scope->symbol_map.find(parent_symbol_name);
                if (it != search_scope->symbol_map.end()) {
                    it->second->child_table = std::make_unique<SymbolTable>();
                    it->second->child_table->current_scope_ = new_scope;
                    break;
                }
                search_scope = search_scope->parent_scope;
            }
        }

        current_scope_ = new_scope;
    }

    void pop_scope() {
        if (current_scope_->parent_scope) {
            current_scope_ = current_scope_->parent_scope;
        }
        else{
            yyerror("Cannot pop global scope");
        }
    }

    void insert_symbol(const std::string& name, const std::string& type, 
        const std::string& kind) {
        int type_size = getTypeSize(type);
        int current_offset = current_scope_->total_size;
        token_table_.push_back({name, type, kind, current_scope_->scope_level, type_size, current_offset});

        if (current_scope_->symbol_map.find(name) != current_scope_->symbol_map.end()) {
            std::string s =  "Error: Redeclaration of '" + name + "' in current scope\n";
            yyerror(s.c_str());
            return;
        }

        current_scope_->ordered_symbols.emplace_back(name, type, kind, current_scope_->scope_level, type_size, current_offset);

        auto it = --current_scope_->ordered_symbols.end();
        current_scope_->symbol_map[name] = it;
        
        // Update scope size
        current_scope_->total_size += type_size;

        if (type.find("typedef") != std::string::npos) {
            char* ttoken = new char[name.size() + 1];
            std::strcpy(ttoken, name.c_str());
            update_symtab(ttoken);
        }
    }

    bool is_array(const std::string& name) {
        // Find the symbol in the current scope or parent scopes
        Scope* scope = current_scope_;
        
        // Look up the symbol
        while (scope) {
            auto it = scope->symbol_map.find(name);
            if (it != scope->symbol_map.end()) {
                // Check if the symbol has dimensions
                return !it->second->dimensions.empty();
            }
            scope = scope->parent_scope;
        }
        
        // Symbol not found
        std::string err = "Identifier '" + name + "' not found when checking if it's an array";
        yyerror(err.c_str());
        return false;
    }

    void update_dimension_sizes(const std::string& name){
        // Find the symbol in the current scope or parent scopes
        Scope* scope = current_scope_;
        std::list<Symbol>::iterator symbolIt;
        bool found = false;
        
        // Look up the symbol
        while (scope && !found) {
            auto it = scope->symbol_map.find(name);
            if (it != scope->symbol_map.end()) {
                symbolIt = it->second;
                found = true;
                break;
            }
            scope = scope->parent_scope;
        }
        
        if (!found) {
            std::string err = "Identifier '" + name + "' not found for dimension update";
            yyerror(err.c_str());
            return;
        }
        std::string& baseType = symbolIt->type;
        
        std::cout<<(symbolIt->dimensions.size())<<"\n\n";
        // Remove appropriate number of pointer stars from the type
        std::string actualBaseType = baseType;
        int totalElements = 1;
        // Remove stars based on dimension count (for multi-dimensional arrays)
        for(int i=0;i<(symbolIt->dimensions.size());i++){
            actualBaseType.pop_back();
            totalElements*=(symbolIt->dimensions[i]);

        }
        if (totalElements==0){
            yyerror("This array declaration is not allowed");
        }
        
        // Calculate the size of a single element
        int elementSize = getTypeSize(actualBaseType);
        
        // Calculate total size
        int originalSize = symbolIt->size;
        int newSize = elementSize * totalElements;
        int sizeDelta = newSize - originalSize;
        std::cout<<actualBaseType<<"\n\n";
        std::cout<<newSize<<"\n\n";
        // Update the symbol size
        symbolIt->size = newSize;
        
        // Update the token table entry
        for (auto& entry : token_table_) {
            if (entry.token == name && entry.scope_level == scope->scope_level) {
                entry.dimensions = symbolIt->dimensions;
                entry.size = newSize;
            }
        }
        
        // Adjust offsets for all subsequent symbols in the same scope
        bool adjustOffsets = false;
        for (auto& sym : scope->ordered_symbols) {
            if (adjustOffsets) {
                sym.offset += sizeDelta;
                
                // Update token table entry for this symbol too
                for (auto& entry : token_table_) {
                    if (entry.token == sym.name && entry.scope_level == scope->scope_level) {
                        entry.offset = sym.offset;
                    }
                }
            } else if (&sym == &(*symbolIt)) {
                // Start adjusting offsets after the current symbol
                adjustOffsets = true;
            }
        }
        
        // Update the scope's total size
        scope->total_size += sizeDelta;
    }

    void update_array_dimensions(const std::string& name, int dimension, const std::string& baseType) {
        Scope* scope = current_scope_;
        std::list<Symbol>::iterator symbolIt;
        bool found = false;
        
        // Look up the symbol
        while (scope && !found) {
            auto it = scope->symbol_map.find(name);
            if (it != scope->symbol_map.end()) {
                symbolIt = it->second;
                found = true;
                break;
            }
            scope = scope->parent_scope;
        }
        
        if (!found) {
            std::string err = "Identifier '" + name + "' not found for dimension update";
            yyerror(err.c_str());
            return;
        }
        
        // If dimensions vector doesn't exist yet, initialize it
        if (!symbolIt->dimensions.empty()) {

            symbolIt->dimensions.push_back(dimension);
        } else {
            symbolIt->dimensions = {dimension};
        }

        for (auto& entry : token_table_) {
            if (entry.token == name && entry.scope_level == scope->scope_level) {
                entry.dimensions = symbolIt->dimensions;
            }
        }
    }

    // To change the IDENTIFEIR to PARAMETER for function
    void changeToParameter(const std::string& identifiers) {
        std::istringstream iss(identifiers);
        std::string token;
        
        while (iss >> token) {
            // Lookup in current scope only
            auto it = current_scope_->symbol_map.find(token);
            
            if (it != current_scope_->symbol_map.end()) {
                // Modify the symbol's kind directly in the scope's symbol list
                it->second->kind = "PARAMETER";
                
                // Optional: Update token table if needed
                for (auto& entry : token_table_) {
                    if (entry.token == token && 
                        entry.scope_level == current_scope_->scope_level) {
                        entry.kind = "PARAMETER";
                        break;
                    }
                }
            } else {
                std::string err = "Identifier '" + token + 
                                 "' not found in current scope";
                yyerror(err.c_str());
            }
        }
    }

    // To update the Kind for access specifier in class
    void update_class_types(const std::string& priMem,const std::string& pubMem,const std::string& proMem){
       
        std::istringstream iss(priMem);
        std::string token;
        while (iss >> token) {
            auto it = current_scope_->symbol_map.find(token);
            it->second->kind += " PRIVATE";
        }

        std::istringstream isss(proMem);
        while (isss >> token) {
            auto it = current_scope_->symbol_map.find(token);
            it->second->kind += " PROTECTED";
        }

        std::istringstream issss(pubMem);
        while (issss >> token) {
            auto it = current_scope_->symbol_map.find(token);
            it->second->kind += " PUBLIC";
        }
    }

    Symbol* lookup(const std::string& name) {
        Scope* scope = current_scope_;
        while (scope) {
            auto it = scope->symbol_map.find(name);
            if (it != scope->symbol_map.end()) {
                return &(*it->second);
            }
            scope = scope->parent_scope;
        }
        return nullptr;
    }

    // ENCLOSING FUNCTION FINDER (NEW)
    Symbol* get_enclosing_procedure() {
        Scope* current = current_scope_;
        while (current && current != global_scope_) {
            // Check parent scope for procedure symbols
            if (current->parent_scope) {
                for (auto& sym : current->parent_scope->ordered_symbols) {
                    if (sym.kind == "procedure" && 
                        sym.child_table && 
                        sym.child_table->current_scope_ == current) {
                        return &sym;
                    }
                }
            }
            current = current->parent_scope;
        }
        return nullptr;
    }

    // HIERARCHY PRINTER (NEW)
    void print_hierarchy() const {
        std::cout << "\nScope Hierarchy:\n";
        print_scope(global_scope_);
    }

    // FLAT TABLE PRINTER (ORIGINAL)
    void print_token_table() const {
        // Calculate maximum column widths
        size_t max_name_len = 4;   // "Name"
        size_t max_type_len = 4;   // "Type"
        size_t max_kind_len = 4;   // "Kind"
        size_t max_scope_len = 5;  // "Scope"
        size_t max_size_len = 4;   // "Size"
        size_t max_offset_len = 6; // "Offset"
    
        for (const auto& entry : token_table_) {
            max_name_len = std::max(max_name_len, entry.token.length());
            max_type_len = std::max(max_type_len, entry.token_type.length());
            max_kind_len = std::max(max_kind_len, entry.kind.length());
            
            std::string scope_str = std::to_string(entry.scope_level);
            std::string size_str = std::to_string(entry.size);
            std::string offset_str = std::to_string(entry.offset);
            
            max_scope_len = std::max(max_scope_len, scope_str.length());
            max_size_len = std::max(max_size_len, size_str.length());
            max_offset_len = std::max(max_offset_len, offset_str.length());
        }
    
        // Add padding
        const size_t padding = 2;
        max_name_len += padding;
        max_type_len += padding;
        max_kind_len += padding;
        max_scope_len += padding;
        max_size_len += padding;
        max_offset_len += padding;
    
        // Calculate total width
        const size_t total_width = max_name_len + max_type_len + max_kind_len 
                                 + max_scope_len + max_size_len + max_offset_len + 7;
    
        // Header
        std::cout << "\n+" << std::string(total_width - 2, '-') << "+\n";
        std::cout << "|" << std::string((total_width - 12)/2, ' ') 
                 << "SYMBOL TABLE" 
                 << std::string((total_width - 12)/2, ' ') << "|\n";
        std::cout << "+" << std::string(max_name_len, '-') << "+"
                 << std::string(max_type_len, '-') << "+"
                 << std::string(max_kind_len, '-') << "+"
                 << std::string(max_scope_len, '-') << "+"
                 << std::string(max_size_len, '-') << "+"
                 << std::string(max_offset_len, '-') << "+\n";
    
        // Column headers
        std::cout << "| " << std::left << std::setw(max_name_len) << "Name" << "| "
                  << std::setw(max_type_len) << "Type" << "| "
                  << std::setw(max_kind_len) << "Kind" << "| "
                  << std::setw(max_scope_len) << "Scope" << "| "
                  << std::setw(max_size_len) << "Size" << "| "
                  << std::setw(max_offset_len) << "Offset" << "|\n";
    
        // Separator
        std::cout << "+" << std::string(max_name_len, '-') << "+"
                 << std::string(max_type_len, '-') << "+"
                 << std::string(max_kind_len, '-') << "+"
                 << std::string(max_scope_len, '-') << "+"
                 << std::string(max_size_len, '-') << "+"
                 << std::string(max_offset_len, '-') << "+\n";
    
        // Data rows
        for (const auto& entry : token_table_) {
            std::cout << "| " << std::left << std::setw(max_name_len) << entry.token << "| "
                      << std::setw(max_type_len) << entry.token_type << "| "
                      << std::setw(max_kind_len) << entry.kind << "| "
                      << std::right << std::setw(max_scope_len) << entry.scope_level << "| "
                      << std::setw(max_size_len) << entry.size << "| "
                      << std::setw(max_offset_len) << entry.offset << "|\n";
        }
    
        // Footer
        std::cout << "+" << std::string(max_name_len, '-') << "+"
                 << std::string(max_type_len, '-') << "+"
                 << std::string(max_kind_len, '-') << "+"
                 << std::string(max_scope_len, '-') << "+"
                 << std::string(max_size_len, '-') << "+"
                 << std::string(max_offset_len, '-') << "+\n";
    }



    // Declare struct variables in the scope
    void declare_struct_variables(const std::string& structName,const std::string& varList){
        bool hasConst = false;
        bool hasStatic = false;
        std::string tmp = structName;
        if(oneConst(structName)){
            hasConst=1;
            tmp = removeConstFromDeclaration(structName);
        }
        if(staticCount(tmp)==1){
            hasStatic =true;
            tmp = removeStaticFromDeclaration(tmp);
        }

        
        for(const auto &scope_ptr:scopes_){
            if(scope_ptr->scope_name == tmp){
                std::istringstream iss(varList);
                std::string token;
                while (iss >> token) {
                    for (const auto &member : scope_ptr->ordered_symbols) {
                        std::string newName = token + "." + member.name;
                        std::cout<< newName<<"\n";
                        if(hasConst && hasStatic){
                            insert_symbol(newName, std::string("static ") + std::string("CONST ") + member.type, member.kind);
                        }
                        else if(hasConst){
                            insert_symbol(newName,  "CONST " + member.type, member.kind);
                        }
                        else if(hasStatic){
                            insert_symbol(newName, "static "  + member.type, member.kind);
                        }
                        else insert_symbol(newName, member.type, member.kind);
                        update_symbol_sizes(newName,0);
                    }
                    
                }
                return;
            }
        }
        
        yyerror(("error:" + structName + " is not declared").c_str());
    }

    std::string removeStaticFromDeclaration(const std::string& typeDecl) const {
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
            if (i > 0) result += " ";
            result += tokens[i];
        }
        
        return result;
    }

    int staticCount(const std::string& typeName) const {

        // Count 'static' occurrences
        size_t staticCount = 0;
        
        std::vector<std::string> tokens;
        std::stringstream ss(typeName);
        std::string token;

        // Storage specifiers and qualifiers to remove
        const std::set<std::string> FILTER = {
            "static"  // Storage classes
        };

        while (ss >> token) {
            if (FILTER.count(token)) {
                staticCount++;
            }
        }

        return staticCount;
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

    // For printing names of all scopes (Debugging)
    void print_all_scopes() const {
        std::cout << "Scopes in the symbol table:\n";
        // Iterate over each scope in the symbol table
        for (const auto& scope_ptr : scopes_) {
            std::cout << "Scope Name: " << scope_ptr->scope_name 
                      << ", Level: " << scope_ptr->scope_level;
            if (scope_ptr->parent_scope) {
                std::cout << ", Parent Scope: " << scope_ptr->parent_scope->scope_name;
            } else {
                std::cout << ", Parent Scope: None";
            }
            std::cout << "\n";
    
            // Now print the full hierarchy starting from this scope.
            // Here we use a depth of 1 (or any starting indent you prefer)
            print_scope(scope_ptr.get(), 1);
            std::cout << "\n";
        }
    }

    // For printing children of global scope (Debugging)
    void print_global_children() const {
        std::cout << "Children of Global Scope:\n";
        // Iterate through the children of the global scope
        for (auto child : global_scope_->children) {
            std::cout << "- " << child->scope_name << "\n";
        }
    }

    // this portion for resolving the goto labels;
    void pushlabel(std::string label){
        goto_label.push_back(label);
    }

    bool resolvelabels(const std::string funcName){
        
        for(const auto &scope_ptr:scopes_){
            if(scope_ptr->scope_name == funcName){
                std::cout<<"hello"<<std::endl;
                int size = goto_label.size();
                std::cout<<size<<std::endl;
                if(size==0)return true;
                int count = 0 ;
                for(const auto &label : goto_label){
                    if(scope_ptr->symbol_map.find(label)!=scope_ptr->symbol_map.end()){
                        auto it = scope_ptr->symbol_map.find(label);
                        if(it->second->type =="LABEL")count++;
                    }
                }
                goto_label.clear();
                if(count==size)return true;
                else return false;
            }
        }
    }

    void updateVariableTypes(const std::string& variables, const std::string& newType) {
        std::vector<std::string> vars = splitString(variables);
        if (vars.empty()) return;

        Scope* currentScope = current_scope_;
        std::list<Symbol>::iterator firstSymbol;
        int baseOffset = -1;
        int accumulatedDelta = 0;

        // Find and update types/sizes
        for (const auto& varName : vars) {
            auto it = currentScope->symbol_map.find(varName);
            if (it == currentScope->symbol_map.end()) {
                yyerror(("Variable '" + varName + "' not found").c_str());
                return;
            }
            

            Symbol& sym = *it->second;
            if(!sym.dimensions.empty()){
                update_dimension_sizes(varName);
                return ;
            }
            if (baseOffset == -1) {
                firstSymbol = it->second;
                baseOffset = sym.offset;
            }

            // Calculate size changes
            int oldSize = sym.size;
            sym.type = newType;
            sym.size = getTypeSize(newType);
            std::cout<<"HELLO WORLD"<<"   "<<sym.size<<std::endl;
            accumulatedDelta += (sym.size - oldSize);
        }

        // Update offsets for subsequent variables
        int currentOffset = baseOffset;
        auto it = firstSymbol;
        
        // Update all symbols from first modified one
        for (; it != currentScope->ordered_symbols.end(); ++it) {
            it->offset = currentOffset;
            currentOffset += it->size;
            
            // Update token table entry
            for (auto& tentry : token_table_) {
                if (tentry.token == it->name && 
                    tentry.scope_level == currentScope->scope_level) {
                    tentry.token_type = it->type;
                    tentry.size = it->size;
                    tentry.offset = it->offset;
                }
            }
        }

        // Update scope total size
        currentScope->total_size += accumulatedDelta;
    }

    void updateProcedureSize(const std::string& procName) {
        // Find the procedure symbol in the current scope only
        auto it = current_scope_->symbol_map.find(procName);
        
        // If procedure not found in current scope, report error
        if (it == current_scope_->symbol_map.end() || 
            it->second->kind.find("PROCEDURE") == std::string::npos) {
            std::string err = "Procedure '" + procName + "' not found in current scope";
            yyerror(err.c_str());
            return;
        }
        
        // Get a reference to the procedure symbol
        Symbol& procSymbol = *it->second;
        
        // Store the original size to calculate the offset adjustment
        int originalSize = procSymbol.size;
        
        // Update the procedure symbol size to 0
        procSymbol.size = 0;
        
        // Calculate the delta to adjust subsequent symbols
        int sizeDelta = -originalSize;
        
        // Find position of the procedure in the ordered symbols list
        auto listIt = current_scope_->ordered_symbols.begin();
        bool foundProc = false;
        
        for (; listIt != current_scope_->ordered_symbols.end(); ++listIt) {
            if (&(*listIt) == &procSymbol) {
                foundProc = true;
                break;
            }
        }
        
        if (!foundProc) {
            yyerror("Internal error: Procedure found in map but not in list");
            return;
        }
        
        // Move to the next symbol after the procedure
        ++listIt;
        
        // Starting offset for the next symbol
        int nextOffset = procSymbol.offset;
        
        // Update offsets for all subsequent symbols in the same scope
        for (; listIt != current_scope_->ordered_symbols.end(); ++listIt) {
            listIt->offset = nextOffset;
            nextOffset += listIt->size;
            
            // Update the token table entry for this symbol
            for (auto& entry : token_table_) {
                if (entry.token == listIt->name && entry.scope_level == current_scope_->scope_level) {
                    entry.offset = listIt->offset;
                    break;
                }
            }
        }
        
        // Update the token table entry for the procedure
        for (auto& entry : token_table_) {
            if (entry.token == procName && entry.scope_level == current_scope_->scope_level) {
                entry.size = 0;
                break;
            }
        }
        
        // Update the total size of the scope
        current_scope_->total_size += sizeDelta;
    }

    void updateParameterSizes() {
        // Check if current scope has any symbols
        if (current_scope_->ordered_symbols.empty()) {
            return; // Nothing to update
        }
        
        // First pass: identify parameters and calculate total size reduction
        int totalSizeReduction = 0;
        std::vector<std::reference_wrapper<Symbol>> parameters;
        
        for (auto& symbol : current_scope_->ordered_symbols) {
            if (symbol.kind.find("PARAMETER") != std::string::npos) {
                totalSizeReduction += symbol.size;
                parameters.push_back(std::ref(symbol));
            }
        }
        
        if (parameters.empty()) {
            return; // No parameters found in current scope
        }
        
        // Second pass: update parameter sizes to 0
        for (auto& paramRef : parameters) {
            Symbol& param = paramRef.get();
            param.size = 0;
            
            // Update the token table entry for this parameter
            for (auto& entry : token_table_) {
                if (entry.token == param.name && 
                    entry.scope_level == current_scope_->scope_level) {
                    entry.size = 0;
                    break;
                }
            }
        }
        
        // Third pass: recalculate offsets for all symbols in the current scope
        int currentOffset = 0;
        for (auto& symbol : current_scope_->ordered_symbols) {
            symbol.offset = currentOffset;
            currentOffset += symbol.size;
            
            // Update the token table entry for this symbol
            for (auto& entry : token_table_) {
                if (entry.token == symbol.name && 
                    entry.scope_level == current_scope_->scope_level) {
                    entry.offset = symbol.offset;
                    break;
                }
            }
        }
        
        // Update the total size of the current scope
        current_scope_->total_size -= totalSizeReduction;
    }

    // to change symbol size to a different value
    void update_symbol_sizes(const std::string& names, int newSize) {
        std::istringstream nameStream(names);
        std::string name;
        
        // Process each name in the space-separated list
        while (nameStream >> name) {
            // Find the symbol in the current scope or parent scopes
            Scope* scope = current_scope_;
            std::list<Symbol>::iterator symbolIt;
            bool found = false;
            
            // Look up the symbol
            while (scope && !found) {
                auto it = scope->symbol_map.find(name);
                if (it != scope->symbol_map.end()) {
                    symbolIt = it->second;
                    found = true;
                    break;
                }
                scope = scope->parent_scope;
            }
            
            if (!found) {
                std::string err = "Identifier '" + name + "' not found for size update";
                yyerror(err.c_str());
                continue; // Skip to next name
            }
            
            // Calculate the size difference
            int originalSize = symbolIt->size;
            int sizeDelta = newSize - originalSize;
            
            // Update the symbol size
            symbolIt->size = newSize;
            
            // Update the token table entry
            for (auto& entry : token_table_) {
                if (entry.token == name && entry.scope_level == scope->scope_level) {
                    entry.size = newSize;
                }
            }
            
            // Adjust offsets for all subsequent symbols in the same scope
            bool adjustOffsets = false;
            for (auto& sym : scope->ordered_symbols) {
                if (adjustOffsets) {
                    sym.offset += sizeDelta;
                    
                    // Update token table entry for this symbol too
                    for (auto& entry : token_table_) {
                        if (entry.token == sym.name && entry.scope_level == scope->scope_level) {
                            entry.offset = sym.offset;
                        }
                    }
                } else if (&sym == &(*symbolIt)) {
                    // Start adjusting offsets after the current symbol
                    adjustOffsets = true;
                }
            }
            
            // Update the scope's total size
            scope->total_size += sizeDelta;
        }
    }

    void addTypeSize(const std::string& type_name, size_t size) {
        type_sizes[type_name] = size;
    }

    // Function to calculate the size of a struct and update it in the scope
    int calculateStructSize(const std::string& structName) {
        // Find the struct's scope in scopes_
        for (const auto& scope_ptr : scopes_) {
            if (scope_ptr->scope_name == structName) {
                // Struct found, calculate its size
                size_t totalSize = 0;
                size_t maxAlignment = 1;  // Track the maximum alignment requirement
                
                // First pass: determine max alignment needed
                for (const auto& symbol : scope_ptr->ordered_symbols) {
                    // Crude alignment estimation based on size
                    // In practice, this would depend on the platform's ABI rules
                    size_t alignment = 1;
                    if (symbol.size == 2) alignment = 2;
                    else if (symbol.size == 4) alignment = 4;
                    else if (symbol.size >= 8) alignment = 8;
                    
                    maxAlignment = std::max(maxAlignment, alignment);
                }
                
                // Second pass: calculate actual size with padding
                for (const auto& symbol : scope_ptr->ordered_symbols) {
                    // Add padding for alignment if needed
                    if (totalSize % symbol.size != 0) {
                        size_t padding = symbol.size - (totalSize % symbol.size);
                        totalSize += padding;
                    }
                    
                    // Add this member's size
                    totalSize += symbol.size;
                }
                
                // Final struct size should be a multiple of its alignment
                if (totalSize % maxAlignment != 0) {
                    totalSize += maxAlignment - (totalSize % maxAlignment);
                }
                
                // Update the scope's total size
                scope_ptr->total_size = totalSize;
                
                std::cout << "Struct '" << structName << "' size calculated: " << totalSize << " bytes\n";
                return totalSize;
            }
        }
        
        // If we get here, the struct wasn't found
        std::string err = "Struct '" + structName + "' not found";
        yyerror(err.c_str());
    }

    int calculateUnionSize(const std::string& unionName) {
        // Find the union's scope in scopes_
        for (const auto& scope_ptr : scopes_) {
            if (scope_ptr->scope_name == unionName) {
                // Union found, calculate its size
                size_t maxSize = 0;
                
                // Iterate through all symbols in this union scope
                for (const auto& symbol : scope_ptr->ordered_symbols) {
                    // Find the maximum size among all members
                    if (symbol.size > maxSize) {
                        maxSize = symbol.size;
                    }
                }
                
                // Update the scope's total size to match the largest member
                scope_ptr->total_size = maxSize;
                
                std::cout << "Union '" << unionName << "' size calculated: " << maxSize << " bytes\n";
                return maxSize;
            }
        }
        
        // If we get here, the union wasn't found
        std::string err = "Union '" + unionName + "' not found";
        yyerror(err.c_str());
    }

    // Function to print all entries in the type_sizes map
    void printTypeSizes() const {
        // Calculate maximum column widths
        size_t max_type_len = 4;   // "Type"
        size_t max_size_len = 4;   // "Size"
        
        for (const auto& entry : type_sizes) {
            max_type_len = std::max(max_type_len, entry.first.length());
            std::string size_str = std::to_string(entry.second);
            max_size_len = std::max(max_size_len, size_str.length());
        }
        
        // Add padding
        const size_t padding = 2;
        max_type_len += padding;
        max_size_len += padding;
        
        // Calculate total width
        const size_t total_width = max_type_len + max_size_len + 3;
        
        // Print header
        std::cout << "\n+" << std::string(total_width - 2, '-') << "+\n";
        std::cout << "|" << std::string((total_width - 10)/2, ' ') 
                << "TYPE SIZES" 
                << std::string((total_width - 10)/2, ' ') << "|\n";
        std::cout << "+" << std::string(max_type_len, '-') << "+"
                << std::string(max_size_len, '-') << "+\n";
        
        // Column headers
        std::cout << "| " << std::left << std::setw(max_type_len) << "Type" << "| "
                << std::setw(max_size_len) << "Size" << "|\n";
        
        // Separator
        std::cout << "+" << std::string(max_type_len, '-') << "+"
                << std::string(max_size_len, '-') << "+\n";
        
        // Sort entries alphabetically for better readability
        std::vector<std::pair<std::string, size_t>> sorted_entries(
            type_sizes.begin(), type_sizes.end()
        );
        std::sort(sorted_entries.begin(), sorted_entries.end(),
            [](const auto& a, const auto& b) { return a.first < b.first; });
        
        // Data rows
        for (const auto& entry : sorted_entries) {
            std::cout << "| " << std::left << std::setw(max_type_len) << entry.first << "| "
                    << std::right << std::setw(max_size_len) << entry.second << "|\n";
        }
        
        // Footer
        std::cout << "+" << std::string(max_type_len, '-') << "+"
                << std::string(max_size_len, '-') << "+\n";
        std::cout << "Total types: " << type_sizes.size() << "\n\n";
    }
    
    int getTypeSize(const std::string& type) {
        // Remove qualifiers (const, volatile, static, etc.)
        std::string baseType = removeQualifiers((type));
        
        // Handle pointers (all pointers have same size)
        if (baseType.find('*') != std::string::npos) {
            // Return pointer size (4 for 32-bit, 8 for 64-bit)
            return sizeof(void*);
        }
            
        // Look up basic types
        auto it = type_sizes.find(baseType);
        if (it != type_sizes.end()) {
            return it->second;
        }
    
        // Default to 4 bytes for unknown types
        return 4;
    }
    void falsekardo(){
        current_scope_->jump[0] = false;
        current_scope_->jump[1] = false;
        current_scope_->contains_break_or_continue = false;
    }
    std::string removeQualifiers(const std::string& type) {
        static const std::set<std::string> qualifiers = {
            "CONST", "VOLATILE", "static", "register", 
            "extern", "auto", "restrict", "inline",
            "declared" // in our implementation declared can be present
        };
    
        std::string result;
        std::istringstream iss(type);
        std::string token;
        
        while (iss >> token) {
            if (qualifiers.find(token) == qualifiers.end()) {
                if (!result.empty()) result += " ";
                result += token;
            }
        }
        return result;
    }
    std::vector<std::unique_ptr<Scope>> scopes_;
    Scope* current_scope_;
private:
    Scope* global_scope_;
    
   
    std::vector<std::string> goto_label ; // labels to be resolved
    // Handle basic types
    std::unordered_map<std::string, int> type_sizes = {
        {"VOID", 0},
        {"BOOL", 1},
        {"CHAR", 1},
        {"SIGNED CHAR", 1},
        {"UNSIGNED CHAR", 1},
        {"SHORT", 2},
        {"SHORT INT", 2},
        {"SIGNED SHORT", 2},
        {"SIGNED SHORT INT", 2},
        {"UNSIGNED SHORT", 2},
        {"UNSIGNED SHORT INT", 2},
        {"INT", 4},
        {"SIGNED", 4},
        {"SIGNED INT", 4},
        {"UNSIGNED", 4},
        {"UNSIGNED INT", 4},
        {"LONG", sizeof(long)},
        {"LONG INT", sizeof(long)},
        {"SIGNED LONG", sizeof(long)},
        {"SIGNED LONG INT", sizeof(long)},
        {"UNSIGNED LONG", sizeof(unsigned long)},
        {"UNSIGNED LONG INT", sizeof(unsigned long)},
        {"LONG LONG", sizeof(long long)},
        {"LONG LONG INT", sizeof(long long)},
        {"SIGNED LONG LONG", sizeof(long long)},
        {"SIGNED LONG LONG INT", sizeof(long long)},
        {"UNSIGNED LONG LONG", sizeof(unsigned long long)},
        {"UNSIGNED LONG LONG INT", sizeof(unsigned long long)},
        {"FLOAT", 4},
        {"DOUBLE", 8},
        {"LONG DOUBLE", sizeof(long double)},
        {"LABEL",0}
    };

    std::vector<std::string> splitString(const std::string& str) {
        std::vector<std::string> tokens;
        std::stringstream ss(str);
        std::string token;
        while (ss >> token) tokens.push_back(token);
        return tokens;
    }

    
    void print_scope(Scope* scope, int depth = 0, bool is_last_child = false) const {
        if (!scope) return;
        
        std::string indent(depth * 2, ' ');
        std::string branch = is_last_child ? "└─" : "├─";
        std::string vertical = is_last_child ? "  " : "│ ";
        
        // Print scope header with better formatting
        std::cout << indent << branch << "─ " << scope->scope_name 
                 << " (level: " << scope->scope_level
                 << ", size: " << format_size(scope->total_size) << ")\n";
        
        // Get iterator for the list
        auto it = scope->ordered_symbols.begin();
        auto end = scope->ordered_symbols.end();
        
        // Count to determine if we're at the last element
        size_t remaining = scope->ordered_symbols.size();
        
        // Iterate through the list instead of using indexing
        while (it != end) {
            const auto& sym = *it;
            remaining--;
            bool is_last_symbol = (remaining == 0);
            
            // Print symbol with updated tree characters
            std::cout << indent << vertical << " " << (is_last_symbol ? "└─" : "├─") << " "
                     << std::left << std::setw(18) << sym.name 
                     << " : " << std::setw(10) << sym.type
                     << " [" << std::setw(15) << sym.kind << "]";
            
            // Align and format size and offset
            std::cout << " Size: " << std::setw(8) << format_size(sym.size)
                     << " Offset: " << std::setw(8) << sym.offset;
            
            // Print dimensions with better formatting
            if (!sym.dimensions.empty()) {
                std::cout << " Dims: [";
                for (size_t d = 0; d < sym.dimensions.size(); ++d) {
                    if (d > 0) std::cout << "][";
                    std::cout << sym.dimensions[d];
                }
                std::cout << "]";
            }
            
            std::cout << "\n";
            
            // Print child table if present
            if (sym.child_table) {
                sym.child_table->print_scope(
                    sym.child_table->current_scope_,
                    depth + 2,
                    is_last_symbol
                );
            }
            
            ++it; // Move to next element
        }
    }
    
    // Helper function to format sizes in human-readable form
    std::string format_size(size_t size_in_bytes) const {
        const char* units[] = {"B", "KB", "MB", "GB"};
        double size = static_cast<double>(size_in_bytes);
        int unit_index = 0;
        
        while (size >= 1024.0 && unit_index < 3) {
            size /= 1024.0;
            unit_index++;
        }
        
        std::ostringstream formatted;
        if (unit_index == 0) {
            formatted << size_in_bytes << " " << units[unit_index];
        } else {
            formatted << std::fixed << std::setprecision(2) << size << " " << units[unit_index]
                     << " (" << size_in_bytes << " B)";
        }
        
        return formatted.str();
    }
};

#endif