%{
#include "symbol.hpp"
#include "Utility_func.h"
#include "types.hpp"
#include "typeconversion.h"
#include "IR_utility.hpp"
#include "backPatcher.h"
#include "AddressAllocationTable.hpp"
#include "BasicBlockConstructor.hpp";
#include "RegisterDescriptor.hpp"
#include "CodeGenerator.hpp"
#include "MACROS.hpp"
using namespace std;

char *currentType = NULL;
int parser_error=0;
int token_count=0;
extern FILE *yyin;

// #include "backPatcher.h"
IRGen irgen;

TypeSet ts;
SymbolTable st;
int classDef = 0;
int block_num = 0;
int isPub=0,isPro=0,isPri=0;
std::string pubMem,proMem,priMem = "";
std::string currFunc = "";
int bpneeded = 0; 
std::string final_ir_code = "";
int counter = 1;

std::string formatTypeChange(const std::string& type1, const std::string& type2) {
    std::string lowerType1 = type1;
    std::string lowerType2 = type2;

    std::transform(lowerType1.begin(), lowerType1.end(), lowerType1.begin(), ::tolower);
    std::transform(lowerType2.begin(), lowerType2.end(), lowerType2.begin(), ::tolower);

    return "cast: " + lowerType1 + " -> " + lowerType2 + " ";
}

std::string removeDeclared(const std::string& input) {
    std::string result = input;
    std::string toRemove = " declared";
    size_t pos;

    while ((pos = result.find(toRemove)) != std::string::npos) {
        result.erase(pos, toRemove.length());
    }

    return result;
}

std::map<std::string,std::string> enum_to_value;
int curr_enum_value =0;

%}

%code requires {
	#include "backPatcher.h"
}
%debug
%union{
	char *id;
	typedef struct {
    char* type;
    char* kind;
	int index;
	char* name;
	BackPatcher* backpatcher;
	typedef struct{
		char* code;
		char* tmp;
		char* par;
	} irg;
	irg ir;
	} attribute_t;
	

	
	attribute_t atr;
}

%token <atr> IDENTIFIER STRING_LIT CHAR_LIT
%token <atr> I_CONSTANT F_CONSTANT TRUE FALSE
%token PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token XOR_ASSIGN OR_ASSIGN TYPEDEF_NAME

%token TYPEDEF EXTERN STATIC AUTO REGISTER SIZEOF
%token CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID BOOL
%token STRUCT UNION ENUM ELLIPSIS

%token CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN UNTIL
%start Global


/* idk uses of this */
%token    FUNC_NAME 
%token	 ENUMERATION_CONSTANT

/* Define keyword tokens missing in this C file.*/
%token <atr> CLASS 
%token PUBLIC PRIVATE PROTECTED

%type <atr> type_specifier declaration_specifiers
%type <atr> direct_declarator declarator 
%type <atr> struct_or_union_specifier struct_or_union
%type <atr> enum_specifier
%type <atr> constant_expression 
%type <atr> pointer type_qualifier_list
%type <atr> storage_class_specifier type_qualifier 
%type <atr> class_specifier access_specifier base_clause_opt base_specifier_list base_specifier  access_specifier_opt

%type <atr> primary_expression constant 
%type <atr> unary_expression postfix_expression cast_expression multiplicative_expression additive_expression shift_expression relational_expression equality_expression and_expression exclusive_or_expression
%type <atr> inclusive_or_expression logical_and_expression logical_or_expression conditional_expression assignment_expression
%type <atr> init_declarator unary_operator
%type <atr> initializer assignment_operator selection_statement block_item_list jump_statement iteration_statement 

%type <atr> parameter_type_list parameter_list parameter_declaration abstract_declarator direct_abstract_declarator initializer_list
%type <atr> specifier_qualifier_list type_name
%type <atr> init_declarator_list
%type <atr> argument_expression_list  expression string labeled_statement BOOLEAN Global translation_unit external_declaration
%type <atr> declaration declaration_list function_definition  block_item compound_statement statement expression_statement
%type <atr> struct_declaration struct_declarator_list struct_declarator enumeration_constant

/* currently removed for now 
ALIGNAS ALIGNOF ATOMIC GENERIC NORETURN STATIC_ASSERT THREAD_LOCAL
ELLIPSIS COMPLEX IMAGINARY COMPLEX IMAGINARY 
*/
%%

primary_expression
	: IDENTIFIER{
		
		char *name = strdup($1.type);
		std::string tmp = name;
		
		if(st.lookup(tmp) == nullptr){
			std::string err = "Undeclared Identifier: " + tmp;
    		yyerror(err.c_str());
		}
		else{
			CHECK_ACCESS(std::string($1.type));
		$$.type = strdup(st.lookup(tmp)->type.c_str());
		$$.kind = strdup(st.lookup(tmp)->kind.c_str());
		$$.name = name;
		int num;
		num = st.lookup(tmp)->block_num;
		
		
		tmp += "#block";
		tmp += to_string(num);
		if(contains($$.kind,"ENUM_CONST")) tmp = enum_to_value[std::string(name)];
		$$.ir.tmp = strdup(tmp.c_str());
		//$$.name=  strdup(tmp.c_str());
		$$.ir.code = "";
		//printf("\n\n%s\n\n%s\n\n%s",$$.type,$$.kind,$$.name);
		}
		$$.backpatcher = new BackPatcher();
	}
	| constant{
		$$.type = $1.type;
		$$.kind = $1.kind;
		$$.ir.tmp = strdup($1.ir.tmp);
		$$.ir.code = "";
		$$.name = $1.name;
		$$.backpatcher = new BackPatcher();
	}
	| string {
		$$.type = $1.type;
		$$.name = $1.name;
		$$.kind = $1.kind;
		$$.ir.tmp = strdup($1.ir.tmp);
		$$.ir.code = "";
		$$.backpatcher = new BackPatcher();
		
	}
	| '(' expression ')'{
		$$.name = $2.name;
		$$.type = $2.type; 
		$$.ir.tmp = $2.ir.tmp;
		$$.ir.code = strdup($2.ir.code);
		$$.backpatcher = BackPatcher::copy($2.backpatcher);
    	delete $2.backpatcher;
	}
	| '(' expression error { yyerrok; }
	;

constant
	: I_CONSTANT{
		$$.name = $1.type;
		$$.type = "INT";
		$$.kind = "CONST";
		$$.ir.tmp = strdup((std::string(yytext)).c_str());
	}		/* includes character_constant */
	| F_CONSTANT{
		$$.name = $1.type;
		$$.type = "FLOAT";
		$$.kind = "CONST";
		$$.ir.tmp = strdup((std::string(yytext)).c_str());
	}
	| ENUMERATION_CONSTANT{
		$$.type = "INT";
		$$.kind = "ENUM_CONST";
		$$.name = strdup((std::string(yytext)).c_str());
		$$.ir.tmp = strdup((std::string(yytext)).c_str());
	}	/* after it has been defined as such */
	| BOOLEAN{
		$$.type = "INT";
		$$.kind = "CONST";
		$$.name = $1.name;
		$$.ir.tmp = strdup($1.ir.tmp);
	}
	;

enumeration_constant		/* before it has been defined as such */
	: IDENTIFIER {
		st.insert_symbol($1.type,"INT" , "ENUM_CONST");
		st.update_symbol_sizes(std::string($1.type),0);
		$$.name = $1.type;
	}
	;

BOOLEAN
	: TRUE {
		$$.ir.tmp  = "true";
		$$.name = "TRUE";
	}
	| FALSE{
		$$.ir.tmp = "false";
		$$.name = "FALSE";
	}

string
	: STRING_LIT{
		$$.name = $1.type;
		$$.type = "CHAR*";
		$$.kind = "IDENTIFIER";
		$$.ir.tmp = strdup((std::string(yytext)).c_str());
	}
	| CHAR_LIT {
		$$.name = $1.type;
		$$.type = "CHAR";
		$$.kind = "IDENTIFIER";
		$$.ir.tmp = strdup((std::string(yytext)).c_str());
	}
	| FUNC_NAME {

	}
	;


postfix_expression
	: primary_expression {
		$$.type = $1.type;
		$$.kind = $1.kind;
		$$.name = $1.name;
		$$.ir.tmp = strdup($1.ir.tmp);
		$$.ir.code = strdup($1.ir.code);
		$$.backpatcher = BackPatcher::copy($1.backpatcher);
    	delete $1.backpatcher;
		$$.index = 0;
	}
	| postfix_expression '[' expression ']' {
		std::string s = std::string($1.type);
		$$.name = $1.name;
		//std::cout<<s<<"\n";
		
		if(s.back() == '*'){
			s.pop_back();
			$$.type = strdup(s.c_str());
		}
		else{
			yyerror("dereferencing by [ ] invalid");
		}


		
		int stars = 0;
		for(int i=s.size()-1 ; i>=0;i--){
			if(s[i] == '*')stars++;
		}
		vector<int> dim = st.lookup($1.name)->dimensions;
		string typ = st.lookup($1.name)->type;
		$$.ir.code = strdup(irgen.concatenate(string($1.ir.code),string($3.ir.code)).c_str());


	
			//not the last case rn.
			
		int index = dim.size()-stars;
		//get the index of the next multiplier constant
		//cout <<s <<"\n";
		
		
		
		int ptr_cnt = 0;
		for(int i=typ.size()-1 ; i>=0;i--){
			if(typ[i] == '*')ptr_cnt++;
		}
		//std::cout <<typ <<"\n";
		printf("%d %d %d \n",dim.size(), stars, ptr_cnt);
		ptr_cnt -= dim.size();
		//string s has bache hue type
		//ie stars
		
		if(stars < ptr_cnt){
			yyerror("cannot index a pointer");
		}
	
		
		
		printf("%s %s YOO\n",$1.ir.tmp, $1.name);
		//easier check is if tmp does not contain a $, since x#block1 and x are different here
		std::string g = $1.ir.tmp;
		
		if(g.find("$") == std::string::npos){
			//we are just starting array, set t = i*dim;
			string temp = irgen.new_temp();
			int n = st.getTypeSize(s);
			string ss = (stars > 0 ? std::to_string(dim[index]):std::to_string(st.getTypeSize(s)) );
			string cd1 = irgen.add_op(temp, $3.ir.tmp, "*", ss);
			if(stars > ptr_cnt)$$.ir.tmp = strdup(temp.c_str());
			else{
				string newtemp;
				newtemp += string($1.ir.tmp);
			newtemp += "[";
			newtemp += temp;
			newtemp += "]";
			$$.ir.tmp = strdup(newtemp.c_str());
			if(is_udt($1.type) == true){
				$$.ir.tmp = strdup(temp.c_str());
				
			}
			else {
				
				$$.ir.tmp = strdup(newtemp.c_str());
			}
			}
			
			$$.ir.code = strdup(irgen.concatenate(string($$.ir.code),cd1).c_str());
		}
		else{
			//also add previous temp
			// we do t' = (told + i)*dim
			string temp = irgen.new_temp();
			
			string cd1 = irgen.add_op(temp, $1.ir.tmp, "+" , $3.ir.tmp); //t1 = told + i
			std::cout <<cd1 <<"BROTHER\n";
			string temp2 = irgen.new_temp();
			string ss = (stars > 0 ? std::to_string(dim[index]):std::to_string(st.getTypeSize(s)) );
			string cd2 = irgen.add_op(temp2, temp, "*" , ss);
			cd1 += "\n";
			cd1 += cd2;
			$$.ir.code = strdup(irgen.concatenate(string($$.ir.code),cd1).c_str());
			if(stars > ptr_cnt)$$.ir.tmp = strdup(temp2.c_str());
			else{
				string newtemp;
				newtemp += string($1.name);
					int num;
		num = st.lookup(string($1.name))->block_num;
		newtemp += "#block";
		newtemp += to_string(num);
				newtemp += "[";
			newtemp += temp2;
			newtemp += "]";
			
			if(is_udt($1.type) == true){
				$$.ir.tmp = strdup(temp2.c_str());
				
			}
			else {
				
				$$.ir.tmp = strdup(newtemp.c_str());
			}
			
			}
		}
		
		$$.backpatcher = new BackPatcher();
		$$.index = 0;
	}
	| postfix_expression '(' ')'{
		$$.name = $1.name;
		if(isPROCEDURE($1.kind)){}
		else{
			yyerror("called object is not a function or function pointer");
		}
		if(eq($1.kind,"CONST")){
			yyerror("called object is not a function or function pointer");
		}
		if(eq($1.name,"printf")){
			yyerror("too few arguments to function \'printf\'");
		}
		else if(eq($1.name,"scanf")){
			yyerror("too few arguments to function \'scanf\'");
		}
		else{
			char* func_kind = strdup(st.lookup($1.name)->kind.c_str());
			char* to_check = extract_between_parentheses(func_kind);
			
			if(eq(to_check, "")){
			}
			else{
				//match prefix with variadic

				yyerror("too few arguments passed or type mismatch");
			}
		}
		$$.name = $1.name;
		string ext ="";
		int arg = 0;
		if(contains($1.name, ".") == true){
			//it is a class, pass obj as parameter
			arg++;
			string s = string($1.name);
			string pref;
			string suf;
			bool dot = false;
			for(int i=0;i<s.size();i++){
				if(s[i] == '.'){
					dot = true;
					continue;
				}
				if(dot == false)pref+=s[i];
				else suf+=s[i];
			}
			
			
			$1.name = strdup(suf.c_str());
			ext = irgen.add_par("&" + pref);
			
		}
		$$.kind = "PROCEDURE";
		$$.type = $1.type;

		string tmp = irgen.new_temp();
		string s = string($1.ir.code);
		s = irgen.concatenate(s,ext);
		string p = irgen.func_call(string($1.name), arg);
		if(eq($1.type,"VOID") == false)p = irgen.assign(tmp,p);
		s = irgen.concatenate(s,p);
		$$.ir.tmp = strdup(tmp.c_str());
		$$.backpatcher = new BackPatcher();
		$$.ir.code = strdup(s.c_str());
	$$.index = 0;
	}
	| postfix_expression '(' argument_expression_list ')'{
		
		$$.index = 0;
		if(isPROCEDURE($1.kind)){}
		else{
			yyerror("called object is not a function or function pointer");
		}
		if(eq($1.kind,"CONST")){
			yyerror("called object is not a function or function pointer");
		}

		if(eq($1.name,"printf")){
			//printf("\n\n%s\n\n",$3.type);
			if(is_first_arg_STRING($3.type)==1){
				// TO BE SEEN LATER;
			}
			else{
				yyerror("expected \'const char * restrict\' but argument is of different type");
			}
		}
		else if(eq($1.name,"scanf")){
			if(is_first_arg_STRING($3.type)==1){
				// TO BE SEEN LATER;
			}
			else{
				yyerror("expected \'const char * restrict\' but argument is of different type");
			}
		}
		else{
			char* func_kind = strdup(st.lookup($1.name)->kind.c_str());
			char* to_check = extract_between_parentheses(func_kind);
			if(validate_arguments(to_check, $3.type) == 1){
				
			}
			else{
				//match prefix with variadic

				yyerror("too few arguments passed or type mismatch");
			}
		}
		$$.name = $1.name;
		string ext ="";
		
		if(contains($1.name, ".") == true){
			//it is a class, pass obj as parameter
			$3.index++;
			string s = string($1.name);
			string pref;
			string suf;
			bool dot = false;
			for(int i=0;i<s.size();i++){
				if(s[i] == '.'){
					dot = true;
					continue;
				}
				if(dot == false)pref+=s[i];
				else suf+=s[i];
			}
			
			
			$1.name = strdup(suf.c_str());
			ext = irgen.add_par("&" + pref);
			
		}
		$$.kind = "PROCEDURE";
		$$.type = $1.type;
		
		$$.backpatcher = new BackPatcher();
		string tmp = irgen.new_temp();
		string s = string($1.ir.code);
		s = irgen.concatenate(s, string($3.ir.code));
		s = irgen.concatenate(s, string($3.ir.par));
		s = irgen.concatenate(s,ext);
		string p = irgen.func_call(string($1.name), $3.index);
		if(eq($1.type,"VOID") == false)p = irgen.assign(tmp,p);
		s = irgen.concatenate(s,p);
		$$.ir.tmp = strdup(tmp.c_str());
		$$.ir.code = strdup(s.c_str());
		
	}
	| postfix_expression '.' IDENTIFIER{

		
		//STRUCT TYPE CHECKING HANDLED
		std::string s = std::string($1.name) + "." + std::string($3.type);
		//printf("\n\n%s\n\n",$1.type);
		//std::cout<<s<<"\n";
		
		if(st.lookup(s) != nullptr){
			if(contains(strdup(st.lookup(s)->kind.c_str()),"PRIVATE")){
				yyerror("Cannot access the private member of the class");
			}
			else if(contains(strdup(st.lookup(s)->kind.c_str()),"PROTECTED")){
				yyerror("Cannot access the protected member of the class");
			}
			if(contains($1.type,"*")){
				yyerror("Pointers dont have members");
			}
			$$.type = strdup(st.lookup(s)->type.c_str());
			$$.kind = strdup(st.lookup(s)->kind.c_str());
			$$.name = strdup(s.c_str());
		}
		
		int offset = -1;
		string typ;
		
		//$3.type is just right wala
		//$1.type is type of p.ID as whole
		
		
		for (const auto& scope_ptr : st.scopes_) {
			if(scope_ptr->scope_name == string($1.type)){
				if(scope_ptr->symbol_map.count(string($3.type)) > 0){
					offset = scope_ptr->symbol_map[string($3.type)]->offset;
					typ = scope_ptr->symbol_map[string($3.type)]->type;
					//aagaya offset
					printf("%d \n",offset);
					break;
				}
			}
		}
		if(offset == -1){
			yyerror("could not find required id");
		}
		string puranaind = to_string($1.index);
		string t0 = irgen.new_temp();
		string off = std::to_string(offset);
		string cd12;
		std::string g = $1.ir.tmp;
		if(g.find('$') != std::string::npos)cd12 = irgen.add_op(t0, string($1.ir.tmp),"+",off);
		else cd12 = irgen.add_op(t0, puranaind,"+",off);
			
	

		$$.index = offset;
		$$.ir.code = strdup(irgen.concatenate(string($1.ir.code),cd12).c_str());
		if(is_udt(strdup(typ.c_str())) == true){
			//there is struct inside or any other except class inside
			$$.ir.tmp = strdup(t0.c_str());
		}
		else{
		string tem = string($1.name);
		
		int num;
		num = st.lookup($1.name)->block_num;
		
		tem += "#block";
		tem += to_string(num);
		tem += "[";
		tem += t0;
		tem += "]";
		$$.ir.tmp = strdup(tem.c_str());
		}
		
		$$.type = strdup(typ.c_str());
		if(contains($1.type, "class")){}
		else{
			$$.name = strdup($1.name);
		}
		
		
		$$.backpatcher = new BackPatcher();
	}
	| postfix_expression PTR_OP IDENTIFIER{
		$$.name = $1.name;
		//arrow operator, dereferencing then access
		//access ka do it like old
		yyerror("-> ooperator not allowed");
		$$.ir.code = strdup($1.ir.code);
		$$.backpatcher = new BackPatcher();
	}
	| postfix_expression INC_OP{
		$$.name = $1.name;
		lvalueError($1.kind);
		if(st.is_array($1.name)){
			yyerror("expression must be a modifiable lvalue");
		}
		if(contains($1.type,"CONST")){
			yyerror("Const value cannot be incremented");
		}
		if(contains($1.type,"struct") || contains($1.type,"union") || contains($1.type,"class") ){
			yyerror("wrong type argument to increment");
		}
		$$.type = $1.type;
		$$.kind = $1.kind;
		$$.index = 0;
		//a++
		std::string temp = irgen.new_temp();
		
		string s = irgen.assign(temp, $1.ir.tmp);
		string g = irgen.add_op(std::string($1.ir.tmp),std::string($1.ir.tmp), "+" , "1");
		string t = irgen.concatenate(s,g);
		$$.ir.code = strdup(irgen.concatenate(string($1.ir.code),t).c_str());
		$$.ir.tmp = strdup(temp.c_str());
		$$.backpatcher = new BackPatcher();
		
	}
	| postfix_expression DEC_OP{
		$$.name = $1.name;
		lvalueError($1.kind);
		if(st.is_array($1.name)){
			yyerror("expression must be a modifiable lvalue");
		}
		if(contains($1.type,"CONST")){
			yyerror("Const value cannot be decremented");
		}
		if(contains($1.type,"struct") || contains($1.type,"union") || contains($1.type,"class") ){
			yyerror("wrong type argument to decrement");
		}
		$$.type = $1.type;
		$$.kind = $1.kind;
		std::string temp = irgen.new_temp();
		string s = irgen.assign(temp, $1.ir.tmp);
		string g = irgen.add_op(std::string($1.ir.tmp),std::string($1.ir.tmp), "-" , "1");
		string t = irgen.concatenate(s,g);
		$$.ir.code = strdup(irgen.concatenate(string($1.ir.code),t).c_str());
		$$.ir.tmp = strdup(temp.c_str());
		$$.backpatcher = new BackPatcher();
		$$.index = 0;
	}
	;

