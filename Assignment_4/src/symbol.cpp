#include "symbol.hpp"
using namespace std;

// To push a new scope in symbol table
void SymbolTable::push_scope(const std::string& parent_symbol_name ) {
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

// To goto the parent scope
void SymbolTable::pop_scope() {
    if (current_scope_->parent_scope) {
        current_scope_ = current_scope_->parent_scope;
    }
    else{
        yyerror("Cannot pop global scope");
    }
}

// To insert symbol in symbol table
void SymbolTable::insert_symbol(const std::string& name, const std::string& type, 
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

// To transfer parameters of function to the function scope from the parent scope
void SymbolTable::transferParametersToFunctionScope(const std::string& function_name) {
    // Find the parent scope
    Scope* parent_scope = current_scope_->parent_scope;
    if (!parent_scope) {
        yyerror("No parent scope found");
        return;
    }
    
    // Find the function symbol in the parent scope
    auto func_it = parent_scope->symbol_map.find(function_name);
    if (func_it == parent_scope->symbol_map.end() || 
        func_it->second->kind.find("PROCEDURE") == std::string::npos) {
        std::string err = "Function '" + function_name + "' not found in parent scope";
        yyerror(err.c_str());
        return;
    }
    
    // Find the position of the function in the ordered symbols list
    auto func_pos = parent_scope->ordered_symbols.begin();
    for (; func_pos != parent_scope->ordered_symbols.end(); ++func_pos) {
        if (func_pos->name == function_name) {
            break;
        }
    }
    
    // If we reached the end, something went wrong
    if (func_pos == parent_scope->ordered_symbols.end()) {
        yyerror("Internal error: Function found in map but not in list");
        return;
    }
    
    // Process parameters directly after the function
    auto param_pos = std::next(func_pos);
    int size_reduction = 0;
    
    // Track the start of parameters for later removal
    auto erase_start = param_pos;
    
    // Process all consecutive parameters after the function
    while (param_pos != parent_scope->ordered_symbols.end() && 
           param_pos->kind.find("PARAMETER") != std::string::npos) {
        // Create a new entry in the current scope based on parameter data
        const std::string& name = param_pos->name;
        const std::string& type = param_pos->type;
        const std::string& kind = param_pos->kind;
        int param_size = param_pos->size;
        std::vector<int> dims = param_pos->dimensions; // Copy dimensions vector
        
        // Track size for later adjustment
        size_reduction += param_size;
        
        // Add parameter to token table for current scope
        token_table_.push_back({
            name, type, kind, 
            current_scope_->scope_level, param_size, 
            current_scope_->total_size, dims
        });
        
        // Insert directly into current scope
        current_scope_->ordered_symbols.emplace_back(
            name, type, kind, current_scope_->scope_level,
            param_size, current_scope_->total_size);
        
        // Update symbol map with iterator to new symbol
        auto new_sym_it = --current_scope_->ordered_symbols.end();
        current_scope_->symbol_map[name] = new_sym_it;
        
        // Copy dimensions to new symbol
        new_sym_it->dimensions = dims;
        
        // Update current scope size
        current_scope_->total_size += param_size;
        
        // Move to next parameter in parent scope
        ++param_pos;
    }
    
    // Store the end iterator for erasing
    auto erase_end = param_pos;
    
    // Remove parameters from parent scope
    for (auto it = erase_start; it != erase_end; ++it) {
        parent_scope->symbol_map.erase(it->name);
    }
    
    // Remove parameter entries from ordered_symbols
    parent_scope->ordered_symbols.erase(erase_start, erase_end);
    
    // Update parent scope's total size
    parent_scope->total_size -= size_reduction;
    
    // Call updateParameterSizes to zero out parameter sizes and fix offsets
    updateParameterSizes();
}

// To check if its an array
bool SymbolTable::is_array(const std::string& name) {
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

// To update the dimension sizes
void SymbolTable::update_dimension_sizes(const std::string& name){
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

// To update the dimension vector
void SymbolTable::update_array_dimensions(const std::string& name, int dimension, const std::string& baseType) {
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
void SymbolTable::changeToParameter(const std::string& identifiers) {
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
void SymbolTable::update_class_types(const std::string& priMem,const std::string& pubMem,const std::string& proMem){
       
    std::istringstream iss(priMem);
    std::string token;
    while (iss >> token) {
        auto it = current_scope_->symbol_map.find(token);
        if(it!=  current_scope_->symbol_map.end())
        it->second->kind += " PRIVATE";
    }

    std::istringstream isss(proMem);
    while (isss >> token) {
        auto it = current_scope_->symbol_map.find(token);
        if(it!=  current_scope_->symbol_map.end())
        it->second->kind += " PROTECTED";
    }

    std::istringstream issss(pubMem);
    while (issss >> token) {
        auto it = current_scope_->symbol_map.find(token);
        if(it!=  current_scope_->symbol_map.end())
        it->second->kind += " PUBLIC";
    }
}

// To lookup a symbol in the symbol table
SymbolTable::Symbol* SymbolTable::lookup(const std::string& name) {
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
SymbolTable::Symbol* SymbolTable::lookup_cur(const std::string& name) {
    Scope* scope = current_scope_;
    auto it = scope->symbol_map.find(name);
        if (it != scope->symbol_map.end()) {
            return &(*it->second);
        }
    return nullptr;
}

// ENCLOSING FUNCTION FINDER (NEW)
SymbolTable::Symbol* SymbolTable::get_enclosing_procedure() {
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
void SymbolTable::print_hierarchy() const {
    std::cout << "\nScope Hierarchy:\n";
    print_scope(global_scope_);
}

// FLAT TABLE PRINTER (ORIGINAL)
void SymbolTable::print_token_table() const {
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

// To implement inheritance
void SymbolTable::implement_inheritance(const std::string& derived_class_name, const std::string& inheritance_spec) {
    // Find the derived class in the scopes
    Scope* derived_scope = nullptr;
    for (const auto& scope_ptr : scopes_) {
        if (scope_ptr->scope_name == derived_class_name) {
            derived_scope = scope_ptr.get();
            break;
        }
    }
    
    if (!derived_scope) {
        std::string err = "Derived class '" + derived_class_name + "' not found";
        yyerror(err.c_str());
        return;
    }
    
    // Parse the inheritance specification
    std::istringstream iss(inheritance_spec);
    std::string access_specifier, base_class_name;
    
    while (iss >> access_specifier >> base_class_name) {
        // Validate access specifier (must be public, private, or protected)
        if (access_specifier != "public" && access_specifier != "private" && access_specifier != "protected") {
            std::string err = "Invalid access specifier '" + access_specifier + "' for inheritance";
            yyerror(err.c_str());
            continue;
        }
        
        // Find the base class in the scopes
        std::string class_name_ = "class " + base_class_name;
        Scope* base_scope = nullptr;
        for (const auto& scope_ptr : scopes_) {
            if (scope_ptr->scope_name == class_name_) {
                base_scope = scope_ptr.get();
                break;
            }
        }
        
        if (!base_scope) {
            std::string err = "Base class '" + base_class_name + "' not found";
            yyerror(err.c_str());
            continue;
        }
        
        // Copy all members from base class to derived class, adjusting access modifiers
        for (const auto& base_symbol : base_scope->ordered_symbols) {
            // Create a new symbol instead of copying (to avoid unique_ptr issue)
            std::string inherited_name =  base_symbol.name;
            
            // Determine the new access modifier based on inheritance rules
            std::string original_access = "";
            
            // Extract original access modifier from the kind string
            if (base_symbol.kind.find("PUBLIC") != std::string::npos) {
                original_access = "PUBLIC";
            } else if (base_symbol.kind.find("PROTECTED") != std::string::npos) {
                original_access = "PROTECTED";
            } else if (base_symbol.kind.find("PRIVATE") != std::string::npos) {
                original_access = "PRIVATE";
            }
            
            // Skip private members
            if (original_access == "PRIVATE") {
                continue;
            }
            
            // Remove old access modifier if present
            std::string new_kind = base_symbol.kind;
            if (!original_access.empty()) {
                size_t pos = new_kind.find(original_access);
                if (pos != std::string::npos) {
                    new_kind.erase(pos, original_access.length());
                }
                // Clean up extra spaces
                new_kind = std::regex_replace(new_kind, std::regex("\\s+"), " ");
                new_kind = std::regex_replace(new_kind, std::regex("^\\s+|\\s+$"), "");
            }
            
            // Apply inheritance access rules
            std::string new_access = "";
            
            if (original_access == "PROTECTED") {
                // Protected members remain protected under public inheritance,
                // become private under private inheritance
                if (access_specifier == "public") {
                    new_access = "PROTECTED";
                } else if (access_specifier == "private") {
                    new_access = "PRIVATE";
                } else { // protected inheritance
                    new_access = "PROTECTED";
                }
            } else if (original_access == "PUBLIC") {
                // Public members inherit the inheritance access specifier
                if (access_specifier == "public") {
                    new_access = "PUBLIC";
                } else if (access_specifier == "private") {
                    new_access = "PRIVATE";
                } else { // protected inheritance
                    new_access = "PROTECTED";
                }
            } else {
                // No explicit access specifier, treat as public
                if (access_specifier == "public") {
                    new_access = "PUBLIC";
                } else if (access_specifier == "private") {
                    new_access = "PRIVATE";
                } else { // protected inheritance
                    new_access = "PROTECTED";
                }
            }
            
            // Set the new kind with updated access modifier
            new_kind = new_kind + " " + new_access;
            
            // Add the new symbol to the derived scope
            derived_scope->ordered_symbols.emplace_back(
                inherited_name,
                base_symbol.type,
                new_kind,
                derived_scope->scope_level,
                base_symbol.size,
                derived_scope->total_size
            );
            
            // Get iterator to the newly added symbol
            auto it = std::prev(derived_scope->ordered_symbols.end());
            
            // Copy dimensions if any
            it->dimensions = base_symbol.dimensions;
            
            // Don't copy child_table as it's a unique_ptr
            // If needed, create a new one: it->child_table = std::make_unique<SymbolTable>();
            
            // Add to symbol map
            derived_scope->symbol_map[inherited_name] = it;
            
            // Update token table
            token_table_.push_back({
                inherited_name,
                base_symbol.type,
                new_kind,
                derived_scope->scope_level,
                base_symbol.size,
                derived_scope->total_size,
                base_symbol.dimensions
            });
            
            // Update the derived class's total size
            derived_scope->total_size += base_symbol.size;
        }
    }
    
    // Recalculate offsets with proper alignment for all symbols in the derived class
    recalculate_offsets_with_alignment(derived_scope);
}

// Helper function to recalculate offsets with alignment
void SymbolTable::recalculate_offsets_with_alignment(Scope* scope) {
    if (!scope) return;
    
    size_t current_offset = 0;
    
    for (auto& symbol : scope->ordered_symbols) {
        // Determine alignment requirement for this symbol
        size_t alignment = 1;
        if (symbol.size == 2) alignment = 2;
        else if (symbol.size == 4) alignment = 4;
        else if (symbol.size >= 8) alignment = 8;
        
        // Align the current offset
        if (current_offset % alignment != 0) {
            current_offset += alignment - (current_offset % alignment);
        }
        
        // Set the aligned offset
        symbol.offset = current_offset;
        
        // Update token table
        for (auto& entry : token_table_) {
            if (entry.token == symbol.name && entry.scope_level == scope->scope_level) {
                entry.offset = current_offset;
                break;
            }
        }
        
        // Move to next position
        current_offset += symbol.size;
    }
    
    // Update total size with proper alignment at the end
    size_t max_alignment = 8; // Maximum alignment requirement
    if (current_offset % max_alignment != 0) {
        current_offset += max_alignment - (current_offset % max_alignment);
    }
    
    // Set the final aligned size
    scope->total_size = current_offset;
    
}

// Declare struct variables in the scope
void SymbolTable::declare_struct_variables(const std::string& structName,const std::string& varList){
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

// To remove static form the Declaration
std::string SymbolTable::removeStaticFromDeclaration(const std::string& typeDecl) const {
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

// Count static
int SymbolTable::staticCount(const std::string& typeName) const {

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
bool SymbolTable::oneConst(const std::string& typeName) const {

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
std::string SymbolTable::removeConstFromDeclaration(const std::string& typeDecl) const {
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
void SymbolTable::print_all_scopes() const {
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
void SymbolTable::print_global_children() const {
    std::cout << "Children of Global Scope:\n";
    // Iterate through the children of the global scope
    for (auto child : global_scope_->children) {
        std::cout << "- " << child->scope_name << "\n";
    }
}

// this portion for resolving the goto labels;
void SymbolTable::pushlabel(std::string label){
    goto_label.push_back(label);
}

// Checking if Goto labels are resolved or not
bool SymbolTable::resolvelabels(const std::string funcName){
        
    for(const auto &scope_ptr:scopes_){
        if(scope_ptr->scope_name == funcName){
            int size = goto_label.size();
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

// To update the type of a variable
void SymbolTable::updateVariableTypes(const std::string& variables, const std::string& newType) {
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

// To update the Procedure size to 0
void SymbolTable::updateProcedureSize(const std::string& procName) {
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

// To update the parameter sizes to 0
void SymbolTable::updateParameterSizes() {
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
void SymbolTable::update_symbol_sizes(const std::string& names, int newSize) {
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

// To add a new type size
void SymbolTable::addTypeSize(const std::string& type_name, size_t size) {
    type_sizes[type_name] = size;
}

// Function to calculate the size of a struct and update it in the scope
int SymbolTable::calculateStructSize(const std::string& structName) {
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
                if(symbol.size!=0)
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
            
            return totalSize;
        }
    }
    
    // If we get here, the struct wasn't found
    std::string err = "Struct '" + structName + "' not found";
    yyerror(err.c_str());
}

// Function to calculate union size
int SymbolTable::calculateUnionSize(const std::string& unionName) {
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
            return maxSize;
        }
    }
    
    // If we get here, the union wasn't found
    std::string err = "Union '" + unionName + "' not found";
    yyerror(err.c_str());
}

// Function to print all entries in the type_sizes map
void SymbolTable::printTypeSizes() const {
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

// To get the type size
int SymbolTable::getTypeSize(const std::string& type) {
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

// To make break and continue flag false
void SymbolTable::falsekardo(){
    current_scope_->jump[0] = false;
    current_scope_->jump[1] = false;
    current_scope_->contains_break_or_continue = false;
}

// To remove Qualifers
std::string SymbolTable::removeQualifiers(const std::string& type) {
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

// Tokenizer
std::vector<std::string> SymbolTable::splitString(const std::string& str) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (ss >> token) tokens.push_back(token);
    return tokens;
}

void SymbolTable::print_scope(Scope* scope, int depth, bool is_last_child) const {
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
std::string SymbolTable::format_size(size_t size_in_bytes) const {
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

// This one is for checking of diamond inheritence
bool SymbolTable::hasDuplicateNamesInScope(const std::string& scope_name) {

    // Use a set to track names we've seen
    std::unordered_set<std::string> seen_names;
    for (const auto& scope_ptr : scopes_) {
        if (scope_ptr->scope_name == scope_name) {
            // Check each symbol in the scope
            for (const auto& symbol : scope_ptr->ordered_symbols) {
                // If we've seen this name before, we found a duplicate
                if (seen_names.find(symbol.name) != seen_names.end()) {
                    return true;
                }
                
                // Add this name to our set of seen names
                seen_names.insert(symbol.name);
            }
        }
    }
    
    // No duplicates found
    return false;
}