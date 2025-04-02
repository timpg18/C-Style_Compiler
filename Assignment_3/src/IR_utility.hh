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
    void write_to_file(const std::string& filename, const std::string& content) {
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

    //arithmetic functions
    std::string add_op(std::string tmp, std::string s1, std::string op,std::string s2);
    std::string assign(std::string tmp , std::string s1);
    std::string add_unary(std::string tmp , std::string op, std::string s1);

    void generate(const std::string s);
    void print(std::string s);

};