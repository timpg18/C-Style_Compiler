#include "IR_utility.hh"

using namespace std;

string IRGen::new_temp() {
    return "t" + to_string(temp_counter++);
}

string IRGen::new_label() {
    return "L" + to_string(label_counter++);
}

void IRGen::emit_raw(const string& code) {
    if (!current_label.empty()) {
        ir_code.push_back(current_label + ":");
        current_label.clear();
    }
    ir_code.push_back("\t" + code);
}

// Arithmetic operations
string IRGen::emit_add(const string& src1, const string& src2) {
    string temp = new_temp();
    emit_raw(temp + " = " + src1 + " + " + src2);
    return temp;
}
std::string IRGen::emit_sub(const std::string& src1, const std::string& src2){
    string temp = new_temp();
    emit_raw(temp + " = " + src1 + " - " + src2);
    return temp;
}

std::string IRGen::emit_opn(const std::string& src1, const std::string& src2, const std::string& src3){
    string temp = new_temp();
    emit_raw(temp + " = " + src1 + src3 + src2);
    return temp;
}
void IRGen::emit_opn_id(const std::string& src1, const std::string& src2, const std::string& src3, const std::string& src4){
    
    emit_raw(src1 + " = " + src2 + src3 + src4);

}



void IRGen::emit_assign(const std::string& dest, const std::string& src) {
    emit_raw(dest + " = " + src);
}
void IRGen::generate(const string& filename) {
    ofstream out(filename);
    for (const auto& line : ir_code) {
        out << line << "\n";
    }
    out.close();
}