argument_expression_list
	: assignment_expression{
		$$.type = $1.type;
		CONVERT_BOOL_EXPR_TO_VALUE($1);
		std::string new_temp_par = string($1.ir.tmp);
		if(new_temp_par.find("[") != std::string::npos){
			std::string new_tmp = irgen.new_temp();
			std::string asm_code = new_tmp + " = " + new_temp_par;
			new_temp_par = new_tmp;
			$1.ir.code = strdup(irgen.concatenate(string($1.ir.code), asm_code).c_str());
		}
		string s = irgen.add_par(new_temp_par);
		$$.ir.code = strdup($1.ir.code);
		$$.ir.par = strdup(s.c_str());
		$$.index = 1;
	}
	| argument_expression_list ',' assignment_expression{
		char* newtype = concat($1.type,$3.type);
		$$.type = newtype;
		CONVERT_BOOL_EXPR_TO_VALUE($3);
		std::string new_temp_par = string($3.ir.tmp);
		if(new_temp_par.find("[") != std::string::npos){
			std::string new_tmp = irgen.new_temp();
			std::string asm_code = new_tmp + " = " + new_temp_par;
			new_temp_par = new_tmp;
			$3.ir.code = strdup(irgen.concatenate(string($3.ir.code), asm_code).c_str());
		}
		string s = string($3.ir.code);
		string p = irgen.add_par(new_temp_par);
		$$.ir.code = strdup(irgen.concatenate(string($1.ir.code), s).c_str());
		$$.ir.par = strdup(irgen.concatenate(string($1.ir.par), p).c_str());
		$$.index = $1.index + 1;
	}
	;

unary_expression
	: postfix_expression {
		$$.type = $1.type;
		$$.kind = $1.kind;
		$$.name = $1.name;
		$$.ir.tmp = strdup($1.ir.tmp);
		$$.ir.code = strdup($1.ir.code);
		$$.backpatcher = BackPatcher::copy($1.backpatcher);
    	delete $1.backpatcher;
	}
	| INC_OP unary_expression{
		lvalueError($2.kind);
		if(st.is_array($2.name)){
			yyerror("expression must be a modifiable lvalue");
		}
		if(contains($2.type,"CONST")){
			yyerror("Const value cannot be incremented");
		}
		if(contains($2.type,"struct") || contains($2.type,"union") || contains($2.type,"class") ){
			yyerror("wrong type argument to increment");
		}
		$$.type = $2.type;
		$$.kind = $2.kind;	
		string s = irgen.new_temp();
		string g = irgen.add_op(s,std::string($2.ir.tmp), "+" , "1");
		//t' = t0 + 1
		$$.ir.tmp = strdup(s.c_str());
		$$.ir.code = strdup(g.c_str());
		$$.backpatcher = new BackPatcher();
	}
	| DEC_OP unary_expression{
		lvalueError($2.kind);
		if(st.is_array($2.name)){
			yyerror("expression must be a modifiable lvalue");
		}
		if(contains($2.type,"CONST")){
			yyerror("Const value cannot be decremented");
		}
		if(contains($2.type,"struct") || contains($2.type,"union") || contains($2.type,"class") ){
			yyerror("wrong type argument to decrement");
		}
		$$.type = $2.type;
		$$.kind = $2.kind;
		string s = irgen.new_temp();
		string g = irgen.add_op(s,std::string($2.ir.tmp), "-" , "1");
		//t' = t0 + 1
		$$.ir.tmp = strdup(s.c_str());
		$$.ir.code = strdup(g.c_str());
		$$.backpatcher = new BackPatcher();
	}
	| unary_operator cast_expression{
		//deref.

		
		char *ptr = "*";
		if(eq($1.type, ptr) == true){
			//we have to dereference
			std::string s = std::string($2.type);
			if(s.back() == '*'){
				s.pop_back();
			}
			else{
				yyerror("dereferencing not valid");
			}
			$$.type = strdup(s.c_str());
		}
		else{
		ptr = "&";
		if(eq($1.type,ptr) == true){
			//we have to convert T to T*
			std::string s = std::string($2.type);
			s.push_back('*');
			$$.type = strdup(s.c_str());
		}
		else{
			$$.type = $2.type;
		}
		}
		
		if(eq($1.type,"+") == true){
			$$.ir.code = strdup($2.ir.code);
			$$.ir.tmp = strdup($2.ir.tmp);
			$$.backpatcher = new BackPatcher();
		}
		else{
			$$.ir.tmp = strdup(irgen.new_temp().c_str());
			string s = irgen.add_unary(string($$.ir.tmp), string($1.type),string($2.ir.tmp));
			string fin = irgen.concatenate($2.ir.code , s);
			$$.ir.code = strdup(fin.c_str());
			if(eq($1.type,"!")){
				$$.backpatcher = new BackPatcher();
				$$.backpatcher->assignFalseList($2.backpatcher->getTrueList());
				$$.backpatcher->assignTrueList($2.backpatcher->getFalseList());
			}
			else{
				$$.backpatcher = new BackPatcher();
			}
		}
		
		$$.name = $2.name;
		$$.kind = $2.kind;

	}
	| SIZEOF unary_expression{
		$$.type = "INT";
		$$.kind = "CONST";
		$$.backpatcher = new BackPatcher();
		$$.name = "sizeof";
		$$.ir.code = "";
		int sz = st.getTypeSize(string($2.type));
		string tem = to_string(sz);
		$$.ir.tmp = strdup(tem.c_str());
		
		
	}
	| SIZEOF '(' type_name ')'{
		$$.type = "INT";
		$$.kind = "CONST";
		$$.name = "sizeof";
		int sz = st.getTypeSize(string($3.type));
		string tem = to_string(sz);
		$$.ir.tmp = strdup(tem.c_str());
		$$.ir.code = "";
		$$.backpatcher = new BackPatcher();
	}
	;

unary_operator
	: '&' {
		$$.type = "&";
	}
	| '*' {
		$$.type = "*";
	}
	| '+'{
		$$.type = "+";
	}
	| '-'{
		$$.type = "-";
	}
	| '~'{
		$$.type = "~";
	}
	| '!'{
		$$.type = "!";
	}
	;

cast_expression
	: unary_expression {
		$$.type = $1.type;
		$$.kind = $1.kind;
		$$.name = $1.name;
		$$.ir.tmp = strdup($1.ir.tmp);
		$$.ir.code = strdup($1.ir.code);
		$$.backpatcher = BackPatcher::copy($1.backpatcher);
    	delete $1.backpatcher;
		
	}
	| '(' type_name ')' cast_expression{
		//explicit typecast
		if(!ts.contains(std::string($2.type))){
			yyerror("the following type is not a valid type");
		}
		else if(contains($2.type,"enum")){
			yyerror("the following type-conversion is not a valid type");
		}
		//printf("\n\n%s\n\n",$2.type);
		//NEW_TMP_NAME = CAST: NEW_TYPE OLD_TMP
		string newtmp = irgen.new_temp();
		string newcd = irgen.typecast(newtmp, $4.ir.tmp, $4.type,$2.type);
		$$.name = $4.name;
		$$.type = $2.type;
		$$.kind = $4.kind;
		$$.ir.tmp = strdup(newtmp.c_str());
		$$.ir.code = strdup(irgen.concatenate($4.ir.code,newcd).c_str());
		$$.backpatcher = new BackPatcher();
	}
	| '(' type_name error cast_expression { yyerrok; }
	;

multiplicative_expression
	: cast_expression {
		$$.type = $1.type;
		$$.kind = $1.kind;
		$$.name = $1.name;
		$$.ir.tmp = strdup($1.ir.tmp);
		$$.ir.code = strdup($1.ir.code);
		$$.backpatcher = BackPatcher::copy($1.backpatcher);
    	delete $1.backpatcher;
	}
	| multiplicative_expression '*' cast_expression {
		// Type Checking
		std::string type1,type2 ;
		CHECK_PROCEDURE_AND_CLEAN_TYPE($1,$3);
		
		// Checking for pointer as '*' in pointers not valid 
		if(ts.hasPointer(type1) || ts.hasPointer(type2)){
			yyerror("invalid operator to pointers");
		}
		//check_type($1.type, $3.type, "incompatible type expression involved in *: ");
		
		// IR
		CONVERT_BOOL_EXPR_TO_VALUE($1)
		CONVERT_BOOL_EXPR_TO_VALUE($3)

		$$.ir.tmp = strdup(irgen.new_temp().c_str());
		$$.ir.code = strdup(irgen.concatenate(string($1.ir.code),string($3.ir.code)).c_str());

		std::string temp1 = string($1.ir.tmp);
		std::string temp2 = string($3.ir.tmp);
		if(temp1.find("[") != std::string::npos){
			std::string new_temp1 = irgen.new_temp();
			std::string assignment_ = new_temp1 + " = " + temp1;
			temp1 = new_temp1;
			$$.ir.code = strdup(irgen.concatenate(string($$.ir.code),assignment_).c_str());
		}
		if(temp2.find("[") != std::string::npos){
			std::string new_temp1 = irgen.new_temp();
			std::string assignment_ = new_temp1 + " = " + temp2;
			temp2 = new_temp1;
			$$.ir.code = strdup(irgen.concatenate(string($$.ir.code),assignment_).c_str());
		}

		string s = irgen.add_op(string($$.ir.tmp), temp1, string("*"), temp2);
		$$.ir.code =  strdup(irgen.concatenate(string($$.ir.code), s).c_str());
		$$.backpatcher = new BackPatcher();
	}
	| multiplicative_expression '/' cast_expression{
		// Type Checking
		std::string type1,type2 ;
		CHECK_PROCEDURE_AND_CLEAN_TYPE($1,$3);

		// Checking for pointer as '/' in pointers not valid 
		if(ts.hasPointer(type1) || ts.hasPointer(type2)){
			yyerror("invalid operator to pointers");
		}
		check_type($1.type, $3.type, "incompatible type expression involved in *: ");

		// IR
		CONVERT_BOOL_EXPR_TO_VALUE($1)
		CONVERT_BOOL_EXPR_TO_VALUE($3)

		
		$$.ir.tmp = strdup(irgen.new_temp().c_str());
		$$.ir.code = strdup(irgen.concatenate(string($1.ir.code),string($3.ir.code)).c_str());

		std::string temp1 = string($1.ir.tmp);
		std::string temp2 = string($3.ir.tmp);
		if(temp1.find("[") != std::string::npos){
			std::string new_temp1 = irgen.new_temp();
			std::string assignment_ = new_temp1 + " = " + temp1;
			temp1 = new_temp1;
			$$.ir.code = strdup(irgen.concatenate(string($$.ir.code),assignment_).c_str());
		}
		if(temp2.find("[") != std::string::npos){
			std::string new_temp1 = irgen.new_temp();
			std::string assignment_ = new_temp1 + " = " + temp2;
			temp2 = new_temp1;
			$$.ir.code = strdup(irgen.concatenate(string($$.ir.code),assignment_).c_str());
		}

		string s = irgen.add_op(string($$.ir.tmp), temp1, string("/"), temp2);
		$$.ir.code =  strdup(irgen.concatenate(string($$.ir.code), s).c_str());
		$$.backpatcher = new BackPatcher();
	}
	| multiplicative_expression '%' cast_expression{
		// Type Checking
		std::string type1,type2 ;
		CHECK_PROCEDURE_AND_CLEAN_TYPE($1,$3);

		// Checking for pointer as '/' in pointers not valid 
		if(ts.hasPointer(type1) || ts.hasPointer(type2)){
			yyerror("invalid operator to pointers");
		}
		if(eq($1.type, "INT") == false || eq($3.type, "INT")==false){
			char *err = "both operands must be int, int but are ";
        	err = concat(err,$1.type);
        	err = concat(err,  $3.type);
        	yyerror(err);
		}
		check_type($1.type, $3.type, "incompatible type expression involved in *: ");

		// IR
		CONVERT_BOOL_EXPR_TO_VALUE($1)
		CONVERT_BOOL_EXPR_TO_VALUE($3)
		
		$$.ir.tmp = strdup(irgen.new_temp().c_str());
		$$.ir.code = strdup(irgen.concatenate(string($1.ir.code),string($3.ir.code)).c_str());

		std::string temp1 = string($1.ir.tmp);
		std::string temp2 = string($3.ir.tmp);
		if(temp1.find("[") != std::string::npos){
			std::string new_temp1 = irgen.new_temp();
			std::string assignment_ = new_temp1 + " = " + temp1;
			temp1 = new_temp1;
			$$.ir.code = strdup(irgen.concatenate(string($$.ir.code),assignment_).c_str());
		}
		if(temp2.find("[") != std::string::npos){
			std::string new_temp1 = irgen.new_temp();
			std::string assignment_ = new_temp1 + " = " + temp2;
			temp2 = new_temp1;
			$$.ir.code = strdup(irgen.concatenate(string($$.ir.code),assignment_).c_str());
		}

		string s = irgen.add_op(string($$.ir.tmp), temp1, string("%"), temp2);
		$$.ir.code =  strdup(irgen.concatenate(string($$.ir.code), s).c_str());
		$$.backpatcher = new BackPatcher();
	}
	;

