#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <stack>
#include <regex>



class IRGen{

    private:
    int temp_counter ;
    int label_counter ;
    int tmp_label_counter ;

    // for Switch case handling
    struct CaseInfo {
        std::string value;    // The constant expression value
        std::string label;    // The corresponding label
        int depth;
    };

    // Stack of case info vectors to handle nested switches
    std::stack<std::vector<CaseInfo>> case_info_stack;
    std::stack<bool> has_default_stack;

    void write_to_file(const std::string& filename, const char* content);

    public:
    IRGen() {
        // Initialize counters
        temp_counter = 0;
        label_counter = 0;
        tmp_label_counter = 0;
        depth_current = 0;
        
        // Initialize first empty switch context
        case_info_stack.push(std::vector<CaseInfo>());
        has_default_stack.push(false);
    }

    std::vector<std::string> break_;
    std::vector<std::string> continue_;
    int depth_current;

    // Start a new switch statement context
    void start_new_switch();
    // End current switch statement context
    void end_switch();
    // Adds the case to current Switch case
    void add_case_info(const std::string& value, const std::string& label);
    // checks for duplicate value for a case statement
    bool is_duplicate_case(const std::string& value);
    // Check if default label is already defined in current switch
    bool has_default_label();
    // To get size of current Switch case
    int get_case_info_size();
    // Generate switch cases code for current switch context
    std::string generate_switch_cases(const std::string& dispatch_var);
    // To get current depth
    bool get_depth();
        

    //temporaries and label generation.
    std::string new_temp();
    std::string new_label();
    std::string concatenate(std::string s1, std::string s2);

    //Assignment
    std::string add_op(std::string tmp, std::string s1, std::string op,std::string s2);
    std::string assign(std::string tmp , std::string s1);
    std::string add_unary(std::string tmp , std::string op, std::string s1);

    //TYPECASTING
    std::string typecast(std::string new_temp, std::string old_temp ,std::string type);
    

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
    

    void generate(const std::string& code,const std::string& filename);
    void print(std::string s);
    std::string format_with_tabs(const std::string& code);

};