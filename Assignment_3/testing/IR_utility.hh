#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>



class IRGen{
    int temp_counter = 0;
    int label_counter = 0;
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


    void generate(const char* s);
    void print(std::string s);

};