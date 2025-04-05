#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>



class IRGen{
    public:
        std::vector<std::string> break_;
        std::vector<std::string> continue_;
    private:
    int temp_counter = 0;
    int label_counter = 0;
    int tmp_label_counter = 0;

    void write_to_file(const std::string& filename, const char* content) {
        if (content == nullptr) {  // Handle uninitialized or null pointers
           std::cout <<"\n\nNULL string exists \n\n";
            return;
        }

        std::ofstream out(filename); // Opens in "overwrite" mode (truncates file)
        if (out.is_open()) {
            out << content;
        }
    }

    public:
    //temporaries and label generation.
    std::string new_temp();
    std::string new_label();
    std::string concatenate(std::string s1, std::string s2);

    //Assignment
    std::string add_op(std::string tmp, std::string s1, std::string op,std::string s2);
    std::string assign(std::string tmp , std::string s1);
    std::string add_unary(std::string tmp , std::string op, std::string s1);
    

    //func def
    std::string add_label(std::string f);
    std::string add_par(std::string par);
    std::string func_call(std::string name, int param);
    std::string return_val(std::string temp);
    //backpatching
    std::string new_tmp_label();
    std::string create_if_goto(const std::string& condition, const std::string& target_label); // Conditional jump
    std::string create_goto(const std::string& target_label); // Unconditional jump
    std::string create_conditional_jump(
        const std::string& condition, 
        const std::string& true_label,  // Label to jump to if condition is true
        const std::string& false_label   // Label to jump to if condition is false
    );
    

    void generate(const std::string& code);
    void print(std::string s);
    std::string format_with_tabs(const std::string& code);

};