#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>

class IRGen {
    std::vector<std::string> ir_code;
    int temp_counter = 0;
    int label_counter = 0;
    std::string current_label;
    
    struct ArrayInfo {
        int size;
        std::string base_name;
    };
    
    std::unordered_map<std::string, ArrayInfo> array_table;
    
public:
    // Temporary and label generation
    std::string new_temp();
    std::string new_label();
    
    // Arithmetic operations
    std::string emit_add(const std::string& src1, const std::string& src2);
    std::string emit_sub(const std::string& src1, const std::string& src2);



    void emit_assign(const std::string& dest, const std::string& src);
    void generate(const std::string& filename);
private:
    void emit_raw(const std::string& code);
};