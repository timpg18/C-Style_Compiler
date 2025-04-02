%{

#include "symbol.h"
#include "Utility_func.h"
#include "types.h"
#include "typeconversion.h"

// MACRO for expression checking
// THIS MUST BE ADDED TO CHECK IF ITS A PROCEDURE THEN IT MUST BE CALLED
// and Removing const and static
#define CHECK_PROCEDURE_AND_CLEAN_TYPE(node1, node2) \
    do { \
        if (isPROCEDURE((node1).kind)) { yyerror("incompatible return type "); } \
        if (isPROCEDURE((node2).kind)) { yyerror("incompatible return type "); } \
        type1 = ts.prepareString(std::string((node1).type)); \
        type2 = ts.prepareString(std::string((node2).type)); \
        (node1).type = strdup(type1.c_str()); \
        (node2).type = strdup(type2.c_str()); \
    } while(0)

TypeSet ts;
SymbolTable st;
int classDef = 0;
int isPub=0,isPro=0,isPri=0;
std::string pubMem,proMem,priMem = "";
std::string currFunc = "";
%}

%debug
%union{
	char *id;
	typedef struct {
    char* type;
    char* kind;
	int index;
	char* name;
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
%token STRUCT UNION ENUM 

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
%type <atr> initializer assignment_operator

%type <atr> parameter_type_list parameter_list parameter_declaration abstract_declarator direct_abstract_declarator initializer_list
%type <atr> specifier_qualifier_list type_name
%type <atr> init_declarator_list
%type <atr> argument_expression_list  expression string labeled_statement


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
		$$.type = strdup(st.lookup(tmp)->type.c_str());
		$$.kind = strdup(st.lookup(tmp)->kind.c_str());
		$$.name = name;
		printf("\n\n%s\n\n%s\n\n%s",$$.type,$$.kind,$$.name);
		}
	}
	| constant{
		$$.type = $1.type;
		$$.kind = $1.kind;
		$$.name = $1.name;
	}
	| string {
		$$.type = $1.type;
		$$.name = $1.name;
		$$.kind = $1.kind;
	}
	| '(' expression ')'{
		$$.name = $2.name;
		$$.type = $2.type; 
	}
	| '(' expression error { yyerrok; }
	;

constant
	: I_CONSTANT{
		$$.name = $1.type;
		$$.type = "INT";
		$$.kind = "CONST";
	}		/* includes character_constant */
	| F_CONSTANT{
		$$.name = $1.type;
		$$.type = "FLOAT";
		$$.kind = "CONST";
	}
	| ENUMERATION_CONSTANT{
		$$.type = "INT";
		$$.kind = "ENUM_CONST";
	}	/* after it has been defined as such */
	| BOOLEAN{
		$$.type = "BOOL";
		$$.kind = "CONST";
	}
	;

enumeration_constant		/* before it has been defined as such */
	: IDENTIFIER {
		st.insert_symbol($1.type,"INT" , "ENUM_CONST");
	}
	;

BOOLEAN
	: TRUE 
	| FALSE

string
	: STRING_LIT{
		$$.type = "CHAR*";
		$$.kind = "IDENTIFIER";
	}
	| CHAR_LIT {
		$$.type = "CHAR";
		$$.kind = "IDENTIFIER";
	}
	| FUNC_NAME {

	}
	;


