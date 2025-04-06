#include "IR_utility.hh"

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


void IRGen::generate(const std::string& code){
    std::string formatted_code = format_with_tabs(code);
    IRGen::write_to_file("irgen",formatted_code.c_str());
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