additive_expression
	: multiplicative_expression {
		$$.type = $1.type;
		$$.kind = $1.kind;
		$$.name = $1.name;
		$$.ir.tmp = strdup($1.ir.tmp);
		$$.ir.code = strdup($1.ir.code);
		$$.backpatcher = BackPatcher::copy($1.backpatcher);
    	delete $1.backpatcher;
	}
	| additive_expression '+' multiplicative_expression{
		// Type Checking
		std::string type1,type2 ;
		CHECK_PROCEDURE_AND_CLEAN_TYPE($1,$3);

		// In case of '+' one pointer and other int is allowed
		if(ts.hasPointer(type1) && ts.hasPointer(type2)){
			yyerror("invalid operator to pointers");
		}
		else if(ts.hasPointer(type1)){
			if(!(type2=="INT")){
				yyerror("invalid operands to binary");
			}
			else{
				$$.type = strdup(type1.c_str());
			}
		}
		else if(ts.hasPointer(type2)){
			if(!(type1=="INT")){
				yyerror("invalid operands to binary");
			}
			else{
				$$.type = strdup(type2.c_str());
			}
		}
		else{
			check_type($1.type, $3.type, "incompatible type expression involved in +: ");
			$$.type = $1.type; 
		}

		// IR
		CONVERT_BOOL_EXPR_TO_VALUE($1)
		CONVERT_BOOL_EXPR_TO_VALUE($3)
		
		$$.ir.tmp = strdup(irgen.new_temp().c_str());
		$$.ir.code = strdup(irgen.concatenate(string($1.ir.code),string($3.ir.code)).c_str());
		std::string temp1 = string($1.ir.tmp);
		std::string temp2 = string($3.ir.tmp);
		if(temp1.find("[") != std::string::npos){
			std::string new_temp1 = irgen.new_temp();
			std::string assignment_ = new_temp1 + " = " + temp1;
			temp1 = new_temp1;
			$$.ir.code = strdup(irgen.concatenate(string($$.ir.code),assignment_).c_str());
		}
		if(temp2.find("[") != std::string::npos){
			std::string new_temp1 = irgen.new_temp();
			std::string assignment_ = new_temp1 + " = " + temp2;
			temp2 = new_temp1;
			$$.ir.code = strdup(irgen.concatenate(string($$.ir.code),assignment_).c_str());
		}

		string s = irgen.add_op(string($$.ir.tmp), temp1, string("+"), temp2);
		$$.ir.code =  strdup(irgen.concatenate(string($$.ir.code), s).c_str());
		$$.backpatcher = new BackPatcher();
	}
	| additive_expression '-' multiplicative_expression{
		// Type Checking
		std::string type1,type2 ;
		CHECK_PROCEDURE_AND_CLEAN_TYPE($1,$3);

		// In case of '+' one pointer and other int is allowed
		if(ts.hasPointer(type1) && ts.hasPointer(type2)){
			yyerror("invalid operator to pointers");
		}
		else if(ts.hasPointer(type1)){
			if(!(type2=="INT")){
				yyerror("invalid operands to binary");
			}
			else{
				$$.type = strdup(type1.c_str());
			}
		}
		else if(ts.hasPointer(type2)){
			if(!(type1=="INT")){
				yyerror("invalid operands to binary");
			}
			else{
				$$.type = strdup(type2.c_str());
			}
		}
		else{
			check_type($1.type, $3.type, "incompatible type expression involved in +: ");
			$$.type = $1.type; 
		}

		// IR
		CONVERT_BOOL_EXPR_TO_VALUE($1)
		CONVERT_BOOL_EXPR_TO_VALUE($3)
		
		$$.ir.tmp = strdup(irgen.new_temp().c_str());
		$$.ir.code = strdup(irgen.concatenate(string($1.ir.code),string($3.ir.code)).c_str());

		std::string temp1 = string($1.ir.tmp);
		std::string temp2 = string($3.ir.tmp);
		if(temp1.find("[") != std::string::npos){
			std::string new_temp1 = irgen.new_temp();
			std::string assignment_ = new_temp1 + " = " + temp1;
			temp1 = new_temp1;
			$$.ir.code = strdup(irgen.concatenate(string($$.ir.code),assignment_).c_str());
		}
		if(temp2.find("[") != std::string::npos){
			std::string new_temp1 = irgen.new_temp();
			std::string assignment_ = new_temp1 + " = " + temp2;
			temp2 = new_temp1;
			$$.ir.code = strdup(irgen.concatenate(string($$.ir.code),assignment_).c_str());
		}

		string s = irgen.add_op(string($$.ir.tmp), temp1, string("-"), temp2);
		$$.ir.code =  strdup(irgen.concatenate(string($$.ir.code), s).c_str());
		$$.backpatcher = new BackPatcher();

	}
	;

shift_expression
	: additive_expression {
		$$.type = $1.type;
		$$.kind = $1.kind;
		$$.name = $1.name;
		$$.ir.code = strdup($1.ir.code);
		$$.ir.tmp = strdup($1.ir.tmp);
		$$.backpatcher = BackPatcher::copy($1.backpatcher);
    	delete $1.backpatcher;
	}
	| shift_expression LEFT_OP additive_expression{
		// Type Checking
		std::string type1,type2 ;
		CHECK_PROCEDURE_AND_CLEAN_TYPE($1,$3);

		if(eq($1.type, "INT") == false || eq($3.type, "INT")==false){
			char *err = "both operands must be int, int but are ";
        	err = concat(err,$1.type);
        	err = concat(err,  $3.type);
        	yyerror(err);
		}
		$$.type = "INT";

		// IR
		CONVERT_BOOL_EXPR_TO_VALUE($1)
		CONVERT_BOOL_EXPR_TO_VALUE($3)

		$$.ir.tmp = strdup(irgen.new_temp().c_str());
		string s = irgen.add_op(string($$.ir.tmp), string($1.ir.tmp), string("<<"), string($3.ir.tmp));
		$$.ir.code = strdup(irgen.concatenate(string($1.ir.code),string($3.ir.code)).c_str());
		$$.ir.code =  strdup(irgen.concatenate(string($$.ir.code), s).c_str());
		$$.backpatcher = new BackPatcher();
	}
	| shift_expression RIGHT_OP additive_expression{
		// Type Checking
		std::string type1,type2 ;
		CHECK_PROCEDURE_AND_CLEAN_TYPE($1,$3);

		if(eq($1.type, "INT") == false || eq($3.type, "INT")==false){
			char *err = "both operands must be int, int but are ";
        	err = concat(err,$1.type);
        	err = concat(err, $3.type );
        yyerror(err);
		}
		$$.type = "INT";

		// IR
		CONVERT_BOOL_EXPR_TO_VALUE($1)
		CONVERT_BOOL_EXPR_TO_VALUE($3)

		$$.ir.tmp = strdup(irgen.new_temp().c_str());
		string s = irgen.add_op(string($$.ir.tmp), string($1.ir.tmp), string(">>"), string($3.ir.tmp));
		$$.ir.code = strdup(irgen.concatenate(string($1.ir.code),string($3.ir.code)).c_str());
		$$.ir.code =  strdup(irgen.concatenate(string($$.ir.code), s).c_str());
		$$.backpatcher = new BackPatcher();
	}
	;

relational_expression
	: shift_expression {
		$$.type = $1.type;
		$$.kind = $1.kind;
		$$.name = $1.name;
		$$.ir.tmp = strdup($1.ir.tmp);
		$$.ir.code = strdup($1.ir.code);
		$$.backpatcher = BackPatcher::copy($1.backpatcher);
    	delete $1.backpatcher;
	}
	| relational_expression '<'  shift_expression{
		// Type Checking
		std::string type1,type2 ;
		CHECK_PROCEDURE_AND_CLEAN_TYPE($1,$3);

		check_type($1.type, $3.type,"incompatible type expression involved in < : ");
		$$.type = "INT";

		// IR
		$$.ir.tmp = strdup(irgen.new_temp().c_str());
		string s = irgen.add_op(string($$.ir.tmp), string($1.ir.tmp), string("<"), string($3.ir.tmp));
		$$.ir.code = strdup(irgen.concatenate(string($1.ir.code),string($3.ir.code)).c_str());
		$$.ir.code =  strdup(irgen.concatenate(string($$.ir.code), s).c_str());
		std::string tmp1 = irgen.new_tmp_label();
		std::string tmp2 = irgen.new_tmp_label();
		std::string conditional = irgen.create_conditional_jump(std::string($$.ir.tmp),tmp1,tmp2);
		$$.ir.code = strdup(irgen.concatenate(string($$.ir.code),conditional).c_str());
		$$.backpatcher = new BackPatcher();
		$$.backpatcher->addToTrueList(tmp1);
		$$.backpatcher->addToFalseList(tmp2);

	}
	| relational_expression '>' shift_expression{
		// Type Checking
		std::string type1,type2 ;
		CHECK_PROCEDURE_AND_CLEAN_TYPE($1,$3);

		check_type($1.type, $3.type,"incompatible type expression involved in >: ");
		$$.type = "INT";

		// IR
		$$.ir.tmp = strdup(irgen.new_temp().c_str());
		string s = irgen.add_op(string($$.ir.tmp), string($1.ir.tmp), string(">"), string($3.ir.tmp));
		$$.ir.code = strdup(irgen.concatenate(string($1.ir.code),string($3.ir.code)).c_str());
		//std::cout<<std::string($$.ir.code)<<std::endl;
		$$.ir.code =  strdup(irgen.concatenate(string($$.ir.code), s).c_str());
		//std::cout<<std::string($$.ir.code)<<std::endl;
		std::string tmp1 = irgen.new_tmp_label();
		std::string tmp2 = irgen.new_tmp_label();
		std::string conditional = irgen.create_conditional_jump(std::string($$.ir.tmp),tmp1,tmp2);
		$$.ir.code = strdup(irgen.concatenate(string($$.ir.code),conditional).c_str());
		$$.backpatcher = new BackPatcher();
		$$.backpatcher->addToTrueList(tmp1);
		$$.backpatcher->addToFalseList(tmp2);
	}
	| relational_expression LE_OP shift_expression{
		// Type Checking
		std::string type1,type2 ;
		CHECK_PROCEDURE_AND_CLEAN_TYPE($1,$3);

		check_type($1.type, $3.type,"incompatible type expression involved in <=: ");
		$$.type = "INT";

		// IR
		$$.ir.tmp = strdup(irgen.new_temp().c_str());
		string s = irgen.add_op(string($$.ir.tmp), string($1.ir.tmp), string("<="), string($3.ir.tmp));
		$$.ir.code = strdup(irgen.concatenate(string($1.ir.code),string($3.ir.code)).c_str());
		$$.ir.code =  strdup(irgen.concatenate(string($$.ir.code), s).c_str());
		std::string tmp1 = irgen.new_tmp_label();
		std::string tmp2 = irgen.new_tmp_label();
		std::string conditional = irgen.create_conditional_jump(std::string($$.ir.tmp),tmp1,tmp2);
		$$.ir.code = strdup(irgen.concatenate(string($$.ir.code),conditional).c_str());
		$$.backpatcher = new BackPatcher();
		$$.backpatcher->addToTrueList(tmp1);
		$$.backpatcher->addToFalseList(tmp2);
	}
	| relational_expression GE_OP shift_expression{
		// Type Checking
		std::string type1,type2 ;
		CHECK_PROCEDURE_AND_CLEAN_TYPE($1,$3);

		check_type($1.type, $3.type,"incompatible type expression involved in >=: ");
		$$.type = "INT";

		// IR
		$$.ir.tmp = strdup(irgen.new_temp().c_str());
		string s = irgen.add_op(string($$.ir.tmp), string($1.ir.tmp), string(">="), string($3.ir.tmp));
		$$.ir.code = strdup(irgen.concatenate(string($1.ir.code),string($3.ir.code)).c_str());
		$$.ir.code =  strdup(irgen.concatenate(string($$.ir.code), s).c_str());
		std::string tmp1 = irgen.new_tmp_label();
		std::string tmp2 = irgen.new_tmp_label();
		std::string conditional = irgen.create_conditional_jump(std::string($$.ir.tmp),tmp1,tmp2);
		$$.ir.code = strdup(irgen.concatenate(string($$.ir.code),conditional).c_str());
		$$.backpatcher = new BackPatcher();
		$$.backpatcher->addToTrueList(tmp1);
		$$.backpatcher->addToFalseList(tmp2);
	}
	;

equality_expression
	: relational_expression {
		$$.type = $1.type;
		$$.kind = $1.kind;
		$$.ir.tmp = strdup($1.ir.tmp);
		$$.name = $1.name;
		$$.ir.code = strdup($1.ir.code);
		$$.backpatcher = BackPatcher::copy($1.backpatcher);
    	delete $1.backpatcher;
	}
	| equality_expression EQ_OP relational_expression{
		// Type Checking
		std::string type1,type2 ;
		CHECK_PROCEDURE_AND_CLEAN_TYPE($1,$3);

		check_type($1.type, $3.type, "incompatible type expression involved in = : ");
		$$.type = "INT";

		// IR
		$$.ir.tmp = strdup(irgen.new_temp().c_str());
		string s = irgen.add_op(string($$.ir.tmp), string($1.ir.tmp), string("=="), string($3.ir.tmp));
		$$.ir.code = strdup(irgen.concatenate(string($1.ir.code),string($3.ir.code)).c_str());
		$$.ir.code =  strdup(irgen.concatenate(string($$.ir.code), s).c_str());
		std::string tmp1 = irgen.new_tmp_label();
		std::string tmp2 = irgen.new_tmp_label();
		std::string conditional = irgen.create_conditional_jump(std::string($$.ir.tmp),tmp1,tmp2);
		$$.ir.code = strdup(irgen.concatenate(string($$.ir.code),conditional).c_str());
		$$.backpatcher = new BackPatcher();
		$$.backpatcher->addToTrueList(tmp1);
		$$.backpatcher->addToFalseList(tmp2);
	}
	| equality_expression NE_OP relational_expression{
		// Type Checking
		std::string type1,type2 ;
		CHECK_PROCEDURE_AND_CLEAN_TYPE($1,$3);

		check_type($1.type, $3.type, "incompatible type expression involved in != : ");
		$$.type = "INT";

		// IR
		$$.ir.tmp = strdup(irgen.new_temp().c_str());
		string s = irgen.add_op(string($$.ir.tmp), string($1.ir.tmp), string("!="), string($3.ir.tmp));
		$$.ir.code = strdup(irgen.concatenate(string($1.ir.code),string($3.ir.code)).c_str());
		$$.ir.code =  strdup(irgen.concatenate(string($$.ir.code), s).c_str());
		std::string tmp1 = irgen.new_tmp_label();
		std::string tmp2 = irgen.new_tmp_label();
		std::string conditional = irgen.create_conditional_jump(std::string($$.ir.tmp),tmp1,tmp2);
		$$.ir.code = strdup(irgen.concatenate(string($$.ir.code),conditional).c_str());
		$$.backpatcher = new BackPatcher();
		$$.backpatcher->addToTrueList(tmp1);
		$$.backpatcher->addToFalseList(tmp2);
	}
	;

and_expression
	: equality_expression {
		$$.type = $1.type;
		$$.kind = $1.kind;
		$$.name = $1.name;
		$$.ir.tmp = strdup($1.ir.tmp);
		$$.ir.code = strdup($1.ir.code);
		$$.backpatcher = BackPatcher::copy($1.backpatcher);
    	delete $1.backpatcher;
	}
	| and_expression '&' equality_expression {
		// Type Checking
		std::string type1,type2 ;
		CHECK_PROCEDURE_AND_CLEAN_TYPE($1,$3);

		// Checking for pointer as '&' in pointers not valid 
		if(ts.hasPointer(type1) || ts.hasPointer(type2)){
			yyerror("invalid operator to pointers");
		}

		if(eq($1.type, "INT") == false || eq($3.type, "INT")==false){
			char *err = "both operands in & must be int, int but are ";
        	err = concat(err,$1.type);
        	err = concat(err, $3.type );
        	yyerror(err);
		}
		$$.type = "INT";

		// IR
		CONVERT_BOOL_EXPR_TO_VALUE($1)
		CONVERT_BOOL_EXPR_TO_VALUE($3)
		
		$$.ir.tmp = strdup(irgen.new_temp().c_str());
		string s = irgen.add_op(string($$.ir.tmp), string($1.ir.tmp), string("&"), string($3.ir.tmp));
		$$.ir.code = strdup(irgen.concatenate(string($1.ir.code),string($3.ir.code)).c_str());
		$$.ir.code =  strdup(irgen.concatenate(string($$.ir.code), s).c_str());
		$$.backpatcher = new BackPatcher();
	}
	;