postfix_expression
	: primary_expression {
		$$.type = $1.type;
		$$.kind = $1.kind;
		$$.name = $1.name;
	}
	| postfix_expression '[' expression ']' {
		std::string s = std::string($1.type);
		std::cout<<s<<"\n";
		if(s.back() == '*'){
			s.pop_back();
			$$.type = strdup(s.c_str());
		}
		else{
			yyerror("dereferencing by [ ] invalid");
		}
	}
	| postfix_expression '(' ')'{
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
			if(!eq($1.kind,"CONST")){
				char* func_kind = strdup(st.lookup($1.name)->kind.c_str());
				char* to_check = extract_between_parentheses(func_kind);
				if(eq(to_check,"")==true){
					
				}
				else{
					yyerror("to few arguments passed");
				}
			}
			
		}
		$$.kind = "CONST";
		$$.type = $1.type;
		
	}
	| postfix_expression '(' argument_expression_list ')'{
		if(eq($1.kind,"CONST")){
			yyerror("called object is not a function or function pointer");
		}

		if(eq($1.name,"printf")){
			printf("\n\n%s\n\n",$3.type);
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
			if(!eq($1.kind,"CONST")){
				char* func_kind = strdup(st.lookup($1.name)->kind.c_str());
				char* to_check = extract_between_parentheses(func_kind);
				if(eq(to_check,"")==true){
					
				}
				else{
					yyerror("to few arguments passed");
				}
			}
		}
		$$.kind = "CONST";
		$$.type = $1.type;
		
	}
	| postfix_expression '.' IDENTIFIER{
		//STRUCT TYPE CHECKING HANDLED
		std::string s = std::string($1.name) + "." + std::string($3.type);
		printf("\n\n%s\n\n",$1.type);
		std::cout<<s<<"\n";
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
			if(!ts.hasPointer(std::string($1.type))){

			}
			$$.type = strdup(st.lookup(s)->type.c_str());
			$$.kind = strdup(st.lookup(s)->kind.c_str());
			$$.name = strdup(s.c_str());
		}
		else{
			yyerror(("error:" + std::string($3.type) + " is not a member of the struct").c_str());
		}
		
	}
	| postfix_expression PTR_OP IDENTIFIER{
		//arrow operator, dereferencing then access
		//access ka do it like old
	}
	| postfix_expression INC_OP{
		lvalueError($1.kind);
		if(contains($1.type,"CONST")){
			yyerror("Const value cannot be incremented");
		}
		if(contains($1.type,"struct") || contains($1.type,"union") || contains($1.type,"class") ){
			yyerror("wrong type argument to increment");
		}
		$$.type = $1.type;
		$$.kind = $1.kind;
	}
	| postfix_expression DEC_OP{
		lvalueError($1.kind);
		if(contains($1.type,"CONST")){
			yyerror("Const value cannot be decremented");
		}
		if(contains($1.type,"struct") || contains($1.type,"union") || contains($1.type,"class") ){
			yyerror("wrong type argument to decrement");
		}
		$$.type = $1.type;
		$$.kind = $1.kind;
	}
	;

argument_expression_list
	: assignment_expression{
		$$.type = $1.type;
	}
	| argument_expression_list ',' assignment_expression{
		char* newtype = concat($1.type,$3.type);
		$$.type = newtype;
	}
	;

unary_expression
	: postfix_expression {
		$$.type = $1.type;
		$$.kind = $1.kind;
		$$.name = $1.name;
	}
	| INC_OP unary_expression{
		lvalueError($2.kind);
		if(contains($2.type,"CONST")){
			yyerror("Const value cannot be incremented");
		}
		if(contains($2.type,"struct") || contains($2.type,"union") || contains($2.type,"class") ){
			yyerror("wrong type argument to increment");
		}
		$$.type = $2.type;
		$$.kind = $2.kind;
	}
	| DEC_OP unary_expression{
		lvalueError($2.kind);
		if(contains($2.type,"CONST")){
			yyerror("Const value cannot be decremented");
		}
		if(contains($2.type,"struct") || contains($2.type,"union") || contains($2.type,"class") ){
			yyerror("wrong type argument to decrement");
		}
		$$.type = $2.type;
		$$.kind = $2.kind;
	}
	| unary_operator cast_expression{
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
		$$.name = $2.name;
		$$.kind = $2.kind;

	}
	| SIZEOF unary_expression{
		$$.type = "INT";
		$$.kind = "CONST";
	}
	| SIZEOF '(' type_name ')'{
		$$.type = "INT";
		$$.kind = "CONST";
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
	}
	| '(' type_name ')' cast_expression{

		if(!ts.contains(std::string($2.type))){
			yyerror("the following type is not a valid type");
		}
		else if(contains($2.type,"enum") || contains($2.type,"class") || contains($2.type,"struct") || contains($2.type,"union")){
			yyerror("the following type is not a valid type");
		}
		printf("\n\n%s\n\n",$2.type);
		$$.type = $2.type;
		$$.kind = $4.kind;
	}
	| '(' type_name error cast_expression { yyerrok; }
	;

