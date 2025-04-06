#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <stack>



class IRGen{
    public:
        IRGen() {
            // Initialize counters
            temp_counter = 0;
            label_counter = 0;
            tmp_label_counter = 0;
            
            // Initialize first empty switch context
            case_info_stack.push(std::vector<CaseInfo>());
            has_default_stack.push(false);
        }

        std::vector<std::string> break_;
        std::vector<std::string> continue_;

        // Start a new switch statement context
        void start_new_switch() {
            case_info_stack.push(std::vector<CaseInfo>());
            has_default_stack.push(false);
        }

        // End current switch statement context
        void end_switch() {
            if (!case_info_stack.empty()) {
                case_info_stack.pop();
                has_default_stack.pop();
                
                // Always ensure we have at least one context
                if (case_info_stack.empty()) {
                    case_info_stack.push(std::vector<CaseInfo>());
                    has_default_stack.push(false);
                }
            }
        }

        void add_case_info(const std::string& value, const std::string& label) {
            if (!case_info_stack.empty()) {
                case_info_stack.top().push_back({value, label});
                
                // Update has_default flag if this is a default case
                if (value == "Default" && !has_default_stack.empty()) {
                    has_default_stack.top() = true;
                }
            }
        }


        bool is_duplicate_case(const std::string& value) {
            if (case_info_stack.empty()) {
                return false;
            }
            
            for (const auto& info : case_info_stack.top()) {
                if (info.value == value) {
                    return true;
                }
            }
            return false;
        }

        // Check if default label is already defined in current switch
        bool has_default_label() {
            if (has_default_stack.empty()) {
                return false;
            }
            return has_default_stack.top();
        }


        int get_case_info_size() {
            if (case_info_stack.empty()) {
                return 0;
            }
            
            return case_info_stack.top().size();
        }

        // Generate switch cases code for current switch context
        std::string generate_switch_cases(const std::string& dispatch_var) {
            if (case_info_stack.empty()) {
                return "";
            }
            
            std::string result = "";
            std::string default_label = "";
            
            // Generate if-goto for each case
            for (const auto& case_info : case_info_stack.top()) {
                if (case_info.value == "Default") {
                    // Store default label for later use
                    default_label = case_info.label;
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
            if (!default_label.empty()) {
                std::string goto_default = create_goto(default_label);
                result = concatenate(result, goto_default);
            }
            
            return result;
        }
        
    private:
    int temp_counter ;
    int label_counter ;
    int tmp_label_counter ;

    // for Switch case handling
    struct CaseInfo {
        std::string value;    // The constant expression value
        std::string label;    // The corresponding label
    };

    // Stack of case info vectors to handle nested switches
    std::stack<std::vector<CaseInfo>> case_info_stack;
    std::stack<bool> has_default_stack;

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