exclusive_or_expression
	: and_expression {
		$$.type = $1.type;
		$$.kind = $1.kind;
		$$.name = $1.name;
		$$.ir.tmp = strdup($1.ir.tmp);
		$$.ir.code = strdup($1.ir.code);
		$$.backpatcher = BackPatcher::copy($1.backpatcher);
    	delete $1.backpatcher;
	}
	| exclusive_or_expression '^' and_expression{
		// Type Checking
		std::string type1,type2 ;
		CHECK_PROCEDURE_AND_CLEAN_TYPE($1,$3);

		// Checking for pointer as '^' in pointers not valid 
		if(ts.hasPointer(type1) || ts.hasPointer(type2)){
			yyerror("invalid operator to pointers");
		}

		if(eq($1.type, "INT") == false || eq($3.type, "INT")==false){
			char *err = "both operands in ^ must be int, int but are ";
        	err = concat(err,$1.type);
        	err = concat(err, $3.type );
        	yyerror(err);
		}
		$$.type = "INT";

		// IR
		CONVERT_BOOL_EXPR_TO_VALUE($1)
		CONVERT_BOOL_EXPR_TO_VALUE($3)
		
		$$.ir.tmp = strdup(irgen.new_temp().c_str());
		string s = irgen.add_op(string($$.ir.tmp), string($1.ir.tmp), string("^"), string($3.ir.tmp));
		$$.ir.code = strdup(irgen.concatenate(string($1.ir.code),string($3.ir.code)).c_str());
		$$.ir.code =  strdup(irgen.concatenate(string($$.ir.code), s).c_str());
		$$.backpatcher = new BackPatcher();
	}
	;

inclusive_or_expression
	: exclusive_or_expression {
		$$.type = $1.type;
		$$.kind = $1.kind;
		$$.name = $1.name;
		$$.ir.tmp = strdup($1.ir.tmp);
		$$.ir.code = strdup($1.ir.code);
		$$.backpatcher = BackPatcher::copy($1.backpatcher);
    	delete $1.backpatcher;
	}
	| inclusive_or_expression '|' exclusive_or_expression{
		// Type Checking
		std::string type1,type2 ;
		CHECK_PROCEDURE_AND_CLEAN_TYPE($1,$3);

		// Checking for pointer as '|' in pointers not valid 
		if(ts.hasPointer(type1) || ts.hasPointer(type2)){
			yyerror("invalid operator to pointers");
		}

		if(eq($1.type, "INT") == false || eq($3.type, "INT")==false){
			char *err = "both operands in | must be int, int but are ";
        	err = concat(err,$1.type);
        	err = concat(err, $3.type );
        	yyerror(err);
		}
		$$.type = "INT";

		// IR
		CONVERT_BOOL_EXPR_TO_VALUE($1)
		CONVERT_BOOL_EXPR_TO_VALUE($3)
		
		$$.ir.tmp = strdup(irgen.new_temp().c_str());
		string s = irgen.add_op(string($$.ir.tmp), string($1.ir.tmp), string("|"), string($3.ir.tmp));
		$$.ir.code = strdup(irgen.concatenate(string($1.ir.code),string($3.ir.code)).c_str());
		$$.ir.code =  strdup(irgen.concatenate(string($$.ir.code), s).c_str());
		$$.backpatcher = new BackPatcher();
	}
	;

logical_and_expression
	: inclusive_or_expression {
		$$.type = $1.type;
		$$.kind = $1.kind;
		$$.name = $1.name;
		$$.ir.tmp = strdup($1.ir.tmp);
		$$.ir.code = strdup($1.ir.code);
		$$.backpatcher = BackPatcher::copy($1.backpatcher);
    	delete $1.backpatcher;
	}
	| logical_and_expression AND_OP  inclusive_or_expression{
		// check for constant expression
		HANDLE_BOOL_EXPR_BACKPATCH_FOR_CONSTANTS($1,irgen);
		std::string backpatch_label = irgen.new_label();
		std::string S_next = irgen.add_label(backpatch_label);
		std::string newIR = $1.backpatcher->backPatchTrueList(std::string($1.ir.code),backpatch_label);
		$1.ir.code = strdup(newIR.c_str());
		$1.ir.code = strdup(irgen.concatenate(string($1.ir.code), S_next).c_str());


		// check for constant expression
		HANDLE_BOOL_EXPR_BACKPATCH_FOR_CONSTANTS($3,irgen);
		std::string type1,type2 ;
		CHECK_PROCEDURE_AND_CLEAN_TYPE($1,$3);
		check_type($1.type, $3.type,"incompatible type expression involved in &&: ");
		$$.type = "INT";
		// no need of the commented
		//$$.ir.tmp = strdup(irgen.new_temp().c_str());
		//string s = irgen.add_op(string($$.ir.tmp), string($1.ir.tmp), string("&&"), string($3.ir.tmp));
		$$.ir.code = strdup(irgen.concatenate(string($1.ir.code),string($3.ir.code)).c_str());
		//$$.ir.code =  strdup(irgen.concatenate(string($$.ir.code), s).c_str());
		$$.backpatcher = new BackPatcher();
		$$.backpatcher->assignTrueList($3.backpatcher->getTrueList()) ; 
		$$.backpatcher->assignFalseList($$.backpatcher->merge($1.backpatcher->getFalseList(),$3.backpatcher->getFalseList()));

	}
	;

logical_or_expression
	: logical_and_expression {
		$$.type = $1.type;
		$$.kind = $1.kind;
		$$.name = $1.name;
		$$.ir.tmp = strdup($1.ir.tmp);
		$$.ir.code = strdup($1.ir.code);
		$$.backpatcher = BackPatcher::copy($1.backpatcher);
    	delete $1.backpatcher;
	}
	| logical_or_expression OR_OP logical_and_expression{
		// check for constant expression
		HANDLE_BOOL_EXPR_BACKPATCH_FOR_CONSTANTS($1,irgen);
		std::string backpatch_label = irgen.new_label();
		std::string S_next = irgen.add_label(backpatch_label);
		std::string newIR = $1.backpatcher->backPatchFalseList(std::string($1.ir.code),backpatch_label);
		$1.ir.code = strdup(newIR.c_str());
		$1.ir.code = strdup(irgen.concatenate(string($1.ir.code), S_next).c_str());


		// check for constant expression
		HANDLE_BOOL_EXPR_BACKPATCH_FOR_CONSTANTS($3,irgen);
		std::string type1,type2 ;
		CHECK_PROCEDURE_AND_CLEAN_TYPE($1,$3);
		check_type($1.type, $3.type, "incompatible type expression involved in || = : ");

		$$.type = "INT";
		//$$.ir.tmp = strdup(irgen.new_temp().c_str());
		//string s = irgen.add_op(string($$.ir.tmp), string($1.ir.tmp), string("||"), string($3.ir.tmp));
		$$.ir.code = strdup(irgen.concatenate(string($1.ir.code),string($3.ir.code)).c_str());
		//$$.ir.code =  strdup(irgen.concatenate(string($$.ir.code), s).c_str());
		$$.backpatcher = new BackPatcher();
		$$.backpatcher->assignFalseList($3.backpatcher->getFalseList()) ; 
		$$.backpatcher->assignTrueList($$.backpatcher->merge($1.backpatcher->getTrueList(),$3.backpatcher->getTrueList()));
	}
	;

conditional_expression
	: logical_or_expression {
		$$.type = $1.type;
		$$.kind = $1.kind;
		$$.ir.tmp = strdup($1.ir.tmp);
		$$.name = $1.name;
		$$.ir.code = strdup($1.ir.code);
		$$.backpatcher = BackPatcher::copy($1.backpatcher);
		
    	delete $1.backpatcher;
	}
	| logical_or_expression '?' expression ':' conditional_expression 
	;

assignment_expression
	: conditional_expression{
		$$.type = $1.type;
		$$.kind = $1.kind;
		$$.name = $1.name;
		$$.ir.tmp = strdup($1.ir.tmp);
		$$.ir.code = strdup($1.ir.code);
		$$.backpatcher = BackPatcher::copy($1.backpatcher);
    	delete $1.backpatcher;
	}
	| unary_expression assignment_operator assignment_expression {
		// the left of the declarator muse be lvalue;
		lvalueError($1.kind);
		
		// If a procedure then must be called procedure
		if(isPROCEDURE($3.kind)){yyerror("Cannot assign function to a variable");}

		// Removing const and static
		std::string type2 = ts.prepareString(std::string($3.type));
		$3.type = strdup(type2.c_str());
		// Removing static
		char* original = $1.type;
		std::string s = ts.removeStaticFromDeclaration(std::string($1.type));
		$1.type = strdup(s.c_str());

		std::string type_change_statement = "";
			
					int stars = 0;
					
					std::string typ = $3.type;
					int ptr_cnt = 0;
					std::string tot;
					int index = 0;
					if(st.lookup($3.name) != nullptr){
						tot = st.lookup($3.name)->type;
						vector<int> dim = st.lookup($3.name)->dimensions;
						index = dim.size();
						for(int i=tot.size()-1;i>=0;i--){
							if(tot[i] == '*')ptr_cnt++;
						}
					}
					
					for(int i=typ.size()-1;i>=0;i--){
						if(typ[i] == '*')stars++;
					}
					
					
		
		
		//get the index of the next multiplier constant
		//cout <<s <<"\n";
		//std::cout <<typ <<"\n";
		printf("%s %s \n %s %s \n %s %s \n CWAZY",$1.ir.tmp,$3.ir.tmp, $1.name,$3.name, $1.type, $3.type);
		printf("%d, %d, %d \n",stars,index, ptr_cnt);
		if(stars > ptr_cnt - index){
			//bt 
			yyerror("assignment involves array on right or has incorrent dereferencing");
		}
		if(eq($2.type,"&=")||eq($2.type,"^=")||eq($2.type,"|=") || eq($2.type,"<<=") || eq($2.type,">>=")){
			if(eq($1.type, "INT") == false || eq($3.type, "INT")==false){
				char *err = "both operands must be int, int in ";
				err = concat(err, $2.type);
				err = concat(err, "but are: ");
				err = concat(err,$1.type);
				err = concat(err, $3.type );
				yyerror(err);
			}
			else if(contains($1.type,"CONST")){
				yyerror("const variable cannot be re-assigned");
			}
		}
		else{
			// case if enum 
			if(contains($1.type,"enum")){
				
				if(contains($1.type,"CONST")){
					yyerror("const variable cannot be re-changed");
				}
				// ONLY INT TYPE VALUES CAN BE ASSIGNED 
				if(eq($3.kind, "ENUM_CONST") || eq($3.kind,"IDENTIFIER") || eq($3.kind, "CONST") ){
					if((eq($3.type,"INT") || eq($1.type,$3.type)) == false){
						yyerror("incompatible types when assigning to type \'enum\'");
					}
				}
				else{
					yyerror("invalid assignment in enum");
				}
			}
			else if(eq($1.type,"auto")){
				st.lookup(std::string($1.name))->type = std::string($3.type);
				original = $3.type;
			}
			else if(contains($1.type,"CONST")){
				if(ts.hasPointer($1.type)){
					if(!eq($2.type,"=")){
						yyerror("invalid pointer operation");
					}
				}
				else{
					yyerror("const variable cannot be re-assigned");
				}
			}
			else{
				
				
				if(!isConvertible(std::string($1.type),std::string($3.type))){
					char* s1 = "incompatible type expression involved in";
					s1 = concat(s1,$2.type);
					s1 = concat(s1," : ");
					check_type($1.type, $3.type,s1);
				}
				else{
					if(!eq($1.type,$3.type)){
						type_change_statement = formatTypeChange(type2,s);
					}
				}
				
			}
			
		}
		$$.ir.code = strdup(irgen.concatenate(string($1.ir.code),string($3.ir.code)).c_str());
		string tem;
		std::string new_temp = irgen.new_temp();
		if(eq($2.type,"=") == true){
			if(type_change_statement == ""){
				tem = irgen.assign(string($1.ir.tmp) ,string($3.ir.tmp));
			}
			else{
				tem = irgen.assign(string($1.ir.tmp) ,new_temp);
			}
			
		}
		else{
			std::string s = $2.type;
			s.pop_back();
			if(type_change_statement == ""){
				tem = (irgen.add_op(string($1.ir.tmp), string($1.ir.tmp),s ,string($3.ir.tmp)));
			}
			else{
				tem = (irgen.add_op(string($1.ir.tmp), string($1.ir.tmp),s ,new_temp));
			}
			
		}
		std::vector<std::string> tmpcheck = $3.backpatcher->getTrueList();
		int size = tmpcheck.size();
		if(size){
			HANDLE_BOOL_RESULT_ASSIGN($1, $$, $3, irgen);
		}
		else{
			if(type_change_statement == ""){
				std::string ss = irgen.concatenate(irgen.concatenate(string($$.ir.code),type_change_statement),tem);
				$$.ir.code = strdup(ss.c_str());
			}
			else{
				std::string type_change_used = new_temp + " = " + type_change_statement + string($3.ir.tmp);
				std::string ss = irgen.concatenate(irgen.concatenate(string($$.ir.code),type_change_used),tem);
				$$.ir.code = strdup(ss.c_str());
			}
			
			
		}
		$$.ir.tmp = strdup($1.ir.tmp);
		$$.type = original ;
		$$.backpatcher = new BackPatcher();
		

	}
	;

assignment_operator
	: '=' {$$.type = "=";}
	| MUL_ASSIGN {$$.type = "*=";}
	| DIV_ASSIGN {$$.type = "/=";}
	| MOD_ASSIGN {$$.type = "%=";}
	| ADD_ASSIGN {$$.type = "+=";}
	| SUB_ASSIGN {$$.type = "-=";} 
	| LEFT_ASSIGN {$$.type = "<<=";}
	| RIGHT_ASSIGN {$$.type = ">>=";}
	| AND_ASSIGN {$$.type = "&=";}
	| XOR_ASSIGN {$$.type = "^=";}
	| OR_ASSIGN {$$.type = "|=";}
	;

expression
	: assignment_expression {
		$$.type=$1.type;
		$$.name=$1.name;
		$$.ir.tmp = strdup($1.ir.tmp);
		$$.ir.code = strdup($1.ir.code);
		
		$$.backpatcher = BackPatcher::copy($1.backpatcher);
		
		
    	delete $1.backpatcher;
	}
	| expression ',' assignment_expression {
		$$.ir.code = strdup(irgen.concatenate(string($1.ir.code),string($3.ir.code)).c_str());
		$$.backpatcher = new BackPatcher();
		$$.ir.tmp = strdup($3.ir.tmp);
		
	}
	;

constant_expression
	: conditional_expression	/* with constraints */
	;

declaration
	: declaration_specifiers ';'{
		$$.ir.code = strdup($1.ir.code);
	}
	| declaration_specifiers init_declarator_list ';'{
		//printf("\n\n%s\n\n",$2.name);
		//std::cout<<"HELL"<<std::endl;
		//printf("\n\n%s\n\n",$2.type);
		//no func
		// saving the static varibles in a map in TypeSet ts
		if(contains($1.type,"static")){
			ts.addStaticVariable(currFunc,std::string($2.name));
		}
		
		
		char* tocheck1 = "struct";
		char* tocheck2 = "class";
		char* tocheck3 = "union";
		char* tocheck4 = "enum";
	
		// this one is for const that is declared(handling this case explicity)
		if(contains($2.type,"declared")){
			
			if(((contains(($1.type),tocheck1)) || (contains(($1.type),tocheck2)) || (contains(($1.type),tocheck3)))){
				st.declare_struct_variables(std::string($1.type),std::string($2.name));
			}
			// here we are changing the offset and size for declaration;
			$2.type = strdup(removeDeclared($2.type).c_str());
			st.updateVariableTypes(std::string($2.name),std::string($2.type));
		}
		else{
			// adding typedef to the TypeSet ts
			if(contains($1.type,"typedef")){
				ts.addTypedef(std::string($2.name),std::string($1.type));
				st.update_symbol_sizes(std::string($2.name),0);
			}
			// for non typedef that means declarations
			else{
				
				// checking if a valid type or not
				if(ts.contains(std::string($1.type))){
					
					// if valid then checking if a const variable
					if(contains($1.type,"CONST")){
						// for const pointer, struct ,enum,union can be Undeclared as well
						// need to check if pointer, struct ,enum , union
						if( (contains(($1.type),tocheck1)) || (contains(($1.type),tocheck2)) || (contains(($1.type),tocheck3)) || (contains(($1.type),tocheck4)) ){}
						else if((!ts.hasPointer(std::string($2.type)))){
							// for for const otherwise
							yyerror("uninitialized const variable");
						}
						
					}
					// this one checks if struct,union,class is being declared then initialize its members as well;
					if((!classDef)  && ((contains(($1.type),tocheck1)) || (contains(($1.type),tocheck2)) || (contains(($1.type),tocheck3)))){
						
						
						st.declare_struct_variables(std::string($1.type),std::string($2.name));
					}
				
					// finally here also we we are updating the offset and size;

					st.updateVariableTypes(std::string($2.name),std::string($2.type));
				}
				else{
					yyerror("Specified type declaration not allowed");
				}
			}
		}
		$$.ir.code = strdup(irgen.concatenate(string($1.ir.code),string($2.ir.code)).c_str());
	}
	| declaration_specifiers error { yyerrok; }
    | declaration_specifiers init_declarator_list error {yyerrok;}
	;