multiplicative_expression
	: cast_expression {
		$$.type = $1.type;
		$$.kind = $1.kind;
		$$.name = $1.name;
	}
	| multiplicative_expression '*' cast_expression {
		std::string type1,type2 ;
		CHECK_PROCEDURE_AND_CLEAN_TYPE($1,$3);
		
		// Checking for pointer as '*' in pointers not valid 
		if(ts.hasPointer(type1) || ts.hasPointer(type2)){
			yyerror("invalid operator to pointers");
		}
		check_type($1.type, $3.type, "incompatible type expression involved in *: ");
		
		
	}
	| multiplicative_expression '/' cast_expression{
		std::string type1,type2 ;
		CHECK_PROCEDURE_AND_CLEAN_TYPE($1,$3);

		// Checking for pointer as '/' in pointers not valid 
		if(ts.hasPointer(type1) || ts.hasPointer(type2)){
			yyerror("invalid operator to pointers");
		}
		check_type($1.type, $3.type, "incompatible type expression involved in *: ");
	}
	| multiplicative_expression '%' cast_expression{
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
	}
	;

additive_expression
	: multiplicative_expression {
		$$.type = $1.type;
		$$.kind = $1.kind;
		$$.name = $1.name;
	}
	| additive_expression '+' multiplicative_expression{
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
		

	}
	| additive_expression '-' multiplicative_expression{
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
	}
	;

shift_expression
	: additive_expression {
		$$.type = $1.type;
		$$.kind = $1.kind;
		$$.name = $1.name;
	}
	| shift_expression LEFT_OP additive_expression{
		std::string type1,type2 ;
		CHECK_PROCEDURE_AND_CLEAN_TYPE($1,$3);

		if(eq($1.type, "INT") == false || eq($3.type, "INT")==false){
			char *err = "both operands must be int, int but are ";
        	err = concat(err,$1.type);
        	err = concat(err,  $3.type);
        	yyerror(err);
		}
		$$.type = "INT";
	}
	| shift_expression RIGHT_OP additive_expression{
		std::string type1,type2 ;
		CHECK_PROCEDURE_AND_CLEAN_TYPE($1,$3);

		if(eq($1.type, "INT") == false || eq($3.type, "INT")==false){
			char *err = "both operands must be int, int but are ";
        	err = concat(err,$1.type);
        	err = concat(err, $3.type );
        yyerror(err);
		}
		$$.type = "INT";
	}
	;

relational_expression
	: shift_expression {
		$$.type = $1.type;
		$$.kind = $1.kind;
		$$.name = $1.name;
	}
	| relational_expression '<' shift_expression{
		std::string type1,type2 ;
		CHECK_PROCEDURE_AND_CLEAN_TYPE($1,$3);

		check_type($1.type, $3.type,"incompatible type expression involved in < : ");
		$$.type = "INT";
	}
	| relational_expression '>' shift_expression{
		std::string type1,type2 ;
		CHECK_PROCEDURE_AND_CLEAN_TYPE($1,$3);

		check_type($1.type, $3.type,"incompatible type expression involved in >: ");
		$$.type = "INT";
	}
	| relational_expression LE_OP shift_expression{
		std::string type1,type2 ;
		CHECK_PROCEDURE_AND_CLEAN_TYPE($1,$3);

		check_type($1.type, $3.type,"incompatible type expression involved in <=: ");
		$$.type = "INT";
	}
	| relational_expression GE_OP shift_expression{
		std::string type1,type2 ;
		CHECK_PROCEDURE_AND_CLEAN_TYPE($1,$3);

		check_type($1.type, $3.type,"incompatible type expression involved in >=: ");
		$$.type = "INT";
	}
	;

equality_expression
	: relational_expression {
		$$.type = $1.type;
		$$.kind = $1.kind;
		$$.name = $1.name;
	}
	| equality_expression EQ_OP relational_expression{
		std::string type1,type2 ;
		CHECK_PROCEDURE_AND_CLEAN_TYPE($1,$3);

		check_type($1.type, $3.type, "incompatible type expression involved in = : ");
		$$.type = "INT";
	}
	| equality_expression NE_OP relational_expression{
		std::string type1,type2 ;
		CHECK_PROCEDURE_AND_CLEAN_TYPE($1,$3);

		check_type($1.type, $3.type, "incompatible type expression involved in != : ");
		$$.type = "INT";
	}
	;

and_expression
	: equality_expression {
		$$.type = $1.type;
		$$.kind = $1.kind;
		$$.name = $1.name;
	}
	| and_expression '&' equality_expression {
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
	}
	;

exclusive_or_expression
	: and_expression {
		$$.type = $1.type;
		$$.kind = $1.kind;
		$$.name = $1.name;
	}
	| exclusive_or_expression '^' and_expression{
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
	}
	;

