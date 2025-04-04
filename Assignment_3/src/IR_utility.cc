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
    res = "LABEL " + f + ":";
    return res;
}

std::string IRGen::add_par(std::string par){
    std::string res;
    res = "PARAM " + par;
    return res;
}

//backpatching stuff
std::string IRGen::new_tmp_label() {
    return "@tmp" + std::to_string(tmp_label_counter++);
}

// Conditional jump: "IF <condition> GOTO <label>"
std::string IRGen::create_if_goto(const std::string& condition, const std::string& target_label) {
    return "IF " + condition + " GOTO " + target_label;
}

// Unconditional jump: "GOTO <label>"
std::string IRGen::create_goto(const std::string& target_label) {
    return "GOTO " + target_label;
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

    while (std::getline(input, line)) {
        // Check if line starts with "LABEL"
        if (line.find("LABEL ") == 0) {  // 👈 Space after LABEL is intentional
            output << line << "\n";
        } else {
            output << "\t" << line << "\n";  // 👈 Add tab to non-label lines
        }
    }
    return output.str();
}