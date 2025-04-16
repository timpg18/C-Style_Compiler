#pragma once
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
#include <list>
#include <sstream>
#include <set>
#include <stdio.h>
#include <unordered_set>

int yylex();
void yyerror(const char *s);
extern int line_no;
extern void print_errors();
extern void print_symtab();
extern int error_count;
extern void update_symtab(char* ttoken);
extern int symcount;
extern char* yytext;


class SymbolTable {
public:
    struct Symbol {
        std::string name;
        std::string type;
        std::string kind;
        int scope_level;
        int block_num;
        int size;      // Added: size of the symbol in bytes
        int offset;    // Added: offset from scope base address
        std::vector<int> dimensions;  // Store array dimensions, empty for non-arrays
        std::unique_ptr<SymbolTable> child_table;

        Symbol(const std::string& n, const std::string& t, 
            const std::string& k, int lvl, int sz = 0, int off = 0, int block_n = 0)
            : name(n), type(t), kind(k), scope_level(lvl), size(sz),block_num(block_n), offset(off), dimensions() {}
    };

    struct TokenEntry {
        std::string token;
        std::string token_type;
        std::string kind;
        int scope_level;
        int size;     
        int offset;   
        std::vector<int> dimensions; // Store array dimensions
    };

    struct Scope {
        std::list<Symbol> ordered_symbols;
        std::unordered_map<std::string, std::list<Symbol>::iterator> symbol_map;
        Scope* parent_scope;
        std::string scope_name;
        int scope_level;
        int block_num;
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
              total_size(0), block_num(0) {}
    };

    std::vector<TokenEntry> token_table_;

    SymbolTable() {
        global_scope_ = new Scope(nullptr, "global", 0);
        current_scope_ = global_scope_;
        scopes_.emplace_back(global_scope_);
        insert_symbol("printf","INT","PROCEDURE ( CHAR* ... )");
        insert_symbol("scanf","INT","PROCEDURE ( CHAR* ... )");
        insert_symbol("malloc","INT*","PROCEDURE ( INT )");
        insert_symbol("free","VOID","PROCEDURE ( VOID* )");
        updateProcedureSize("printf");
        updateProcedureSize("scanf");
        updateProcedureSize("malloc");
        updateProcedureSize("free");
    }

    // Operations on Symbol table
    // To push a new scope in symbol table
    void push_scope(const std::string& parent_symbol_name = "");
    // To goto the parent scope
    void pop_scope();
    // To insert symbol in symbol table
    void insert_symbol(const std::string& name, const std::string& type,const std::string& kind);
    // To transfer parameters of function to the function scope from the parent scope
    void transferParametersToFunctionScope(const std::string& function_name);
    // To check if its an array
    bool is_array(const std::string& name);
    // To update the dimension sizes
    void update_dimension_sizes(const std::string& name);
    // To update the dimension vector
    void update_array_dimensions(const std::string& name, int dimension, const std::string& baseType);
    // To change the IDENTIFEIR to PARAMETER for function
    void changeToParameter(const std::string& identifiers);
    // To update the Kind for access specifier in class
    void update_class_types(const std::string& priMem,const std::string& pubMem,const std::string& proMem);
    // To lookup a symbol in the symbol table
    Symbol* lookup(const std::string& name);
    // ENCLOSING FUNCTION FINDER (NEW)
    Symbol* lookup_cur(const std::string& name);
    // ENCLOSING FUNCTION FINDER (NEW)
    Symbol* get_enclosing_procedure();
    // HIERARCHY PRINTER (NEW)
    void print_hierarchy() const;
    // FLAT TABLE PRINTER (ORIGINAL)
    void print_token_table() const;
    // To implement inheritance
    void implement_inheritance(const std::string& derived_class_name, const std::string& inheritance_spec);
    // Helper function to recalculate offsets with alignment
    void recalculate_offsets_with_alignment(Scope* scope) ;
    // Declare struct variables in the scope
    void declare_struct_variables(const std::string& structName,const std::string& varList);
    // To remove static form the Declaration
    std::string removeStaticFromDeclaration(const std::string& typeDecl) const;
    // Count static
    int staticCount(const std::string& typeName) const;
    // Check if the type has exactly one 'CONST'
    bool oneConst(const std::string& typeName) const;
    // To remove CONST from the declaration
    std::string removeConstFromDeclaration(const std::string& typeDecl) const;
    // For printing names of all scopes (Debugging)
    void print_all_scopes() const;
    // For printing children of global scope (Debugging)
    void print_global_children() const ;
    // this portion for resolving the goto labels;
    void pushlabel(std::string label);
    // Checking if Goto labels are resolved or not
    bool resolvelabels(const std::string funcName);
    // To update the type of a variable
    void updateVariableTypes(const std::string& variables, const std::string& newType);
    // To update the Procedure size to 0
    void updateProcedureSize(const std::string& procName);
    // To update the parameter sizes to 0
    void updateParameterSizes();
    // To change symbol size to a different value
    void update_symbol_sizes(const std::string& names, int newSize);
    // To add a new type size
    void addTypeSize(const std::string& type_name, size_t size);
    // Function to calculate the size of a struct and update it in the scope
    int calculateStructSize(const std::string& structName);
    // Function to calculate union size
    int calculateUnionSize(const std::string& unionName);
    // Function to print all entries in the type_sizes map
    void printTypeSizes() const;
    // To get the type size
    int getTypeSize(const std::string& type);
    // To make break and continue flag false
    void falsekardo();
    // To remove Qualifers
    std::string removeQualifiers(const std::string& type);
    // To check for Diamond inheritence
    bool hasDuplicateNamesInScope(const std::string& scope_name);

    // Public declarations
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

    // Tokenizer
    std::vector<std::string> splitString(const std::string& str);
    // Print a scope
    void print_scope(Scope* scope, int depth = 0, bool is_last_child = false) const;
    // Helper function to format sizes in human-readable form
    std::string format_size(size_t size_in_bytes) const;
};