inclusive_or_expression
	: exclusive_or_expression {
		$$.type = $1.type;
		$$.kind = $1.kind;
		$$.name = $1.name;
	}
	| inclusive_or_expression '|' exclusive_or_expression{
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
	}
	;

logical_and_expression
	: inclusive_or_expression {
		$$.type = $1.type;
		$$.kind = $1.kind;
		$$.name = $1.name;
	}
	| logical_and_expression AND_OP inclusive_or_expression{
		std::string type1,type2 ;
		CHECK_PROCEDURE_AND_CLEAN_TYPE($1,$3);

		check_type($1.type, $3.type,"incompatible type expression involved in &&: ");
		$$.type = "INT";
	}
	;

logical_or_expression
	: logical_and_expression {
		$$.type = $1.type;
		$$.kind = $1.kind;
		$$.name = $1.name;
	}
	| logical_or_expression OR_OP logical_and_expression{
		std::string type1,type2 ;
		CHECK_PROCEDURE_AND_CLEAN_TYPE($1,$3);

		check_type($1.type, $3.type, "incompatible type expression involved in || = : ");
		$$.type = "INT";
	}
	;

conditional_expression
	: logical_or_expression {
		$$.type = $1.type;
		$$.kind = $1.kind;
		$$.name = $1.name;
	}
	| logical_or_expression '?' expression ':' conditional_expression {
		//DO WE NEED TYPE CHECKING HERE? I DOUBT IT
	}
	;

assignment_expression
	: conditional_expression{
		$$.type = $1.type;
		$$.kind = $1.kind;
		$$.name = $1.name;
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
				printf("\n\n%s\n\n%s\n\n",$3.kind,$3.type);
				if(contains($1.type,"CONST")){
					yyerror("const variable cannot be re-changed");
				}
				// ONLY INT TYPE VALUES CAN BE ASSIGNED 
				if(eq($3.kind, "ENUM_CONST") || eq($3.kind,"IDENTIFIER") || eq($3.kind, "CONST") ){
					if(!eq($3.type,"INT")){
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
				
			}
			
		}
		$$.type = original ;
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
	}
	| expression ',' assignment_expression
	;

constant_expression
	: conditional_expression	/* with constraints */
	;

