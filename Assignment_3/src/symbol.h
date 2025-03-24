#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <memory>
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



class SymbolTable {
public:
    struct Symbol {
        std::string name;
        std::string type;
        std::string kind;  // Added kind
        int scope_level;
        Symbol() = default;
        Symbol(const std::string& n, const std::string& t, const std::string& k, int lvl)
            : name(n), type(t), kind(k), scope_level(lvl) {}
    };

    struct TokenEntry {
        std::string token;
        std::string token_type;
        std::string kind;  // Added kind
        int scope_level;
    };

    SymbolTable() {
        push_scope();
    }

    void push_scope() {
        scopes_.emplace_back();
        current_level_++;
        std::cout << "Pushed scope level " << current_level_ << "\n";
    }

    void pop_scope() {
        if (scopes_.size() <= 1) {
            std::cerr << "Cannot pop global scope\n";
            return;
        }
        
        scopes_.pop_back();
        current_level_--;
        std::cout << "Popped scope. Current level: " << current_level_ << "\n";
    }

    // Updated to include kind parameter
    void insert_symbol(const std::string& name, const std::string& type, const std::string& kind) {
        if (scopes_.back().count(name)) {
            std::string s =  "Error: Redeclaration of '" + name + "' in current scope\n";
            yyerror(s.c_str());
            return;
        }

        // Added kind to symbol creation
        scopes_.back().emplace(name, Symbol(name, type, kind, current_level_));
        
        // Added kind to token entry
        token_table_.push_back({name, type, kind, current_level_});
        char *token_type = new char[type.size()+1];
        char *ttoken = new char[name.size()+1];
        strcpy(ttoken, name.c_str());
        strcpy(token_type,type.c_str());
        if (strstr(token_type, "typedef") != NULL){
            update_symtab(ttoken);
        }
    }
    const Symbol* lookup(const std::string& name) const {
        // Search from current scope outward
        for (auto it = scopes_.rbegin(); it != scopes_.rend(); ++it) {
            if (auto search = it->find(name); search != it->end()) {
                return &search->second;
            }
        }
        return nullptr;
    }
   

    void print_symbol_table() const {
        std::cout << "\nSymbol Table:\n";
        // Updated header with Kind
        std::cout << "-------------------------------------------------------------------\n";
        std::cout << "| Name               | Type               | Kind      | Scope |\n";
        std::cout << "-------------------------------------------------------------------\n";
        
        for (const auto& scope : scopes_) {
            for (const auto& [name, sym] : scope) {
                // Added kind to print
                std::printf("| %-18s | %-18s | %-10s | %-5d |\n", 
                          name.c_str(), sym.type.c_str(), sym.kind.c_str(), sym.scope_level);
            }
        }
        std::cout << "-------------------------------------------------------------------\n";
    }

    void print_token_table() const {
        std::cout << "\nToken Table:\n";
        // Updated header with Kind
        std::cout << "-------------------------------------------------------------------\n";
        std::cout << "| Token              | Type               | Kind      | Scope |\n";
        std::cout << "-------------------------------------------------------------------\n";
        
        for (const auto& entry : token_table_) {
            // Added kind to print
            std::printf("| %-18s | %-18s | %-10s | %-5d |\n", 
                      entry.token.c_str(), entry.token_type.c_str(), entry.kind.c_str(), entry.scope_level);
        }
        std::cout << "-------------------------------------------------------------------\n";
    }

    std::vector<std::unordered_map<std::string, Symbol>> scopes_;
    std::vector<TokenEntry> token_table_;
    int current_level_ = -1;
}; 

