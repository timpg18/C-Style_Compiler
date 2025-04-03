#include "IR_utility.hh"

using namespace std;

string IRGen::new_temp() {
    return "t" + to_string(temp_counter++);
}

string IRGen::new_label() {
    return "L" + to_string(label_counter++);
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
std::string IRGen::emit_sub(const std::string& src1, const std::string& src2){
    string temp = new_temp();
    emit_raw(temp + " = " + src1 + " - " + src2);
    return temp;

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

std::string IRGen::emit_opn(const std::string& src1, const std::string& src2, const std::string& src3){
    string temp = new_temp();
    emit_raw(temp + " = " + src1 + src3 + src2);
    return temp;
}
void IRGen::emit_opn_id(const std::string& src1, const std::string& src2, const std::string& src3, const std::string& src4){
    
    emit_raw(src1 + " = " + src2 + src3 + src4);

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