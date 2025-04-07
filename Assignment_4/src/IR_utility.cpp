#include "IR_utility.hpp"

using namespace std;

string IRGen::new_temp() {
    return "$" + to_string(temp_counter++);
}

string IRGen::new_label() {
    return "@L" + to_string(label_counter++);
}

//concatenate codes
std::string IRGen::concatenate(std::string s1, std::string  s2){
    if(s1 == "")return s2;
    if(s2 == "")return s1;
   string result = s1;
   result += "\n";
   result += s2;

    return result; // Caller must free this
}


//assignment operators
std::string IRGen::add_op(std::string tmp, std::string s1,std::string op, std::string s2){
    std::string res;
    res = tmp + " = " + s1 + " " + op + " " + s2;
    return res;
}

std::string IRGen::assign(std::string tmp , std::string s1){
    std::string res;
    res = tmp +  " = " + s1;
    return res;
}

std::string IRGen::add_unary(std::string tmp , std::string op, std::string s1){
    std::string res;
    res = tmp + " = " + op + s1;
    return res;
}



//Function definition stuff

std::string IRGen::add_label(std::string f){
    std::string res;
    res = "label " + f + ":";
    return res;
}

std::string IRGen::add_par(std::string par){
    std::string res;
    res = "param " + par;
    return res;
}


std::string IRGen::func_call(std::string name, int param){
    string res;
    res = "call " + name + ", ";
    res += to_string(param);
    return res;
}

std::string IRGen::return_val(std::string temp){
string res;
res = "return " + temp;
return res;
}

//backpatching stuff
std::string IRGen::new_tmp_label() {
    return "@tmp" + std::to_string(tmp_label_counter++);
}

// Conditional jump: "IF <condition> GOTO <label>"
std::string IRGen::create_if_goto(const std::string& condition, const std::string& target_label) {
    return "if " + condition + " goto " + target_label;
}

// Unconditional jump: "GOTO <label>"
std::string IRGen::create_goto(const std::string& target_label) {
    return "goto " + target_label;
}

std::string IRGen::create_conditional_jump(
    const std::string& condition, 
    const std::string& true_label, 
    const std::string& false_label
) {
    // Generate "IF <condition> GOTO <true_label>"
    std::string if_code = create_if_goto(condition, true_label);
    
    // Generate "GOTO <false_label>"
    std::string goto_code = create_goto(false_label);
    
    // Combine them into two lines
    return concatenate(if_code, goto_code);
}


void IRGen::generate(const std::string& code, const std::string& filename) {
    std::string formatted_code = format_with_tabs(code);

    // Extract number from filename using regex
    std::smatch match;
    std::regex pattern("test(\\d+)\\.c");  // Matches 'test' followed by digits and '.c'

    std::string output_file = "./intermediate_representation/irgen.tac"; // default

    if (std::regex_search(filename, match, pattern) && match.size() > 1) {
        std::string number = match[1];  // Captured group with the digits
        output_file = "./intermediate_representation/irgen" + number + ".tac";
    }

    IRGen::write_to_file(output_file, formatted_code.c_str());
}
void IRGen::print(std::string s){
    if(s == ""){
        std::cout <<"NULL STRING \n";
        return;
    }
    std::cout <<s <<"\n";
}

std::string IRGen::format_with_tabs(const std::string& code) {
    std::istringstream input(code);
    std::ostringstream output;
    std::string line;

    bool inside_function = false;

    while (std::getline(input, line)) {
        std::string trimmed = line;
        trimmed.erase(0, trimmed.find_first_not_of(" \t"));  // Trim leading spaces

        // 1. Always handle label lines first
        if (trimmed.rfind("label ", 0) == 0) {
            if(inside_function)
            output << "\t"<<line << "\n";
            else output <<line << "\n";
            continue;
        }

        // 2. Detect start and end of function block
        if (trimmed.rfind("func_prologue", 0) == 0) {
            inside_function = true;
            output << "\t"<<line << "\n";
            continue;
        }

        if (trimmed.rfind("func_epilogue", 0) == 0) {
            inside_function = false;
            output << "\t"<<line << "\n";
            continue;
        }

        // 3. Indent inside-function lines (except labels)
        if (inside_function) {
            output << "\t\t" << line << "\n";
        } else {
            output << line << "\n";  // Outside function: print as-is
        }
    }

    return output.str();
}

void IRGen::write_to_file(const std::string& filename, const char* content) {
    if (content == nullptr) {  // Handle uninitialized or null pointers
        std::cout <<"\n\nNULL string exists \n\n";
        return;
    }
    
    std::ofstream out(filename); // Opens in "overwrite" mode (truncates file)
    if (out.is_open()) {
        out << content;
    }
}

// Switch case Funtions


// Start a new switch statement context
void IRGen::start_new_switch() {
    case_info_stack.push(std::vector<CaseInfo>());
    has_default_stack.push(false);
}

// Adds the case to current Switch case
void IRGen::add_case_info(const std::string& value, const std::string& label) {
    if (!case_info_stack.empty()) {
        case_info_stack.top().push_back({value, label,depth_current});
        
        // Update has_default flag if this is a default case
        if (value == "Default" && !has_default_stack.empty()) {
            has_default_stack.top() = true;
        }
    }
}

// to get depth of current stack
bool IRGen::get_depth(){
    for (const auto& info : case_info_stack.top()) {
        if (abs(info.depth - depth_current)>1) {
            return false;
        }
    }
    return true;
}

// checks for duplicate value for a case statement
bool IRGen::is_duplicate_case(const std::string& value) {
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
bool IRGen::has_default_label() {
    if (has_default_stack.empty()) {
        return false;
    }
    return has_default_stack.top();
}

//  To get the size fo current Switch case
int IRGen::get_case_info_size() {
    if (case_info_stack.empty()) {
        return 0;
    }
    
    return case_info_stack.top().size();
}

// Generate Switch cases
std::string IRGen::generate_switch_cases(const std::string& dispatch_var) {
    if (case_info_stack.empty()) {
        return "";
    }
    
    std::string result = "";
    std::string default_label = "";
    
    // Generate if-goto for each case
    for (const auto& case_info : case_info_stack.top()) {
        std::cout<<case_info.value<<std::endl;
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

void IRGen::end_switch() {
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