declaration_specifiers
	: storage_class_specifier declaration_specifiers
	    {
			char *combined = (char*)malloc(strlen($1.type) + strlen($2.type) + 2);
              sprintf(combined, "%s %s", $1.type, $2.type);
              $$.type = combined;
              currentType = $$.type;
              free($1.type);
              free($2.type);

				//printf("\n\n%s\n\n",combined);

			  if(ts.storagespecifercount(std::string($$.type))>=2){
					yyerror("Multiple storage class specifiers not allowed");
			  }
			  $$.ir.code = "";

		}
	| storage_class_specifier{
		$$.type = $1.type;
        currentType = $$.type;
		$$.ir.code = "";
	}
	| type_specifier declaration_specifiers
	      {
              /* Concatenate the current type_specifier ($1) with the following declaration_specifiers ($2).
                 A space is inserted between the two parts. */
              char *combined = (char*)malloc(strlen($1.type) + strlen($2.type) + 2);
              sprintf(combined, "%s %s", $1.type, $2.type);
              $$.type = combined;
              currentType = $$.type; /* Update the global currentType */
			$$.ir.code = "";
          }
	| type_specifier 
	      { 
              $$.type = $1.type; 
			  $$.ir.code = "";
              currentType = $$.type; 
          }
	| type_qualifier declaration_specifiers
	    { 
			char *combined = (char*)malloc(strlen($1.type) + strlen($2.type) + 2);
			sprintf(combined, "%s %s", $1.type, $2.type);
			/* type qualifier is const, volatile. we add it to property, cuz 
			const int and int both must be compatible...
			but wait, const int on left must throw an error so see it
			*/
			$$.type = combined;
			currentType = $$.type; 
			if((ts.constCount(std::string(combined)) > 1)){
				yyerror("duplicate \'const\'");
			}
			// const auto is an invalid type
			if(eq(combined,"CONST auto")){
				yyerror("type defaults to 'int'");
			}
			$$.ir.code = "";
		}
	| type_qualifier
	      { $$.type = $1.type; 
		  currentType = $$.type;
		  $$.ir.code = "";
		  }
	| declaration_specifiers '&'{

		$$.ir.code = "";
	
	}
	;


init_declarator_list
	: init_declarator {
		if(contains($1.kind , "PROCEDURE") ==true){
			yyerror("NO FUNC DECLARATIONS ALLOWED, ONLY DEFINITIONS");
		}
		 $$.index = $1.index;
		 $$.type = $1.type;
		 $$.kind = $1.kind;
		 $$.name = $1.name;
		 $$.ir.tmp = strdup($1.ir.tmp);
		 $$.ir.code = strdup($1.ir.code);
		
	}
	| init_declarator_list ',' init_declarator {
		if(contains($1.kind , "PROCEDURE") ==true || contains($3.kind, "PROCEDURE") == true){
			yyerror("NO FUNC DECLARATIONS ALLOWED, ONLY DEFINITIONS");
		}
		$$.name = concat($1.name,$3.name);
		$$.type = $1.type;
		$$.ir.tmp = strdup($3.ir.tmp);
		 $$.kind = $1.kind;
		 $$.index = $1.index;
		$$.ir.code = strdup(irgen.concatenate(string($1.ir.code),string($3.ir.code)).c_str());
	}
	;

init_declarator
	: declarator '=' initializer {
		// the left of the declarator muse be lvalue;
			lvalueError($1.kind);
					int stars = 0;
					
					std::string typ = $3.type;
					int ptr_cnt = 0;
					std::string tot;
					int index = 0;
					if(st.lookup($3.name) != nullptr){
						tot = st.lookup($3.name)->type;
						vector<int> dim = st.lookup($3.name)->dimensions;
						index = dim.size();
						for(int i=tot.size()-1;i>=0;i--){
						if(tot[i] == '*')ptr_cnt++;
					}
					}
					
					for(int i=typ.size()-1;i>=0;i--){
						if(typ[i] == '*')stars++;
					}
					
		
		
		//get the index of the next multiplier constant
		//cout <<s <<"\n";
		//std::cout <<typ <<"\n";
		printf("%s %s \n %s %s \n %s %s \n CWAZY",$1.ir.tmp,$3.ir.tmp, $1.name,$3.name, $1.type, $3.type);
		
		if(stars > ptr_cnt - index){
			//bt 
			yyerror("assignment involves array on right or has incorrent dereferencing");
		}
			// If a procedure then must be called procedure
			if(isPROCEDURE($3.kind)){yyerror("Cannot assign function to a variable");}

			//remove static
			std::string t = ts.removeStaticFromDeclaration(std::string($1.type));
			char* temp = strdup(t.c_str());

			// remove static and const
			t = ts.removeStaticFromDeclaration(std::string($3.type));
			char* temp2 = strdup(t.c_str());

			//printf("\n%s\n%s\n",temp,temp2);

			std::string type_change_statement = "";
			// TO CHECK IF BOTH THE TYPES ARE SAME OR NOT
			if(eq(temp2 , temp) == false){
				// HANDLING IN CASE OF AUTO AND CHANGING THE TYPE TO THE FIRST ASSIGNED TYPE
				if(eq($1.type,"auto")){
					st.lookup(std::string($1.name))->type = std::string($3.type);
					$$.type = $3.type;
				} 
				// CASE OF ENUM
				else if(contains(temp,"enum")){
					$$.type = $1.type;

					//printf("\n\n%s\n\n%s\n\n",$3.kind,$3.type);
					// ENUM CAN BE INITIALIZED BY ENUM_CONST , IDENTIFIER , CONST 
					if(eq($3.kind, "ENUM_CONST") || contains($3.kind,"IDENTIFIER") || eq($3.kind, "CONST") ){
						// ONLY INT TYPE VALUES CAN BE ASSIGNED 
						std::string tmp = ts.removeConstFromDeclaration(std::string(temp));
						temp = strdup(tmp.c_str());
						if(eq(temp2,"INT")){
							// CHECKING IF IT IS A CONST ENUM
							if(contains(temp,"CONST")){
								$$.type = concat($$.type,"declared");  //adding this to show that the const variable is declared (to be only used for declaration of const);
							}
						}
						else{
							yyerror("incompatible types when assigning to type 'enum'");
						}
					}
					else{
						yyerror("enum must be initialized with INT");
					}
				}
				// Case if declarator has const
				else if(contains(temp,"CONST")){
					$$.type = $1.type;
					char* matchingType1 = strdup(ts.removeConstFromDeclaration(std::string(temp)).c_str());
					char* matchingType2 = strdup(ts.removeConstFromDeclaration(std::string(temp2)).c_str());
					//printf("\n%s\n%s\n",matchingType1,matchingType2);
					if(ts.hasPointer(std::string(matchingType1)) || ts.hasPointer(std::string(matchingType2))){
						check_type(matchingType1, matchingType2, "incompatible type involved : ");
					}
					else if(isConvertible(std::string(matchingType1),std::string(matchingType2))){
						$$.type = concat($$.type,"declared");  //adding this to show that the const variable is declared (to be only used for declaration of const);
						if(!eq(matchingType1,matchingType2)){
							type_change_statement = formatTypeChange(matchingType2,matchingType1);
						}
					}
					else {
						yyerror("invalid assignment");
					}
					
				}
				// all other cases
				else{
					char* matchingType1 = strdup(ts.removeConstFromDeclaration(std::string(temp)).c_str());
					char* matchingType2 = strdup(ts.removeConstFromDeclaration(std::string(temp2)).c_str());
					if(!isConvertible(matchingType1,matchingType2)){
						char *err = "incompatible type declaration: ";
						err = concat(err,$1.type);
						err = concat(err, $3.type);
						yyerror(err);
					}
					else{
						
				
						if(!eq(matchingType2,matchingType1)){
							type_change_statement = formatTypeChange(std::string(matchingType2),std::string(matchingType1));
						}
						
					}
					
					$$.type = $1.type;
				}	
			}
			else{
				
				// IF THE TYPES ARE SAME
				//printf("\n%s\n%s\n",$1.kind,$3.kind);
				$$.type = $1.type;
				
				// If a porcedure then must be called procedure
				if(isPROCEDURE($3.kind)){
					yyerror("Cannot assign function to a variable");
				}
				else if(contains($1.type,"CONST")){
					$$.type = concat($$.type, "declared");
				}
			}	
		
			//IR GEN
			
			std::vector<std::string> tmpcheck = $3.backpatcher->getTrueList();
				int size = tmpcheck.size();
					
			if(size>0){ // case where a = ()||()
			
				// Label making
				std::string label1 = irgen.new_label();
				std::string True_label = irgen.add_label(label1);
				std::string label2 = irgen.new_label();
				std::string False_label = irgen.add_label(label2);
				std::string label3 = irgen.new_label();
				std::string False_end = irgen.add_label(label3);
				std::string goto_false_end = irgen.create_goto(label3);

				// making assignment terms 
				std::string true_assign = irgen.assign($1.ir.tmp, "1");
				std::string false_assign = irgen.assign($1.ir.tmp, "0");

				// Backpatching
				$3.ir.code = strdup($3.backpatcher->backPatchTrueList(std::string($3.ir.code),label1).c_str());
				$3.ir.code = strdup($3.backpatcher->backPatchFalseList(std::string($3.ir.code),label2).c_str());

				// IRGEN
				$$.ir.code = strdup(irgen.concatenate(std::string($3.ir.code),True_label).c_str());
				$$.ir.code = strdup(irgen.concatenate(std::string($$.ir.code),true_assign).c_str());
				$$.ir.code = strdup(irgen.concatenate(std::string($$.ir.code),goto_false_end).c_str());
				$$.ir.code = strdup(irgen.concatenate(std::string($$.ir.code),False_label).c_str());
				$$.ir.code = strdup(irgen.concatenate(std::string($$.ir.code),false_assign).c_str());
				$$.ir.code = strdup(irgen.concatenate(std::string($$.ir.code),False_end).c_str());
				
			}
			else{
				string new_name = string($1.name);
				int num;
				num = st.lookup(new_name)->block_num;
				new_name += "#block";
				new_name += to_string(num);
				if(type_change_statement != ""){
					std::string new_temp = irgen.new_temp();
					std::string tem =  irgen.assign(new_name, new_temp);
					std::string use_type_change = new_temp + " = " + type_change_statement + std::string($3.ir.tmp);
					std::string g = irgen.concatenate(std::string($3.ir.code),use_type_change);
					g = irgen.concatenate(g,tem);
					$$.ir.code =strdup(irgen.concatenate(std::string(""),std::string(g)).c_str());
				}
				else{
					std::string tem = irgen.assign(new_name, $3.ir.tmp);
					std::cout<<tem<<"\n";
					std::string g = irgen.concatenate(std::string($3.ir.code),type_change_statement);
					g = irgen.concatenate(g,tem);
					
					$$.ir.code =strdup(irgen.concatenate(std::string(""),std::string(g)).c_str());
				}
				
			}
				
				
	}
	| declarator {
		 $$.index = $1.index;
		 $$.type = $1.type;
		 $$.kind = $1.kind;
		 $$.name = $1.name;
		 $$.ir.tmp = strdup($1.ir.tmp);
		 $$.ir.code = strdup($1.ir.code);
	}
	;

storage_class_specifier
	: TYPEDEF	{ $$.type = strdup("typedef"); }/* identifiers must be flagged as TYPEDEF_NAME */
	| EXTERN { $$.type = strdup("extern"); }
	| STATIC { $$.type = strdup("static"); }
	| AUTO { $$.type = strdup("auto"); yyerror("auto is not allowed"); }
	| REGISTER { $$.type = strdup("register"); }
	;

type_specifier
	: VOID { currentType = strdup("VOID"); $$.type = currentType; }
	| CHAR { currentType = strdup("CHAR"); $$.type = currentType; }
	| SHORT { currentType = strdup("SHORT"); $$.type = currentType; }
	| INT { currentType = strdup("INT"); $$.type = currentType; }
	| LONG { currentType = strdup("LONG"); $$.type = currentType; }
	| FLOAT { currentType = strdup("FLOAT"); $$.type = currentType; }
	| DOUBLE { currentType = strdup("DOUBLE"); $$.type = currentType; }
	| SIGNED { currentType = strdup("SIGNED"); $$.type = currentType; }
	| UNSIGNED { currentType = strdup("UNSIGNED"); $$.type = currentType; }
	| BOOL { currentType = strdup("BOOL"); $$.type = currentType; }
	| struct_or_union_specifier { currentType = $1.type; $$.type = currentType; }
	| class_specifier {currentType = $1.type; $$.type = currentType;}
	| enum_specifier { currentType = $1.type; $$.type = currentType; }
	| TYPEDEF_NAME	{ currentType = strdup(ts.getTypedef(std::string(yytext)).c_str()); $$.type = currentType;}
	;

class_specifier
    : CLASS '{' {st.push_scope("class anonymous");block_num++;st.current_scope_->block_num = block_num;} class_member_list '}' 
         {	
			if(st.current_scope_->contains_break_or_continue == true){
				yyerror("Using continue or break inside Classes in invalid");
			}
			st.pop_scope();
			st.current_scope_->contains_break_or_continue =false;
			$$.type = (char*)malloc(strlen("class") + 14); 
			sprintf($$.type, "class (anonymous)");
			yyerror("anonymous class definition not allowed");
		   }
	| CLASS IDENTIFIER base_clause_opt  '{' 
		{   classDef=1;
			std::string s = std::string("class ") + std::string(strdup($2.type));
			st.insert_symbol(std::string($2.type),"CLASS","USER DEFINED");
			st.update_symbol_sizes(std::string($2.type),0);
			st.push_scope( std::string("class ")+ std::string(strdup($2.type)));
			block_num++;
			st.current_scope_->block_num = block_num;
			ts.addClass(std::string($2.type));
			if(!eq($3.name,"NULL")){
				st.implement_inheritance(std::string(concat("class",$2.type)),std::string($3.name));
			}
		}
		class_member_list '}' 
		{
			classDef=0;
			//std::cout<<priMem<<"\n"<<proMem<<"\n"<<pubMem<<"\n";
			st.update_class_types(priMem,pubMem,proMem);
			priMem = "";
			proMem = "";
			pubMem = ""; 
			if(st.current_scope_->contains_break_or_continue == true){
				yyerror("using continue or break invalid in this context");
			}
			st.pop_scope();
			st.current_scope_->contains_break_or_continue = false;
		    $$.type = (char*)malloc( strlen("class") + strlen($2.type) + 14 ); // one space plus null
         	sprintf($$.type, "class %s", $2.type);
			std::string s = std::string("class ") + std::string($2.type); 

			int size = st.calculateStructSize(s);
			st.addTypeSize(s,size);
			if(st.hasDuplicateNamesInScope(s)){
				yyerror("Diamond Inheritence Not allowed");
			}
		   
		}
	| CLASS IDENTIFIER base_clause_opt  
         { 
          $$.type = (char*)malloc(strlen("class") + strlen($2.type) + 2);
         sprintf($$.type, "%s %s", "class", $2.type);
		
		 if(!ts.contains(std::string($$.type))){
			yyerror("Use of undeclared CLASS");
		 }
		 }
    ;


class_member_list
    : class_member
    | class_member_list class_member
    ;

class_member
    : access_specifier ':' '{' translation_unit '}' {
		isPub=0;
		isPri=0;
		isPro=0;
	}
    ;


access_specifier
    : PUBLIC { $$.type = strdup("public"); isPub=1; }
    | PRIVATE { $$.type = strdup("private"); isPri=1; }
    | PROTECTED { $$.type = strdup("protected"); isPro=1; }
    ;
/* Optional inheritance clause */
base_clause_opt
    : ':' base_specifier_list { $$.name = $2.name; }
    | /* empty */ { $$.name = "NULL"; }
    ;

base_specifier_list
    : base_specifier { $$.type = $1.type; }
    | base_specifier_list ',' base_specifier { 
           
		   $$.name = concat($1.name,$3.name);
        }
    ;

/* A single base specifier with an optional access specifier */
base_specifier
    : access_specifier_opt IDENTIFIER{
		if( (st.lookup(std::string($2.type))) && (st.lookup(std::string($2.type))->type == "CLASS") ){}
		else{
			std::string err = "base class \'" + std::string($2.type) + "\' has not been declared";
			char* errr = strdup(err.c_str());
			yyerror(errr);
		}

		$$.name = strdup(concat($1.type,$2.type));
	}
		
    ;

/* Optional access specifier in the base clause */
access_specifier_opt
	: /* empty */ {$$.type = NULL;}
	| access_specifier {$$.type = $1.type;}
	;

