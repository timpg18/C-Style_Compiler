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

        void add_case_info(const std::string& value, const std::string& label) {
            case_infos.push_back({value, label});
        }

        void set_default_label() {
            has_default = true;
        }

        bool is_duplicate_case(const std::string& value) {
            for (const auto& info : case_infos) {
                if (info.value == value) {
                    return true;
                }
            }
            return false;
        }

        bool has_default_label() {
            return has_default;
        }

        void clear_switch_info() {
            case_infos.clear();
            has_default = false;
        }

        int get_case_info_size(){
            return case_infos.size();
        }

        // Add this function to the public section of your IRGen class

    std::string generate_switch_cases(const std::string& dispatch_var) {
        std::string result = "";
        
        // Generate if-goto for each case
        for (const auto& case_info : case_infos) {
            if (case_info.value == "Default") {
                // Skip default case for now, we'll handle it at the end
                continue;
            }
            
            // Create condition: dispatch_var == case_value
            std::string condition = dispatch_var + " == " + case_info.value;
            
            // Use existing create_if_goto function
            std::string if_goto = create_if_goto(condition, case_info.label);
            
            // Concatenate to result
            result = concatenate(result, if_goto);
        }
        
        // Add goto for default case if it exists
        for (const auto& case_info : case_infos) {
            if (case_info.value == "Default") {
                // Use existing create_goto function
                std::string goto_default = create_goto(case_info.label);
                result = concatenate(result, goto_default);
                break;
            }
        }
        
        clear_switch_info();
        
        return result;
    }
        
    private:
    int temp_counter = 0;
    int label_counter = 0;
    int tmp_label_counter = 0;
    struct CaseInfo {
        std::string value;    // The constant expression value
        std::string label;    // The corresponding label
    };

    std::vector<CaseInfo> case_infos; //Store all cases
    bool has_default = false;

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