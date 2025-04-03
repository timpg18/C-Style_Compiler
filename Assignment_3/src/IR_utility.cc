#include "IR_utility.hh"

using namespace std;

string IRGen::new_temp() {
    return "t" + to_string(temp_counter++);
}

string IRGen::new_label() {
    return "L" + to_string(label_counter++);
}

std::string IRGen::concatenate(std::string s1, std::string  s2){
    if(s1 == "")return s2;
    if(s2 == "")return s1;
   string result = s1;
   result += "\n";
   result += s2;

    return result; // Caller must free this
}
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

std::string IRGen::func_def(std::string f){
    std::string res;
    res = "LABEL " + f + ":";
    return res;
}


void IRGen::generate(const char* s){
    IRGen::write_to_file("irgen",s);
}
void IRGen::print(std::string s){
    if(s == ""){
        std::cout <<"NULL STRING \n";
        return;
    }
    std::cout <<s <<"\n";
}