struct_or_union_specifier
	: struct_or_union '{'
	 {
		if(eq($1.type,"struct")){
			st.push_scope("struct anonymous");
		}
		else{
			st.push_scope("union anonymous");
		}
		block_num++;
		st.current_scope_->block_num = block_num;
	 } struct_declaration_list '}'  {
		if(st.current_scope_->contains_break_or_continue == true){
			yyerror("using break/continue invalid in this scope");
		}
		if(eq($1.type, "union")){
			//i need to keep the offset of all internal variables 0 
			//since all var have same memory location
				//offset = scope_ptr->symbol_map[string($3.type)]->offset;
			for(auto &it: st.scopes_.back()->symbol_map){
				//it.first is variable
				//it.second->offset is what i need to update
				it.second->offset = 0;
				
			}
		}
		st.pop_scope();
		st.falsekardo();
         /* Anonymous struct or union */
         $$.type = (char*)malloc(strlen($1.type) + 14); // Enough for " (anonymous)" and '\0'
         sprintf($$.type, "%s (anonymous)", $1.type);
		 if(eq($1.type,"struct")){
			yyerror("anonymous struct definition not allowed");
		}
		else{
			yyerror("anonymous union definition not allowed");
		}
		 
    }
	| struct_or_union IDENTIFIER '{' {
			std::string s = std::string(strdup($1.type)) + std::string(" ") + std::string(strdup($2.type));
			if(eq($1.type,"struct")){
				st.insert_symbol(std::string($2.type),"STRUCT","USER DEFINED");
				st.update_symbol_sizes(std::string($2.type),0);
				ts.addStruct(std::string($2.type));
			}
			else{
				st.insert_symbol(std::string($2.type),"UNION","USER DEFINED");
				st.update_symbol_sizes(std::string($2.type),0);
				ts.addUnion(std::string($2.type));
			}
			st.push_scope(s);
			block_num++;
			st.current_scope_->block_num = block_num;
			}
			struct_declaration_list '}'  {
			
			if(eq($1.type, "union")){
				
				//i need to keep the offset of all internal variables 0 
				//since all var have same memory location
					//offset = scope_ptr->symbol_map[string($3.type)]->offset;
				for(auto &it: st.scopes_.back()->symbol_map){
					//it.first is variable
					//it.second->offset is what i need to update
					it.second->offset = 0;
					
				}
			}

			if(st.current_scope_->contains_break_or_continue == true){
				yyerror("using break/continue invalid in this scope");
			}
			st.pop_scope();
			st.falsekardo();

			/* Named struct/union with body */
			$$.type = (char*)malloc(strlen($1.type) + strlen($2.type) + 2); // one space plus null
			sprintf($$.type, "%s %s", $1.type, $2.type);
			std::string s = std::string(strdup($1.type)) + std::string(" ") + std::string(strdup($2.type));
			int size =0 ;
			if(eq($1.type,"struct")){
				size = st.calculateStructSize(s);
			}
			else{
				size = st.calculateUnionSize(s);
				if(eq($1.type, "union")){
			
			//i need to keep the offset of all internal variables 0 
			//since all var have same memory location
				//offset = scope_ptr->symbol_map[string($3.type)]->offset;
				string t = string($1.type);
				t += " ";
				t += string($2.type);
				//cout <<t <<"\n";

			for(auto &it: st.scopes_){
				if(it->scope_name == t){
					for(auto &it2: it->symbol_map){
						it2.second->offset = 0;
					}
				}
			}
			
		}
			}
			st.addTypeSize(s,size);
			

		}
	| struct_or_union IDENTIFIER {
         /* Named struct/union declaration without body */
         $$.type = (char*)malloc(strlen($1.type) + strlen($2.type) + 2);
         sprintf($$.type, "%s %s", $1.type, $2.type);
	
		 if(!ts.contains(std::string($$.type))){
			yyerror("Use of undeclared Struct");
		 }

    }
	;

struct_or_union
	: STRUCT{ $$.type = strdup("struct"); }
	| UNION { $$.type = strdup("union"); }
	;

struct_declaration_list
	: struct_declaration
	| struct_declaration_list struct_declaration
	;

struct_declaration
	: specifier_qualifier_list ';'{
		if(contains($1.type,"class")){
			yyerror("class definitions not allowed inside structs");
		}
		
	}
	| specifier_qualifier_list struct_declarator_list ';'{
		//std::cout<<std::string($1.type)<<"\n"<<std::string($2.name)<<"\n";
		st.updateVariableTypes(std::string($2.name),std::string($1.type));
		if(contains($1.type,"class")){
			yyerror("class definitions not allowed inside structs");
		}
		
	}
	;

specifier_qualifier_list
	: type_specifier  specifier_qualifier_list {
		$$.type = $1.type;
		$$.type = concat($$.type,$2.type);
	}
	| type_specifier{
		$$.type = $1.type;
	}
	| type_qualifier specifier_qualifier_list {
		$$.type = $1.type;
		$$.type = concat($$.type,$2.type);
	}
	| type_qualifier
	;

struct_declarator_list
	: struct_declarator{
		$$.name = $1.name;
		$$.index = $1.index;
		 $$.type = $1.type;
		 $$.kind = $1.kind;
		 $$.name = $1.name;
		 $$.ir.tmp = strdup($1.ir.tmp);
		 $$.ir.code = strdup($1.ir.code);
	}
	| struct_declarator_list ',' struct_declarator {
		$$.name = concat($1.name,$3.name);
	}
	;

struct_declarator
	: declarator {
		 $$.index = $1.index;
		 $$.type = $1.type;
		 $$.kind = $1.kind;
		 $$.name = $1.name;
		 $$.ir.tmp = strdup($1.ir.tmp);
		 $$.ir.code = strdup($1.ir.code);
	}
	;

enum_specifier
    : ENUM '{' enumerator_list '}' { 
          /* Anonymous enum; you might choose to simply return "enum" */
          $$.type = strdup("enum");
		  yyerror("anonymous enum declaration not allowed");
      }
    | ENUM '{' enumerator_list ',' '}' { 
          $$.type = strdup("enum");
		   yyerror("anonymous enum declaration not allowed");
      }
    | ENUM IDENTIFIER '{' enumerator_list '}' { 
          /* Named enum: produce "enum <identifier>" */
          char *tmp = (char*)malloc(strlen("enum") + strlen($2.type) + 2);
          sprintf(tmp, "enum %s", $2.type);
          $$.type = tmp;
		  ts.addEnum(std::string($2.type));
		  st.insert_symbol(std::string($2.type),"ENUM","USER DEFINED");
		  st.update_symbol_sizes(std::string($2.type),0);
		  curr_enum_value =0;
      }
    | ENUM IDENTIFIER '{' enumerator_list ',' '}' { 
          char *tmp = (char*)malloc(strlen("enum") + strlen($2.type) + 2);
          sprintf(tmp, "enum %s", $2.type);
          $$.type = tmp;
		  ts.addEnum(std::string($2.type));
		  st.insert_symbol(std::string($2.type),"ENUM","USER DEFINED");
		  curr_enum_value =0;
      }
    | ENUM IDENTIFIER { 
          char *tmp = (char*)malloc(strlen("enum") + strlen($2.type) + 2);
          sprintf(tmp, "enum %s", $2.type);
          $$.type = tmp;
		  ts.addEnum(std::string($2.type));
      }
    ;


enumerator_list
	: enumerator
	| enumerator_list ',' enumerator
	;

enumerator	/* identifiers must be flagged as ENUMERATION_CONSTANT */
	: enumeration_constant '=' constant_expression {
		std::string enum_const = std::string($1.name);
		std::string value = std::string($3.name);
		enum_to_value[enum_const] = value;
		int num = std::stoi(value);
		curr_enum_value =num+1;
	}
	| enumeration_constant{
		std::string enum_const = std::string($1.name);
		enum_to_value[enum_const] = std::to_string(curr_enum_value++);
	}
	;

type_qualifier
	: CONST { $$.type = strdup("CONST"); }
	| VOLATILE { $$.type = strdup("VOLATILE"); }
	;



declarator
	: pointer direct_declarator { // concatenation of * will be spearate everywhere
          int idx = $2.index;  // $2 is the token table index from direct_declarator.
          char* newtype = concat(st.token_table_[idx].token_type.c_str(), $1.type);
		  st.token_table_[idx].token_type += std::string($1.type);
		  //$2.name is name of id
		  $$.name = $2.name;
		  st.lookup(std::string($2.name))->type = st.token_table_[idx].token_type;
		  //std::cout<<"HELLO"<<std::endl;
		  $$.type = strdup(st.token_table_[idx].token_type.c_str());
		  if (strstr(newtype, "typedef") != NULL){
            char *temp = new char[st.token_table_[idx].token.size()+1];
            std::strcpy(temp,st.token_table_[idx].token.c_str());
			update_symtab(temp);
		  }
          $$.index = idx;
		  $$.kind = $2.kind;
		  // for now const after * not allowed
		  if(ts.hasConstAfterStar(std::string($$.type))){
				yyerror("const after * not allowed");
		  }
		  $$.ir.tmp = strdup($2.ir.tmp);
		  $$.ir.code = strdup($2.ir.code);
      }
	| direct_declarator {
		 $$.index = $1.index;
		 $$.type = $1.type;
		 $$.kind = $1.kind;
		 $$.name = $1.name;
		 $$.ir.tmp = strdup($1.ir.tmp);
		 $$.ir.code = strdup($1.ir.code);
		  }
	;

direct_declarator
	: IDENTIFIER{
		std::string tmp = $1.type;
		if(st.lookup_cur(tmp) != nullptr){
			//THIS GIVES ERROR EVEN IF IN PARENT SCOPE... (ie conflict with parent scope)
			std::string err = st.lookup(tmp)->name + " already declared before: ";
			yyerror(err.c_str());
		}
		else{
			$$.index = st.token_table_.size() ;
		$$.type = currentType;
		$$.kind = "IDENTIFIER";
		$$.name = $1.type;
			st.insert_symbol($1.type, currentType ? currentType : "INVALID", "IDENTIFIER");
			st.lookup($1.type)->block_num = block_num;
			if(classDef==1){
				if(isPri==1){
					priMem += " " + std::string($1.type);
				}
				else if(isPub==1){
					pubMem += " " + std::string($1.type);
				}
				else if(isPro==1){
					proMem += " " + std::string($1.type);
				}
			}
			else{
				
				
			}
		}
		
		
        $$.ir.tmp = strdup($$.name);
		$$.ir.code = "";

    }
	| '(' declarator ')' {
		 $$.index = $2.index;
		$$.name = $2.name;
		$$.ir.tmp = strdup($2.ir.tmp);
		$$.ir.code = strdup($2.ir.code);
	 }
	| direct_declarator '[' ']'{
		int idx = $1.index;
		char newType[256];
		sprintf(newType, "%s*",st.token_table_[idx].token_type);
		st.token_table_[idx].token_type += "*";
		$$.name = $1.name;
		st.lookup(std::string($1.name))->type = st.token_table_[idx].token_type;
		$$.type = strdup(st.token_table_[idx].token_type.c_str());
		st.update_array_dimensions(std::string($1.name),0,std::string($$.type));
		if (strstr(newType, "typedef") != NULL){
		char *temp = new char[st.token_table_[idx].token.size()+1];
		std::strcpy(temp,st.token_table_[idx].token.c_str());
		update_symtab(temp);
		
		}
		$$.index = idx;
		$$.ir.code = strdup($1.ir.code);
		$$.ir.tmp = strdup($1.ir.tmp);
      }
	| direct_declarator '[' assignment_expression ']'{
			if($3.type == "INT" && $3.kind == "CONST"){
				 int idx = $1.index;
          	char newType[256];
          	sprintf(newType, "%s*", st.token_table_[idx].token_type, $3.type);
		  	$$.name = $1.name;
			st.token_table_[idx].token_type += "*";
			st.lookup(std::string($1.name))->type = st.token_table_[idx].token_type;
			$$.type = strdup(st.token_table_[idx].token_type.c_str());
			//printf("\n\n%s\n\n%s\n\n",$$.type,$3.name);

			// here we are updating the size and offset for array declaration (already made a check in updateVariableTypes so that its implementation doesn't clash )
			st.update_array_dimensions(std::string($1.name),std::stoi(std::string($3.name)),std::string($$.type));
			if (strstr(newType, "typedef") != NULL){
				char *temp = new char[st.token_table_[idx].token.size()+1];
				std::strcpy(temp,st.token_table_[idx].token.c_str());
				update_symtab(temp);
			}
			$$.index = idx;
		}
		else{
			std::string err = "declaration must have only integer constants inside: ";
			yyerror(err.c_str());
		}
		$$.ir.tmp = strdup($1.ir.tmp);
		$$.ir.code = strdup(irgen.concatenate(string($1.ir.code),string($3.ir.code)).c_str());
		
      }
	| direct_declarator '('  parameter_type_list ')' {
		/* pushing scopes extra if there are arguments inside, eg. fun(int a, int b) */
       int idx = $1.index;  // $1 is now of type int (the token index)
		
	   	st.token_table_[idx].kind = strdup("PROCEDURE ");
		char* newkind =concat(st.token_table_[idx].kind.c_str(),"(");
		newkind = concat(newkind,$3.type);
		newkind = concat(newkind,")");
		st.token_table_[idx].kind = strdup(newkind);

		st.lookup(std::string($1.name))->kind = st.token_table_[idx].kind;
	  	$$.type = strdup(st.token_table_[idx].token_type.c_str());
	   	$$.kind = newkind;
      	$$.index = idx;
	   	$$.name = $1.name;
		currFunc = std::string($$.name);
		//std::cout<<"\n\n"<<currFunc<<"\n\n";
		if(!ts.contains(st.lookup(std::string($1.name))->type)){
			yyerror("Specified function return type not allowed");
		}
		//std::cout<<"\n\n"<<std::string($3.name)<<"\n\n";
		// here we are having 3 important steps 
		// 1. update the identifier kind to PARAMETER
		// 2. update the PARAMETER sizes to 0 and change offset
		// 3. update the size of procedure to 0 since it is called 
		st.changeToParameter(std::string($3.name));
		st.updateParameterSizes();
		st.updateProcedureSize(std::string($$.name));
		
		string label = string($$.name);
		string cd = irgen.add_label(label);
		$$.ir.code = strdup(irgen.concatenate(cd,string($3.ir.code)).c_str());
		$$.ir.tmp = strdup($1.ir.tmp);

    }
	| direct_declarator '('   ')'  {
       int idx = $1.index;
		st.token_table_[idx].kind = strdup("PROCEDURE ( )");
		st.lookup(std::string($1.name))->kind = st.token_table_[idx].kind;
		st.lookup(std::string($1.name))->type = st.token_table_[idx].token_type;
		$$.type = strdup(st.token_table_[idx].token_type.c_str());
		$$.kind = strdup("PROCEDURE ( )");
		$$.index = idx;
		$$.name = $1.name;
		
	   	currFunc = std::string($$.name);
		//std::cout<<"\n\n"<<currFunc<<"\n\n";
		if(!ts.contains(st.lookup(std::string($1.name))->type)){
			yyerror("Specified function return type not allowed");
		}
		// since no parameter_list so directly updating procedure size to 0
		st.updateProcedureSize(std::string($$.name));
		
		string lb = string($$.name);
		string cd = irgen.add_label(lb);
		$$.ir.code = strdup(cd.c_str());
	   	$$.ir.tmp  =strdup($1.ir.tmp);
    }
	| direct_declarator '('  identifier_list ')'{
		//NOW GIVES ERROR
		//K&R style function, can remove it
       	int idx = $1.index;
    	st.token_table_[idx].token_type = strdup(currentType);
	   	st.token_table_[idx].kind = strdup("PROCEDURE");
	    st.lookup(std::string($1.name))->type = st.token_table_[idx].token_type;
		st.lookup(std::string($1.name))->kind = st.token_table_[idx].kind;
	   	$$.type = strdup(currentType);
	   	$$.kind = strdup("PROCEDURE");
       	$$.index = idx;
	   	$$.name = $1.name;
		$$.ir.code ="";
		$$.ir.tmp = $1.ir.tmp;
		yyerror("K&R Style function definition not allowed");
    }
	| direct_declarator '[' assignment_expression error {
          /* Catch an invalid array declaration (missing ']'). 
             Use the default error message and recover. */
          yyerrok;
          $$.index = $1.index;  /* Propagate the index as is */
      }
	;
					// completed the implementation of pointer class;
					
pointer
	: '*' type_qualifier_list pointer {
		std::string s = "* ";
		s += std::string($2.type);
		s += std::string($3.type);
		$$.type = strdup(s.c_str());
	}
	| '*' type_qualifier_list {
		char* newtype = concat("*",$2.type);
		$$.type = newtype;
	}
	| '*' pointer{
		std::string s = "*";
		s += std::string($2.type);
		$$.type = strdup(s.c_str());
      }
	| '*' {
          $$.type = strdup("*");
      }
	;

type_qualifier_list
	: type_qualifier{
		$$.type = $1.type;
	}
	| type_qualifier_list type_qualifier{ 
          // Concatenate the qualifiers
		  char* newtype =concat($1.type,$2.type);
          $$.type = newtype;
      }
	;


parameter_type_list
	: parameter_list{  //Progation of type
		$$.type = $1.type;
		$$.name = $1.name;
		$$.ir.code = strdup($1.ir.code);
		$$.index = 0;
	}
	| parameter_list ',' ELLIPSIS{
		$$.type = $1.type;
		$$.type = concat($1.type, "...");
		
		$$.name = $1.name;
		$$.ir.code = strdup($1.ir.code);
		$$.index = 1;
	}
	;