declaration
	: declaration_specifiers ';'
	| declaration_specifiers init_declarator_list ';'{
		printf("\n\n%s\n\n",$2.name);
		std::cout<<"HELL"<<std::endl;
		
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
			st.updateVariableTypes(std::string($2.name),std::string($2.type));
		}
		else{
			// adding typedef to the TypeSet ts
			if(contains($1.type,"typedef")){
				ts.addTypedef(std::string($2.name),std::string($1.type));
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

				printf("\n\n%s\n\n",combined);

			  if(ts.storagespecifercount(std::string($$.type))>=2){
					yyerror("Multiple storage class specifiers not allowed");
			  }
		}
	| storage_class_specifier{
		$$.type = $1.type;
        currentType = $$.type;
	}
	| type_specifier declaration_specifiers
	      {
              /* Concatenate the current type_specifier ($1) with the following declaration_specifiers ($2).
                 A space is inserted between the two parts. */
              char *combined = (char*)malloc(strlen($1.type) + strlen($2.type) + 2);
              sprintf(combined, "%s %s", $1.type, $2.type);
              $$.type = combined;
              currentType = $$.type; /* Update the global currentType */

          }
	| type_specifier 
	      { 
              $$.type = $1.type; 
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
		}
	| type_qualifier
	      { $$.type = $1.type; 
		  currentType = $$.type;
		  }
	| declaration_specifiers '&'
	;


init_declarator_list
	: init_declarator {
		$$.name = $1.name;
		$$.type = $1.type;
	}
	| init_declarator_list ',' init_declarator {
		$$.name = concat($1.name,$3.name);
		$$.type = $1.type;
	}
	;

init_declarator
	: declarator '=' initializer {
		// the left of the declarator muse be lvalue;
			lvalueError($1.kind);

			// If a procedure then must be called procedure
			if(isPROCEDURE($3.kind)){yyerror("Cannot assign function to a variable");}

			//remove static
			std::string t = ts.removeStaticFromDeclaration(std::string($1.type));
			char* temp = strdup(t.c_str());

			// remove static and const
			t = ts.removeStaticFromDeclaration(std::string($3.type));
			char* temp2 = strdup(t.c_str());

			printf("\n%s\n%s\n",temp,temp2);


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

					printf("\n\n%s\n\n%s\n\n",$3.kind,$3.type);
					// ENUM CAN BE INITIALIZED BY ENUM_CONST , IDENTIFIER , CONST 
					if(eq($3.kind, "ENUM_CONST") || eq($3.kind,"IDENTIFIER") || eq($3.kind, "CONST") ){
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
					printf("\n%s\n%s\n",matchingType1,matchingType2);
					if(ts.hasPointer(std::string(matchingType1)) || ts.hasPointer(std::string(matchingType2))){
						check_type(matchingType1, matchingType2, "incompatible type involved : ");
					}
					else if(isConvertible(std::string(matchingType1),std::string(matchingType2))){
						$$.type = concat($$.type,"declared");  //adding this to show that the const variable is declared (to be only used for declaration of const);
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
					$$.type = $1.type;
				}	
			}
			else{
				
				// IF THE TYPES ARE SAME
				printf("\n%s\n%s\n",$1.kind,$3.kind);
				$$.type = $1.type;

				// If a porcedure then must be called procedure
				if(isPROCEDURE($3.kind)){
					yyerror("Cannot assign function to a variable");
				}
				else if(contains($1.type,"CONST")){
					$$.type = concat($$.type, "declared");
				}
			}	
	}
	| declarator {
		$$.name = $1.name;
		$$.type = $1.type;
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
    : CLASS '{' {st.push_scope("class anonymous");} class_member_list '}' PopScope
         {
		 $$.type = (char*)malloc(strlen("class") + 14); 
         sprintf($$.type, "class (anonymous)");
		 yyerror("anonymous class definition not allowed");
		   }
	| CLASS IDENTIFIER base_clause_opt  '{' 
		{   
			classDef=1;
			std::string s = std::string("class ") + std::string(strdup($2.type));
			st.insert_symbol(std::string($2.type),"CLASS","USER DEFINED");
			st.push_scope( std::string("class ")+ std::string(strdup($2.type)));
			ts.addClass(std::string($2.type));
		}
		class_member_list '}' 
		{
			classDef=0;
			std::cout<<priMem<<"\n"<<proMem<<"\n"<<pubMem<<"\n";
			st.update_class_types(priMem,pubMem,proMem);
			priMem = "";
			proMem = "";
			pubMem = ""; 
		}
		PopScope
        { 
		    $$.type = (char*)malloc( strlen("class") + strlen($2.type) + 14 ); // one space plus null
         	sprintf($$.type, "class %s", $2.type);
		   
		}
	| CLASS IDENTIFIER base_clause_opt  
         { 
           $$.type = (char*)malloc(strlen("class") + strlen($2.type) + 14);
           sprintf($$.type, "class %s", $2.type);
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
    : ':' base_specifier_list { $$.type = $2.type; }
    | /* empty */ { $$.type = NULL; }
    ;

base_specifier_list
    : base_specifier { $$.type = $1.type; }
    | base_specifier_list ',' base_specifier 
         { 
           /* Concatenate the list, e.g., "public Base1, private Base2" */
           char *tmp = (char*)malloc(strlen($1.type) + strlen($3.type) + 14);
           sprintf(tmp, "%s, %s", $1.type, $3.type);
           $$.type= tmp;
         }
    ;

/* A single base specifier with an optional access specifier */
base_specifier
    : access_specifier_opt IDENTIFIER
         { 
           if ($1.type)
           {
              $$.type = (char*)malloc(strlen($1.type) + strlen($2.type) + 14);
              sprintf($$.type, "%s %s", $1.type, $2.type);
           }
           else
           {
              $$.type = strdup($2.type);
           }
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
	 } struct_declaration_list '}' PopScope {
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
				ts.addStruct(std::string($2.type));
			}
			else{
				st.insert_symbol(std::string($2.type),"UNION","USER DEFINED");
				ts.addUnion(std::string($2.type));
			}
			st.push_scope(s);
			}
		 struct_declaration_list '}' PopScope {
         /* Named struct/union with body */
         $$.type = (char*)malloc(strlen($1.type) + strlen($2.type) + 2); // one space plus null
         sprintf($$.type, "%s %s", $1.type, $2.type);
    }
	| struct_or_union IDENTIFIER {
         /* Named struct/union declaration without body */
         $$.type = (char*)malloc(strlen($1.type) + strlen($2.type) + 2);
         sprintf($$.type, "%s %s", $1.type, $2.type);
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
	: specifier_qualifier_list ';'	/* for anonymous struct/union */
	| specifier_qualifier_list struct_declarator_list ';'
	;

specifier_qualifier_list
	: type_specifier specifier_qualifier_list {
		$$.type = $1.type;
		$$.type = concat($$.type,$2.type);
	}
	| type_specifier{
		$$.type = $1.type;
	}
	| type_qualifier specifier_qualifier_list
	| type_qualifier
	;

struct_declarator_list
	: struct_declarator
	| struct_declarator_list ',' struct_declarator
	;

struct_declarator
	: ':' constant_expression
	| declarator ':' constant_expression
	| declarator
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
      }
    | ENUM IDENTIFIER '{' enumerator_list ',' '}' { 
          char *tmp = (char*)malloc(strlen("enum") + strlen($2.type) + 2);
          sprintf(tmp, "enum %s", $2.type);
          $$.type = tmp;
		  ts.addEnum(std::string($2.type));
		  st.insert_symbol(std::string($2.type),"ENUM","USER DEFINED");
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
	: enumeration_constant '=' constant_expression
	| enumeration_constant
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
		  std::cout<<"HELLO"<<std::endl;
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
      }
	| direct_declarator {
		 $$.index = $1.index;
		 $$.type = $1.type;
		 $$.kind = $1.kind;
		 $$.name = $1.name;
		  }
	;

direct_declarator
	: IDENTIFIER{
		std::string tmp = $1.type;
		if(st.lookup(tmp) != nullptr){
			//THIS GIVES ERROR EVEN IF IN PARENT SCOPE... (ie conflict with parent scope)
			std::string err = st.lookup(tmp)->name + " already declared before: ";
			yyerror(err.c_str());
		}
		else{
			st.insert_symbol($1.type, currentType ? currentType : "INVALID", "IDENTIFIER");
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
			
        $$.index = st.token_table_.size() - 1;
		$$.type = currentType;
		$$.kind = "IDENTIFIER";
		$$.name = $1.type;
		}
        
    }
	| '(' declarator ')' {
		 $$.index = $2.index;
		$$.name = $2.name;
	 }
	| direct_declarator '[' ']'{
          int idx = $1.index;
          char newType[256];
          sprintf(newType, "%s*",st.token_table_[idx].token_type);
           st.token_table_[idx].token_type += "*";
		   $$.name = $1.name;
		   st.lookup(std::string($1.name))->type = st.token_table_[idx].token_type;
			$$.type = strdup(st.token_table_[idx].token_type.c_str());
		  if (strstr(newType, "typedef") != NULL){
            char *temp = new char[st.token_table_[idx].token.size()+1];
            std::strcpy(temp,st.token_table_[idx].token.c_str());
			update_symtab(temp);
			
		  }
          $$.index = idx;
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
			printf("\n\n%s\n\n%s\n\n",$$.type,$3.name);

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
		std::cout<<"\n\n"<<currFunc<<"\n\n";
		if(!ts.contains(st.lookup(std::string($1.name))->type)){
			yyerror("Specified function return type not allowed");
		}
		std::cout<<"\n\n"<<std::string($3.name)<<"\n\n";
		// here we are having 3 important steps 
		// 1. update the identifier kind to PARAMETER
		// 2. update the PARAMETER sizes to 0 and change offset
		// 3. update the size of procedure to 0 since it is called 
		st.changeToParameter(std::string($3.name));
		st.updateParameterSizes();
		st.updateProcedureSize(std::string($$.name));
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
		std::cout<<"\n\n"<<currFunc<<"\n\n";
		if(!ts.contains(st.lookup(std::string($1.name))->type)){
			yyerror("Specified function return type not allowed");
		}
		// since no parameter_list so directly updating procedure size to 0
		st.updateProcedureSize(std::string($$.name));

	   
    }
	| direct_declarator '('  identifier_list ')'{

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
	}
	;

parameter_list
	: parameter_declaration{ //Progation of type
		$$.type = $1.type;
		$$.name = $1.name;
	}
	| parameter_list ',' parameter_declaration { // Concatenate the types
		char* newtype = concat($1.type,$3.type);
		$$.type = newtype;
		char* newname = concat($1.name,$3.name);
		$$.name = newname;
	}
	;

parameter_declaration
	: declaration_specifiers declarator { //Progation of type
		if(!ts.contains(std::string($1.type))){
			yyerror("parameter type not allowed");
		}
		$$.type = $2.type;
		$$.name = $2.name;
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
		std::string s = std::string($2.type);
		s += "*";
		$$.type = strdup(s.c_str());
	}
	| assignment_expression {
		$$.type = $1.type;
		$$.kind = $1.kind;
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
	: labeled_statement
	|  {st.push_scope();} compound_statement {st.pop_scope();}
	| expression_statement
	| selection_statement
	| iteration_statement
	| jump_statement
	;

labeled_statement
	: IDENTIFIER ':'{
		st.insert_symbol($1.type,"LABEL" , "GOTO LABEL");
	} statement 
	| CASE constant_expression ':' statement
	| DEFAULT ':' statement
	;

compound_statement
	: '{'   '}' 
	| '{'  block_item_list '}' 
	| '{'   block_item_list error {  
     yyerrok;}
	;

block_item_list
	: block_item
	| block_item_list block_item
	;

block_item
	: declaration
	| statement
	;

expression_statement
	: ';'
	| expression ';'
	| expression error {yyerrok;}
	;

selection_statement
	: IF '(' expression ')' statement ELSE statement
	| IF '(' expression ')' statement
	| SWITCH '(' expression ')' statement
	;

iteration_statement
	: WHILE '(' expression ')' statement
	| DO statement WHILE '(' expression ')' ';'
	| DO statement UNTIL '(' expression ')' ';'
	| FOR '(' PushScope expression_statement expression_statement ')' statement PopScope
	| FOR '(' PushScope expression_statement expression_statement expression ')' statement PopScope
	| FOR '(' PushScope declaration expression_statement ')' statement PopScope
	| FOR '(' PushScope declaration expression_statement expression ')' statement PopScope
	;

jump_statement
	: GOTO IDENTIFIER ';' {
		st.pushlabel(std::string($2.type));
	}
	| CONTINUE ';'
	| BREAK ';'  
	| RETURN ';'  {
		std::string s = (st.lookup(currFunc)->type);
		if(s!="VOID"){
			yyerror("\'return\' with no value, in function returning non-void");
		}
	}
	| RETURN expression ';'{
		if(contains($2.kind,"PROCEDURE")){
			if(!eq($2.kind,"PROCEDURE")){
				yyerror("incompatible return type");
			}
		}
		std::string s = (st.lookup(currFunc)->type);
		std::string typeExp = std::string($2.type);
		s = ts.removeConstFromDeclaration(s);
		s = ts.removeStaticFromDeclaration(s);
		std::cout<<typeExp<<std::endl;
		if(s==typeExp){}
		else{
			if(!isConvertible(typeExp,s)){
				yyerror("The return expression has type different from the declared function");
			}
			
		}
	}
	;

Global
	:  translation_unit 

translation_unit
	: external_declaration
	| translation_unit external_declaration
	;


external_declaration
	: function_definition
	| declaration
	;

function_definition
	: declaration_specifiers  declarator  declaration_list {st.push_scope(std::string(strdup($2.name)));} compound_statement {
		st.pop_scope();
		yyerror("Declaration before {} is not allowed");
		}
	| declaration_specifiers  declarator {
		st.push_scope(std::string(strdup($2.name)));
		if(eq($2.name,"main")){
			if(!eq($1.type,"INT")){
				yyerror("main must have return type int");
			}
		}
	} 
	compound_statement {
		st.pop_scope();
		if(!st.resolvelabels(std::string($2.name))){
			yyerror("GOTO label used but not defined");
		}
	}
	| declaration_specifiers   declarator  error { yyerrok; }
	;

declaration_list
	: declaration
	| declaration_list declaration
	;

PushScope
	: {st.push_scope();
    }
	;

PopScope
	: {st.pop_scope();
    }
	;


%%
void yyerror(const char *s) {
	parser_error++;
	fflush(stdout);
    fprintf(stderr, "Syntax Error: %s at line %d\n", s, line_no);
}


main(int argc, char **argv) {
	 yydebug = 1;
int parserresult = yyparse(); // Parser calls yylex() internally

if (parserresult == 0 && error_count == 0 && parser_error == 0) {
	printf("LEX and Parsing Success\n");
	
	st.print_hierarchy();
	//st.print_token_table();
	//st.print_all_scopes();
	//ts.printAllTypes();
	//ts.printTypedefs();
	//ts.printStaticVariables();
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