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
        int scope_level;
        Symbol() = default;
        Symbol(const std::string& n, const std::string& t, int lvl)
            : name(n), type(t), scope_level(lvl) {}
    };

    struct TokenEntry {
        std::string token;
        std::string token_type;
        int scope_level;
    };

    SymbolTable() {
        // Start with global scope
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

    void insert_symbol(const std::string& name, const std::string& type) {
        // Check current scope first for redeclaration
        if (scopes_.back().count(name)) {
            std::string s =  "Error: Redeclaration of '" + name + "' in current scope\n";
            yyerror(s.c_str());
            return;
        }

        // Add to symbol table
        scopes_.back().emplace(name, Symbol(name, type, current_level_));
        
        // Add to token table
        token_table_.push_back({name, type, current_level_});
        char *token_type = new char[type.size()+1];
        char *ttoken = new char[name.size()+1];
        strcpy(ttoken, name.c_str());
        strcpy(token_type,type.c_str());
        if (strstr(token_type, "typedef") != NULL){
			update_symtab(ttoken);
		  }
    }

    // const Symbol* lookup(const std::string& name) const {
    //     // Search from current scope outward
    //     for (auto it = scopes_.rbegin(); it != scopes_.rend(); ++it) {
    //         if (auto search = it->find(name); search != it->end()) {
    //             return &search->second;
    //         }
    //     }
    //     return nullptr;
    // }

    bool lookup(const std::string &name) const {
        for (auto it = scopes_.rbegin(); it != scopes_.rend(); ++it) {
            if (auto search = it->find(name); search != it->end()) {
                return true;
            }
        }
        return false;
    }

    void print_symbol_table() const {
        std::cout << "\nSymbol Table:\n";
        std::cout << "-------------------------------------------------\n";
        std::cout << "| Name               | Type               | Scope |\n";
        std::cout << "-------------------------------------------------\n";
        
        for (const auto& scope : scopes_) {
            for (const auto& [name, sym] : scope) {
                std::printf("| %-18s | %-18s | %-5d |\n", 
                          name.c_str(), sym.type.c_str(), sym.scope_level);
            }
        }
        std::cout << "-------------------------------------------------\n";
    }

    void print_token_table() const {
        std::cout << "\nToken Table:\n";
        std::cout << "-------------------------------------------------\n";
        std::cout << "| Token              | Type               | Scope |\n";
        std::cout << "-------------------------------------------------\n";
        
        for (const auto& entry : token_table_) {
            std::printf("| %-18s | %-18s | %-5d |\n", 
                      entry.token.c_str(), entry.token_type.c_str(), entry.scope_level);
        }
        std::cout << "-------------------------------------------------\n";
    }

    std::vector<std::unordered_map<std::string, Symbol>> scopes_;
    std::vector<TokenEntry> token_table_;
    int current_level_ = -1;
};