parameter_list
	: parameter_declaration{ //Progation of type
		$$.type = $1.type;
		$$.name = $1.name;
		$$.ir.code = strdup($1.ir.code);
	}
	| parameter_list ',' parameter_declaration { // Concatenate the types
		char* newtype = concat($1.type,$3.type);
		$$.type = newtype;
		char* newname = concat($1.name,$3.name);
		$$.name = newname;
		$$.ir.code = strdup(irgen.concatenate(string($1.ir.code),string($3.ir.code)).c_str());
	}
	;

parameter_declaration
	: declaration_specifiers declarator { //Progation of type
		if(!ts.contains(std::string($1.type))){
			yyerror("parameter type not allowed");
		}
		$$.type = $2.type;
		$$.name = $2.name;

		$$.ir.code ="";
	}
	;

identifier_list
	: IDENTIFIER
	| identifier_list ',' IDENTIFIER
	;

type_name
	: specifier_qualifier_list abstract_declarator{
		std::string s = std::string($1.type);
		s += std::string($2.type);
		$$.type = strdup(s.c_str());
		$$.kind = $2.kind;
	
	}
	| specifier_qualifier_list{
		$$.type = $1.type;
		$$.kind = $1.kind;
	}
	;

abstract_declarator
	: pointer direct_abstract_declarator{
		std::string s = std::string($2.type);
		s += std::string($1.type);
		$$.type = strdup(s.c_str());
		$$.kind = $2.kind;
	}
	| pointer {
		$$.type = strdup("*");
	}
	| direct_abstract_declarator{
		$$.type = $1.type;
		$$.kind = $1.kind;
	}
	;

direct_abstract_declarator
	: '(' abstract_declarator ')' {
		char *tmp = "(";
		$$.type = concat(tmp,$2.type);
		tmp = ")";
		$$.type = concat($$.type,tmp);
		$$.kind = $2.kind;

	}
	| '[' ']' {
		$$.type = strdup("*");
	}
	| '[' assignment_expression ']' {
		$$.type = $2.type;
		$$.kind = $2.kind;
	}
	| direct_abstract_declarator '[' ']'{
		std::string s = std::string($1.type);
		s += "*";
		$$.type = strdup(s.c_str());
		$$.kind = $1.kind;
	}
	| direct_abstract_declarator '[' assignment_expression ']'{
	}
	| '(' ')'{
		$$.type = " ( )";
		$$.kind = "PROCEDURE";
	}
	| '(' parameter_type_list ')'{
		char *tmp = "(";
		$$.type = concat(tmp, $2.type);
		tmp = ")";
		$$.type = concat($$.type, tmp );
	}
	| direct_abstract_declarator '(' ')' {
		char *tmp = "(";
		$$.type = concat($1.type, tmp);
		tmp = ")";
		$$.type = concat($$.type, tmp );
		$$.kind = "PROCEDURE";
	}
	| direct_abstract_declarator '(' parameter_type_list ')' {
		char *tmp = "(";
		$$.type = concat($1.type, tmp);
		tmp = ")";
		$$.type = concat($$.type, $3.type );
		$$.type = concat($$.type,tmp);
		$$.kind = "PROCEDURE";
	}
	;

initializer
	: '{' initializer_list '}' {
		// if this will go to declarator '=' initializer , it will give error because of backpatcher checking;
		std::string s = std::string($2.type);
		s += "*";
		$$.type = strdup(s.c_str());
		yyerror("initializer list not allowed");
	}
	| assignment_expression {
		$$.type = $1.type;
		$$.kind = $1.kind;
		$$.ir.tmp = strdup($1.ir.tmp);
		$$.ir.code = strdup($1.ir.code);
		$$.backpatcher = BackPatcher::copy($1.backpatcher);
    	delete $1.backpatcher;

	}
	;

initializer_list
	:  initializer{
		$$.type = $1.type;
		$$.kind = $1.kind;
	}
	| initializer_list ',' initializer{
		char* err = "initializer must have same types";
		check_type($1.type,$3.type,err);
	}
	;

statement
	: labeled_statement {
		//doubts in this prodn.
		$$.ir.code = strdup($1.ir.code);
		$$.backpatcher = BackPatcher::copy($1.backpatcher);
    	delete $1.backpatcher;
	}
	|  {st.push_scope();irgen.depth_current++;block_num++;st.current_scope_->block_num = block_num;} compound_statement {
		irgen.depth_current++;
		bool x = st.current_scope_->contains_break_or_continue;
		bool y = st.current_scope_->jump[0];
		bool z = st.current_scope_->jump[1];
		st.pop_scope();
		st.current_scope_->contains_break_or_continue = x;
		st.current_scope_->jump[1] = z;
		st.current_scope_->jump[0] = y;
		$$.ir.code = strdup($2.ir.code);
		$$.backpatcher = BackPatcher::copy($2.backpatcher);
    	delete $2.backpatcher;
		}
	| expression_statement{
		$$.ir.code = strdup($1.ir.code);
		$$.backpatcher = BackPatcher::copy($1.backpatcher);
    	delete $1.backpatcher;
	}
	| selection_statement{
		$$.ir.code = strdup($1.ir.code);
		$$.backpatcher = BackPatcher::copy($1.backpatcher);
    	delete $1.backpatcher;
		std::vector<string> tmp = $$.backpatcher->getNextList();
		int size = tmp.size();
		//printf("\n\n\n%d\n\n\n",size);
		//for(std::string i:tmp){
			//std::cout<<i<<std::endl;
		//}
	}
	| iteration_statement{
		$$.ir.code = $1.ir.code;
		$$.backpatcher = BackPatcher::copy($1.backpatcher);
    	delete $1.backpatcher;
	}
	| jump_statement{
		$$.ir.code = $1.ir.code;
		$$.backpatcher = BackPatcher::copy($1.backpatcher);
	}
	;


labeled_statement
	: IDENTIFIER ':' statement {
		st.insert_symbol($1.type,"LABEL" , "GOTO LABEL");
		string lb = irgen.add_label(std::string($1.type));
		$$.ir.code =strdup(irgen.concatenate(lb,string($3.ir.code)).c_str());
		$$.backpatcher = BackPatcher::copy($3.backpatcher);
	}
	| CASE constant_expression ':' statement {
		if(irgen.is_duplicate_case(std::string($2.name))){
			yyerror("duplicate case value");
		}
		if(!eq($2.type,"INT")){
			yyerror("expression must be an integral constant expression");
		}
		if(!eq($2.kind,"CONST")){
			yyerror("expression must have a constant value");
		}
		std::string label = irgen.new_label();
		std::string Case_label = irgen.add_label(label);
		irgen.add_case_info(std::string($2.name),label);
		//std::cout<<std::string($2.name)<< "    "<<label<<"\n";

		$$.ir.code = strdup(irgen.concatenate(Case_label,std::string($4.ir.code)).c_str());
		$$.backpatcher = new BackPatcher();
	}
	| DEFAULT ':' statement {
		if(irgen.has_default_label()){
			yyerror("duplicate default label");
		}
		
		string label = irgen.new_label();
		string Default_label = irgen.add_label(label);
		irgen.add_case_info("Default",label);
		//std::cout<<"Default"<< "    "<<label<<"\n";
		$$.ir.code =strdup(irgen.concatenate(Default_label,string($3.ir.code)).c_str());
		$$.backpatcher = new BackPatcher();
	}
	;
compound_statement
	: '{'   '}' {
		$$.ir.code = "";
	}
	| '{'  block_item_list '}' {

		// another point for backpatching solving the mentioned problem
		if(bpneeded == 1){
			bpneeded = 0;
			std::string label = irgen.new_label();
			std::string backpatch_label = irgen.add_label(label);
			$2.ir.code = strdup($2.backpatcher->backPatchNextList(std::string($2.ir.code),label).c_str());
			$2.ir.code = strdup(irgen.concatenate(string($2.ir.code),backpatch_label).c_str());
		}
		$$.ir.code = strdup($2.ir.code);
		$$.backpatcher = BackPatcher::copy($2.backpatcher);
		std::vector<std::string> tmp1 = $2.backpatcher->getNextList();
		int size = tmp1.size();
		//std::cout<<size<<std::endl;
    	delete $2.backpatcher;
	}
	| '{'   block_item_list error {  
     yyerrok;}
	;

block_item_list
	: block_item {
		$$.ir.code = strdup($1.ir.code);
		$$.backpatcher = BackPatcher::copy($1.backpatcher);
		delete $1.backpatcher;
		
		std::vector<std::string> tmp = $1.backpatcher->getNextList();
		int size = tmp.size();
		//printf("\n\n\n\nvector %d\n\n\n\n",size);
	}
	| block_item_list block_item{
		$$.backpatcher = BackPatcher::copy($1.backpatcher);
		std::vector<std::string> tmp1 = $1.backpatcher->getNextList();
		std::vector<std::string> tmp2 = $2.backpatcher->getNextList();
		int size2 = tmp2.size();
		int size1 = tmp1.size();
		//std::cout<<size1<<" "<<size2<<" "<<bpneeded<<std::endl;

		// one of the spots for backpatching if the incoming has no label in newlist and no backpatching is needed
		// Problem - must have a statement following the thing to be backpatched (solved)
		if(size1 > 0 && bpneeded == 1){
			std::string backpatch_label = irgen.new_label();
			std::string S_next = irgen.add_label(backpatch_label);
			$1.ir.code = strdup($$.backpatcher->backPatchNextList(std::string($1.ir.code),backpatch_label).c_str());
			$1.ir.code =  strdup(irgen.concatenate(string($1.ir.code), S_next).c_str());
		}
		else{
			tmp2 = $$.backpatcher->merge(tmp2,$1.backpatcher->getNextList());
		}
		if(size1 == 0 && size2 ==0){}
		else if(size1 >= 1 && size2 == 0){bpneeded = 0;}
		else if(size1 == 0  && size2 >=1){bpneeded = 1;}
		else{bpneeded = 1;}
		$$.backpatcher->assignNextList(tmp2);
		//std::cout<<size<<std::endl;
		$$.ir.code = strdup(irgen.concatenate(string($1.ir.code), string($2.ir.code)).c_str());
		delete $1.backpatcher;
		
		
	}
	;

block_item
	: declaration {
		$$.ir.code = strdup($1.ir.code);
		$$.backpatcher = new BackPatcher();

	}
	| statement{
		$$.ir.code = strdup($1.ir.code);
		$$.backpatcher = BackPatcher::copy($1.backpatcher);
    	delete $1.backpatcher;
		std::vector<std::string> tmp = $$.backpatcher->getNextList();
		int size = tmp.size();
		//printf("\n\n\n\nvector %d\n\n\n\n",size);
	}
	;

expression_statement
	: ';'{
		$$.ir.code =  "";
		$$.backpatcher = new BackPatcher();
	}
	| expression ';' {
		$$.ir.code = strdup($1.ir.code);
		$$.backpatcher = BackPatcher::copy($1.backpatcher);
    	delete $1.backpatcher;
		
	}
	| expression error {yyerrok;}
	;

selection_statement
	: IF '(' expression ')' statement ELSE statement {
		
		// checking if expression is a constant
		HANDLE_BOOL_EXPR_BACKPATCH_FOR_CONSTANTS($3, irgen);

		// Declaration of all the labels to be used
		std::string backpatch_label1 = irgen.new_label();
		std::string backpatch_label2 = irgen.new_label();
		std::string E_true = irgen.add_label(backpatch_label1);
		std::string E_false = irgen.add_label(backpatch_label2);

		// Backpatching
		std::string newIR = $3.backpatcher->backPatchTrueList(std::string($3.ir.code),backpatch_label1);
		newIR = $3.backpatcher->backPatchFalseList(newIR,backpatch_label2);
		$3.ir.code = strdup(newIR.c_str());

		// IR GEN
		std::string S_next = irgen.new_tmp_label();
		std::string goto_S_next = irgen.create_goto(S_next);
		$$.ir.code = strdup(irgen.concatenate(string($3.ir.code),E_true).c_str());
		$$.ir.code = strdup(irgen.concatenate(string($$.ir.code), std::string($5.ir.code)).c_str());
		$$.ir.code = strdup(irgen.concatenate(string($$.ir.code), goto_S_next).c_str());
		$$.ir.code = strdup(irgen.concatenate(string($$.ir.code), E_false).c_str());
		$$.ir.code = strdup(irgen.concatenate(string($$.ir.code), std::string($7.ir.code)).c_str());
		

		$$.backpatcher = new BackPatcher();
		std::vector<std::string> vec1(0),vec2(0);
		if($5.backpatcher != NULL) vec1 = $5.backpatcher->getNextList();
		if($7.backpatcher != NULL) vec2 = $7.backpatcher->getNextList();
		std::vector<string> tmp = $$.backpatcher->merge(vec1,vec2);
		tmp.push_back(S_next);
		$$.backpatcher->assignNextList(tmp);
		bpneeded = 1;
		int size = tmp.size();
		//printf("\n\n\n%d\n\n\n",size);

	}
	| IF '(' expression ')' statement {
		
		// checking if expression is a constant
		HANDLE_BOOL_EXPR_BACKPATCH_FOR_CONSTANTS($3, irgen);

		// Declaration of all the labels to be used
		std::string backpatch_label = irgen.new_label();
		std::string E_true = irgen.add_label(backpatch_label);

		// Backpatching
		std::string newIR = $3.backpatcher->backPatchTrueList(std::string($3.ir.code),backpatch_label);
		$3.ir.code = strdup(newIR.c_str());


		// IR GEN
		$$.ir.code = strdup(irgen.concatenate(string($3.ir.code),E_true).c_str());
		$$.ir.code = strdup(irgen.concatenate(string($$.ir.code), std::string($5.ir.code)).c_str());
		$$.backpatcher = new BackPatcher();
		std::vector<std::string> vec1(0),vec2(0);
		if($5.backpatcher != NULL)vec1 = $5.backpatcher->getNextList();
		if($3.backpatcher != NULL)vec2 = $3.backpatcher->getFalseList();
		$$.backpatcher->assignNextList($$.backpatcher->merge(vec1,vec2));
		bpneeded = 1;
	}
	| SWITCH '(' expression ')' {irgen.start_new_switch();} statement{

		
		//Type Checking
		if( IS_INT_LIKE_TYPE($3.type)){}
		else{
			yyerror("switch quantity not an integer");
		}

		if(st.current_scope_->jump[0] == true){
			yyerror("cannot have continue inside switchcase");
		}
		st.falsekardo();

		CONVERT_BOOL_EXPR_TO_VALUE($3);		

		std::string label11 = irgen.new_label();
		std::string Switch_end = irgen.add_label(label11);
		std::string Switch_start = irgen.generate_switch_cases(std::string($3.ir.tmp));
		if(irgen.has_default_label() == false) Switch_start = irgen.concatenate(Switch_start,"goto " + label11);
		irgen.end_switch();
	
		$$.ir.code = strdup($3.ir.code);
		$$.ir.code = strdup(irgen.concatenate(std::string($$.ir.code),Switch_start).c_str()); 
		$$.ir.code = strdup(irgen.concatenate(std::string($$.ir.code),std::string($6.ir.code)).c_str()); 
		$$.ir.code = strdup($6.backpatcher->staticBackPatch(irgen.break_,std::string($$.ir.code),label11).c_str());
		$$.ir.code = strdup(irgen.concatenate(std::string($$.ir.code),Switch_end).c_str()); 

		$$.backpatcher = new BackPatcher();
	}
	;

