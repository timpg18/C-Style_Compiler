#ifndef SYMBOL_H
#define SYMBOL_H
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <cstring>
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

class SymbolTable {
public:
    struct Symbol {
        std::string name;
        std::string type;
        std::string kind;
        int scope_level;
        std::unique_ptr<SymbolTable> child_table;

        Symbol(const std::string& n, const std::string& t, 
              const std::string& k, int lvl)
            : name(n), type(t), kind(k), scope_level(lvl) {}
    };

    struct TokenEntry {
        std::string token;
        std::string token_type;
        std::string kind;
        int scope_level;
    };

    struct Scope {
        std::list<Symbol> ordered_symbols;
        std::unordered_map<std::string, std::list<Symbol>::iterator> symbol_map;
        Scope* parent_scope;
        std::string scope_name;
        int scope_level;
        std::vector<Scope*> children;

        Scope(Scope* parent = nullptr, 
             std::string name = "global", 
             int level = 0)
            : parent_scope(parent), 
              scope_name(name), 
              scope_level(level) {}
    };

    std::vector<TokenEntry> token_table_;

    SymbolTable() {
        global_scope_ = new Scope(nullptr, "global", 0);
        current_scope_ = global_scope_;
        scopes_.emplace_back(global_scope_);
        insert_symbol("printf","INT","PROCEDURE ( ARG DEPENDENT )");
        insert_symbol("scanf","INT","PROCEDURE ( ARG DEPENDENT )");
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
        token_table_.push_back({name, type, kind, current_scope_->scope_level});

        if (current_scope_->symbol_map.find(name) != current_scope_->symbol_map.end()) {
            std::string s =  "Error: Redeclaration of '" + name + "' in current scope\n";
            yyerror(s.c_str());
            return;
        }

        current_scope_->ordered_symbols.emplace_back(name, type, kind, current_scope_->scope_level);
        auto it = --current_scope_->ordered_symbols.end();
        current_scope_->symbol_map[name] = it;

        if (type.find("typedef") != std::string::npos) {
            char* ttoken = new char[name.size() + 1];
            std::strcpy(ttoken, name.c_str());
            update_symtab(ttoken);
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
        std::cout << "\nToken Table:\n";
        std::cout << "--------------------------------------------------------\n";
        std::cout << "| Name       | Type       | Kind        | Scope Level |\n";
        std::cout << "--------------------------------------------------------\n";
        for (const auto& entry : token_table_) {
            std::printf("| %-10s | %-10s | %-12s | %-11d |\n",
                       entry.token.c_str(),
                       entry.token_type.c_str(),
                       entry.kind.c_str(),
                       entry.scope_level);
        }
        std::cout << "--------------------------------------------------------\n";
    }

    // Declare struct variables in the scope
    void declare_struct_variables(const std::string& structName,const std::string& varList){
        for(const auto &scope_ptr:scopes_){
            if(scope_ptr->scope_name == structName){
                std::istringstream iss(varList);
                std::string token;
                while (iss >> token) {
                    for (const auto &member : scope_ptr->ordered_symbols) {
                        std::string newName = token + "." + member.name;
                        std::cout<< newName<<"\n";
                        insert_symbol(newName, member.type, member.kind);
                    }
                    
                }
                return;
            }
        }
        
        yyerror(("error:" + structName + " is not declared").c_str());
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
    

private:
    Scope* global_scope_;
    Scope* current_scope_;
    std::vector<std::unique_ptr<Scope>> scopes_;

    void print_scope(Scope* scope, int depth = 0) const {
        std::string indent(depth * 2, ' ');
        std::cout << indent << "└─ " << scope->scope_name 
                 << " (level: " << scope->scope_level << ")\n";

        // Print symbols in declaration order
        for (const auto& sym : scope->ordered_symbols) {
            std::cout << indent << "   ├─ " << sym.name << " : " << sym.type
                     << " [" << sym.kind << "]\n";
            if (sym.child_table) {
                sym.child_table->print_scope(
                    sym.child_table->current_scope_,
                    depth + 2
                );
            }
        }

        // Print anonymous scopes in insertion order
        for (auto child : scope->children) {
            if (child->scope_name == "anonymous") {
                print_scope(child, depth + 2);
            }
        }
    }
};

#endif