iteration_statement
	: WHILE '(' expression ')' statement {
		st.falsekardo();
		// checking if expression is a constant
		HANDLE_BOOL_EXPR_BACKPATCH_FOR_CONSTANTS($3, irgen);
		
		// Declaration of all the labels to be used
		std::string label1 = irgen.new_label();
		std::string S_begin = irgen.add_label(label1);
		std::string label2 = irgen.new_label();
		std::string E_true = irgen.add_label(label2);
		std::string label3 = irgen.new_label();
		std::string Loop_end = irgen.add_label(label3);
		std::string goto_S_begin = irgen.create_goto(label1);

		// Backpatching
		$5.ir.code = strdup($5.backpatcher->backPatchNextList(std::string($5.ir.code),label1).c_str());
		$3.ir.code = strdup($3.backpatcher->backPatchTrueList(std::string($3.ir.code),label2).c_str());
		$3.ir.code = strdup($3.backpatcher->backPatchFalseList(std::string($3.ir.code),label3).c_str());

		// For break and continue
		$5.ir.code = strdup($5.backpatcher->staticBackPatch(irgen.break_,std::string($5.ir.code),Loop_end).c_str());
		$5.ir.code = strdup($5.backpatcher->staticBackPatch(irgen.continue_,std::string($5.ir.code),S_begin).c_str());

		// IR GEN
		$$.ir.code = strdup(irgen.concatenate(S_begin,string($3.ir.code)).c_str());
		$$.ir.code = strdup(irgen.concatenate(string($$.ir.code),E_true).c_str());
		$$.ir.code = strdup(irgen.concatenate(string($$.ir.code), std::string($5.ir.code)).c_str());
		$$.ir.code = strdup(irgen.concatenate(string($$.ir.code), goto_S_begin).c_str());
		$$.ir.code = strdup(irgen.concatenate(string($$.ir.code), Loop_end).c_str());


		$$.backpatcher = new BackPatcher();

	}
	| DO statement WHILE '(' expression ')' ';' {
		st.falsekardo();
		// checking if expression is a constant
		HANDLE_BOOL_EXPR_BACKPATCH_FOR_CONSTANTS($5, irgen);

		// Declaration of all the labels to be used
		std::string label1 = irgen.new_label();
		std::string S_begin = irgen.add_label(label1);
		std::string label2 = irgen.new_label();
		std::string E_begin = irgen.add_label(label2);
		std::string label3 = irgen.new_label();
		std::string Loop_end = irgen.add_label(label3);

		// Backpatching
		$5.ir.code = strdup($5.backpatcher->backPatchTrueList(std::string($5.ir.code),label1).c_str());
		$5.ir.code = strdup($5.backpatcher->backPatchFalseList(std::string($5.ir.code),label3).c_str());

		// For break and continue
		$2.ir.code = strdup($2.backpatcher->staticBackPatch(irgen.break_,std::string($2.ir.code),Loop_end).c_str());
		$2.ir.code = strdup($2.backpatcher->staticBackPatch(irgen.continue_,std::string($2.ir.code),S_begin).c_str());

		// IR GEN
		$$.ir.code = strdup(irgen.concatenate(S_begin,string($2.ir.code)).c_str());
		$$.ir.code = strdup(irgen.concatenate(string($$.ir.code),E_begin).c_str());
		$$.ir.code = strdup(irgen.concatenate(string($$.ir.code),std::string($5.ir.code)).c_str());
		$$.ir.code = strdup(irgen.concatenate(string($$.ir.code),Loop_end).c_str());

		// Progation of nextList
		$$.backpatcher = new BackPatcher();
	}
	| DO statement UNTIL '(' expression ')' ';' {
		st.falsekardo();
		// checking if expression is a constant
		HANDLE_BOOL_EXPR_BACKPATCH_FOR_CONSTANTS($5, irgen);

		// Declaration of all the labels to be used
		std::string label1 = irgen.new_label();
		std::string S_begin = irgen.add_label(label1);
		std::string label2 = irgen.new_label();
		std::string E_begin = irgen.add_label(label2);
		std::string label3 = irgen.new_label();
		std::string Loop_end = irgen.add_label(label3);

		// Backpatching
		$5.ir.code = strdup($5.backpatcher->backPatchTrueList(std::string($5.ir.code),label1).c_str());
		$5.ir.code = strdup($5.backpatcher->backPatchFalseList(std::string($5.ir.code),label3).c_str());

		// For break and continue
		$2.ir.code = strdup($2.backpatcher->staticBackPatch(irgen.break_,std::string($2.ir.code),Loop_end).c_str());
		$2.ir.code = strdup($2.backpatcher->staticBackPatch(irgen.continue_,std::string($2.ir.code),S_begin).c_str());

		// IR GEN
		$$.ir.code = strdup(irgen.concatenate(S_begin,string($2.ir.code)).c_str());
		$$.ir.code = strdup(irgen.concatenate(string($$.ir.code),E_begin).c_str());
		$$.ir.code = strdup(irgen.concatenate(string($$.ir.code),std::string($5.ir.code)).c_str());
		$$.ir.code = strdup(irgen.concatenate(string($$.ir.code),Loop_end).c_str());

		// Progation of nextList
		$$.backpatcher = new BackPatcher();
	}
	| FOR '(' PushScope expression_statement expression_statement ')' statement  {

		st.pop_scope();
		st.falsekardo();
		st.current_scope_->jump[0] = false;
		st.current_scope_->jump[1] = false;
		// checking if expression is a constant
		HANDLE_BOOL_EXPR_BACKPATCH_FOR_CONSTANTS($5, irgen);

		// Declaration of all the labels to be used
		GEN_FOR_LABELS

		// Backpatching
		$5.ir.code = strdup($5.backpatcher->backPatchTrueList(std::string($5.ir.code),label2).c_str());
		$5.ir.code = strdup($5.backpatcher->backPatchFalseList(std::string($5.ir.code),label3).c_str());

		// For break and continue
		$7.ir.code = strdup($7.backpatcher->staticBackPatch(irgen.break_,std::string($7.ir.code),label3).c_str());
		$7.ir.code = strdup($7.backpatcher->staticBackPatch(irgen.continue_,std::string($7.ir.code),label1).c_str());

		// IR GEN
		$$.ir.code = strdup(irgen.concatenate(string($4.ir.code),FOR_begin).c_str());
		$$.ir.code = strdup(irgen.concatenate(string($$.ir.code),string($5.ir.code)).c_str());
		$$.ir.code = strdup(irgen.concatenate(string($$.ir.code),S_begin).c_str());
		$$.ir.code = strdup(irgen.concatenate(string($$.ir.code),string($7.ir.code)).c_str());
		$$.ir.code = strdup(irgen.concatenate(string($$.ir.code),Goto_FOR_begin).c_str());
		$$.ir.code = strdup(irgen.concatenate(string($$.ir.code),FOR_end).c_str());

		$$.backpatcher = new BackPatcher();

	}
	| FOR '(' PushScope expression_statement expression_statement expression ')' statement  {
		st.pop_scope();
		st.falsekardo();
		st.current_scope_->jump[1] = false;
		st.current_scope_->jump[0] = false;
		// checking if expression is a constant
		HANDLE_BOOL_EXPR_BACKPATCH_FOR_CONSTANTS($5, irgen);

		// Declaration of all the labels to be used
		GEN_FOR_LABELS
		
		// Backpatching
		$5.ir.code = strdup($5.backpatcher->backPatchTrueList(std::string($5.ir.code),label2).c_str());
		$5.ir.code = strdup($5.backpatcher->backPatchFalseList(std::string($5.ir.code),label3).c_str());

		// For break and continue
		$8.ir.code = strdup($8.backpatcher->staticBackPatch(irgen.break_,std::string($8.ir.code),label3).c_str());
		$8.ir.code = strdup($8.backpatcher->staticBackPatch(irgen.continue_,std::string($8.ir.code),label1).c_str());

		// IR GEN
		$$.ir.code = strdup(irgen.concatenate(string($4.ir.code),FOR_begin).c_str());
		$$.ir.code = strdup(irgen.concatenate(string($$.ir.code),string($5.ir.code)).c_str());
		$$.ir.code = strdup(irgen.concatenate(string($$.ir.code),S_begin).c_str());
		$$.ir.code = strdup(irgen.concatenate(string($$.ir.code),string($8.ir.code)).c_str());
		$$.ir.code = strdup(irgen.concatenate(string($$.ir.code),string($6.ir.code)).c_str());
		$$.ir.code = strdup(irgen.concatenate(string($$.ir.code),Goto_FOR_begin).c_str());
		$$.ir.code = strdup(irgen.concatenate(string($$.ir.code),FOR_end).c_str());

		$$.backpatcher = new BackPatcher();
	}
	| FOR '(' PushScope declaration expression_statement ')' statement  {
		st.pop_scope();
		st.falsekardo();
		// checking if expression is a constant
		HANDLE_BOOL_EXPR_BACKPATCH_FOR_CONSTANTS($5, irgen);

		// Declaration of all the labels to be used
		GEN_FOR_LABELS

		// Backpatching
		$5.ir.code = strdup($5.backpatcher->backPatchTrueList(std::string($5.ir.code),label2).c_str());
		$5.ir.code = strdup($5.backpatcher->backPatchFalseList(std::string($5.ir.code),label3).c_str());

		// For break and continue
		$7.ir.code = strdup($7.backpatcher->staticBackPatch(irgen.break_,std::string($7.ir.code),label3).c_str());
		$7.ir.code = strdup($7.backpatcher->staticBackPatch(irgen.continue_,std::string($7.ir.code),label1).c_str());

		// IR GEN
		$$.ir.code = strdup(irgen.concatenate(string($4.ir.code),FOR_begin).c_str());
		$$.ir.code = strdup(irgen.concatenate(string($$.ir.code),string($5.ir.code)).c_str());
		$$.ir.code = strdup(irgen.concatenate(string($$.ir.code),S_begin).c_str());
		$$.ir.code = strdup(irgen.concatenate(string($$.ir.code),string($7.ir.code)).c_str());
		$$.ir.code = strdup(irgen.concatenate(string($$.ir.code),Goto_FOR_begin).c_str());
		$$.ir.code = strdup(irgen.concatenate(string($$.ir.code),FOR_end).c_str());

		$$.backpatcher = new BackPatcher();

	}
	| FOR '(' PushScope declaration expression_statement expression ')' statement {
		st.pop_scope();
		st.falsekardo();
		// checking if expression is a constant
		HANDLE_BOOL_EXPR_BACKPATCH_FOR_CONSTANTS($5, irgen);

		// Declaration of all the labels to be used
		GEN_FOR_LABELS

		// Backpatching
		$5.ir.code = strdup($5.backpatcher->backPatchTrueList(std::string($5.ir.code),label2).c_str());
		$5.ir.code = strdup($5.backpatcher->backPatchFalseList(std::string($5.ir.code),label3).c_str());

		// For break and continue
		$8.ir.code = strdup($8.backpatcher->staticBackPatch(irgen.break_,std::string($8.ir.code),label3).c_str());
		$8.ir.code = strdup($8.backpatcher->staticBackPatch(irgen.continue_,std::string($8.ir.code),label1).c_str());

		// IR GEN
		$$.ir.code = strdup(irgen.concatenate(string($4.ir.code),FOR_begin).c_str());
		$$.ir.code = strdup(irgen.concatenate(string($$.ir.code),string($5.ir.code)).c_str());
		$$.ir.code = strdup(irgen.concatenate(string($$.ir.code),S_begin).c_str());
		$$.ir.code = strdup(irgen.concatenate(string($$.ir.code),string($8.ir.code)).c_str());
		$$.ir.code = strdup(irgen.concatenate(string($$.ir.code),string($6.ir.code)).c_str());
		$$.ir.code = strdup(irgen.concatenate(string($$.ir.code),Goto_FOR_begin).c_str());
		$$.ir.code = strdup(irgen.concatenate(string($$.ir.code),FOR_end).c_str());

		$$.backpatcher = new BackPatcher();



	}
	;

jump_statement
	: GOTO IDENTIFIER ';' {
		std::string s = std::string($2.type);
		st.pushlabel(s);
		$$.ir.code = strdup(irgen.create_goto(s).c_str());
		$$.backpatcher = new BackPatcher();
	}
	| CONTINUE ';'{
		//do back jumps here
		std::string s = irgen.new_tmp_label();
		irgen.continue_.push_back(s);
		s = irgen.create_goto(s);
		$$.ir.code = strdup(s.c_str());
		st.current_scope_->contains_break_or_continue = true;
		st.current_scope_->jump[0] = true;
		$$.backpatcher = new BackPatcher();
	}
	| BREAK ';'  {
		//do forward jumps here
		std::string s = irgen.new_tmp_label();
		irgen.break_.push_back(s);
		s = irgen.create_goto(s);
		$$.ir.code = strdup(s.c_str());
		st.current_scope_->contains_break_or_continue = true;
		st.current_scope_->jump[1] = true;
		$$.backpatcher = new BackPatcher();
	}
	| RETURN ';'  {
		std::string s = (st.lookup(currFunc)->type);
		if(s!="VOID"){
			yyerror("\'return\' with no value, in function returning non-void");
		}
		$$.ir.code = "return";
		$$.backpatcher = new BackPatcher();
	}
	| RETURN expression ';'{
		if(contains($2.kind,"PROCEDURE")){
			if(!eq($2.kind,"PROCEDURE")){
				yyerror("incompatible return type");
			}
		}
		std::string s = (st.lookup(currFunc)->type);
		
		s = ts.removeConstFromDeclaration(s);
		s = ts.removeStaticFromDeclaration(s);
		std::string err = std::string($2.type);
		std::string type_change_statement = "";
		if(s==err){}
		else{
			if(!isConvertible(err,s)){
				err += " ";
				err += s;
				err += " The return expression has type different from the declared function";
				yyerror(strdup(err.c_str()));
			}
			else{
				if(!eq($2.type,s.c_str())){
					type_change_statement = formatTypeChange(std::string($2.type),s);
				}
			}
		}
		if(type_change_statement != ""){
			string temp = irgen.new_temp();
			string cd = string($2.ir.code);
			cd +=  temp + " = " + type_change_statement + std::string($2.ir.tmp);
			cd += "\n";
			cd += irgen.return_val(temp);
			$$.ir.code = strdup(cd.c_str());

		}
		else{
			string temp = string($2.ir.tmp);
			string cd = string($2.ir.code);
			cd += "\n";
			cd += irgen.return_val(temp);
			$$.ir.code = strdup(cd.c_str());
		}
		$$.backpatcher = new BackPatcher();
	}
	;

Global
	:  translation_unit{
		final_ir_code = std::string($1.ir.code);
	}

translation_unit
	: external_declaration{
		$$.ir.code = strdup($1.ir.code);
	}
	| translation_unit external_declaration {
		std::string s = irgen.concatenate(string($1.ir.code),string($2.ir.code));
		$$.ir.code = strdup(s.c_str());
	}
	;


external_declaration
	: function_definition {
		$$.ir.code = strdup($1.ir.code);
	}
	| declaration{
		$$.ir.code = strdup($1.ir.code);
		
	}
	;

function_definition
	: declaration_specifiers  declarator  declaration_list {st.push_scope(std::string(strdup($2.name)));block_num++;st.current_scope_->block_num = block_num;} compound_statement {
		if(irgen.get_case_info_size() > 0){
			yyerror("a case label may only be used within a switch");
		}
		if(st.current_scope_->contains_break_or_continue == true){
			yyerror("Using continue or break invalid in this scope.");
		}
		st.pop_scope();
		st.falsekardo();
		yyerror("Declaration before {} is not allowed");
		string temp;
		temp = irgen.concatenate(string($1.ir.code),string($2.ir.code));
		temp = irgen.concatenate(temp,string($3.ir.code));
		temp = irgen.concatenate(temp,string($5.ir.code));
		$$.ir.code = strdup(temp.c_str());
		}
	| declaration_specifiers  declarator {
		st.push_scope(std::string(strdup($2.name)));
		block_num++;
		st.current_scope_->block_num = block_num;
		
		if(eq($2.name,"main")){
			if(!eq($1.type,"INT")){
				yyerror("main must have return type int");
			}
		}
		 st.transferParametersToFunctionScope(std::string(strdup($2.name)));
		 
		
	} 
	compound_statement {
		if(irgen.get_case_info_size() > 0){
			yyerror("a case label may only be used within a switch");
		}
		if(st.current_scope_->contains_break_or_continue == true){
			yyerror("using break/continue invalid in this context");
		}
		st.pop_scope();
		st.falsekardo();
		if(!st.resolvelabels(std::string($2.name))){
			yyerror("GOTO label used but not defined");
		}
		string temp;
		temp = irgen.concatenate(string($1.ir.code),string($2.ir.code));
		temp = irgen.concatenate(temp,concat("func_begin",$2.name));
		temp = irgen.concatenate(temp,string($4.ir.code));
		temp = irgen.concatenate(temp,concat("func_end",$2.name));
		$$.ir.code = strdup(temp.c_str());
	}
	| declaration_specifiers   declarator  error { yyerrok; }
	;

declaration_list
	: declaration{
		$$.ir.code = strdup($1.ir.code);
	}
	| declaration_list declaration {
		std::string s = irgen.concatenate(string($1.ir.code),string($2.ir.code));
		$$.ir.code = strdup(s.c_str());
	}
	;

PushScope
	: {st.push_scope();
	block_num++;
	st.current_scope_->block_num = block_num;
	//cout <<block_num <<"\n";
    }
	;


%%
void yyerror(const char *s) {
	parser_error++;
	fflush(stdout);
    fprintf(stderr, "Syntax Error: %s at line %d\n", s, line_no);
}


main(int argc, char **argv) {
	//yydebug = 1;

	// Check if a filename is passed
	if (argc > 1) {
		FILE* file = fopen(argv[1], "r");
		if (!file) {
			perror("Failed to open input file");
			return 1;
		}
		yyin = file;  // Tell the parser to read from this file
	} else {
		yyin = stdin; // Fall back to stdin (for ./parser < test.c)
	}

int parserresult = yyparse(); // Parser calls yylex() internally
st.print_all_scopes();
if (parserresult == 0 && error_count == 0 && parser_error == 0) {
	printf("LEX and Parsing Success\n");
	std::string filename = std::string(argv[1]);

	irgen.generate(final_ir_code,filename);

	// Create code generator
	CodeGenerator codeGen(final_ir_code, st);

	// Generate and print assembly code
	std::string assemblyCode = codeGen.generateCode();
	std::cout << "Generated Assembly Code:\n" << assemblyCode << std::endl;

	// Print information from all components
	//codeGen.printComponentInfo();

	//st.print_hierarchy();
	//st.print_token_table();
	
	//ts.printAllTypes();
	//ts.printTypedefs();
	//ts.printStaticVariables();
	//st.printTypeSizes();
} else {
	if(error_count > 0){
		printf("Errors in LEX stage:\n PARSING FAILED.");
		print_errors();
	} 
	else {
		printf("Error in parsing with errorcode: %d\n", parserresult);
		
	}
}
return 0;
}