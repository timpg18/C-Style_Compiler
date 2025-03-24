%{

#include "symbol.h"
SymbolTable st;

%}
%debug
%union{
	char *id;
    int index;
}

%token <id> IDENTIFIER 
%token I_CONSTANT F_CONSTANT STRING_LIT CHAR_LIT
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
%token <id> CLASS 
%token PUBLIC PRIVATE PROTECTED

%type <index> direct_declarator declarator 
%type <id> struct_or_union_specifier struct_or_union
%type <id> enum_specifier
%type <id> constant_expression assignment_expression
%type <id> pointer type_qualifier_list
%type <id> declaration_specifiers type_specifier storage_class_specifier type_qualifier conditional_expression
%type <id> class_specifier access_specifier base_clause_opt base_specifier_list base_specifier  access_specifier_opt

/* currently removed for now 
ALIGNAS ALIGNOF ATOMIC GENERIC NORETURN STATIC_ASSERT THREAD_LOCAL
ELLIPSIS COMPLEX IMAGINARY COMPLEX IMAGINARY 
*/
%%

primary_expression
	: IDENTIFIER{
		char *name = strdup($1);
		std::string tmp = name;

		if(st.lookup(tmp) == false){
			std::string err = "Undeclared Identifier: " + tmp;
    		yyerror(err.c_str());
			
		}
	}
	| constant
	| string
	| '(' expression ')'
	| '(' expression error { yyerrok; }
	;

constant
	: I_CONSTANT		/* includes character_constant */
	| F_CONSTANT
	| ENUMERATION_CONSTANT	/* after it has been defined as such */
	;

enumeration_constant		/* before it has been defined as such */
	: IDENTIFIER {
		st.insert_symbol($1, "enumeration_constant");
	}
	;

string
	: STRING_LIT
	| CHAR_LIT   /* BEEP BEEP , adding char literals here */
	| FUNC_NAME
	;


postfix_expression
	: primary_expression
	| postfix_expression '[' expression ']'
	| postfix_expression '(' ')'
	| postfix_expression '(' argument_expression_list ')'
	| postfix_expression '.' IDENTIFIER
	| postfix_expression PTR_OP IDENTIFIER
	| postfix_expression INC_OP
	| postfix_expression DEC_OP
	| '(' type_name ')' '{' initializer_list '}'
	| '(' type_name ')' '{' initializer_list ',' '}'
	;

argument_expression_list
	: assignment_expression
	| argument_expression_list ',' assignment_expression
	;

unary_expression
	: postfix_expression
	| INC_OP unary_expression
	| DEC_OP unary_expression
	| unary_operator cast_expression
	| SIZEOF unary_expression
	| SIZEOF '(' type_name ')'
	;

unary_operator
	: '&'
	| '*'
	| '+'
	| '-'
	| '~'
	| '!'
	;

cast_expression
	: unary_expression
	| '(' type_name ')' cast_expression
	| '(' type_name error cast_expression { yyerrok; }
	;

multiplicative_expression
	: cast_expression
	| multiplicative_expression '*' cast_expression
	| multiplicative_expression '/' cast_expression
	| multiplicative_expression '%' cast_expression
	;

additive_expression
	: multiplicative_expression
	| additive_expression '+' multiplicative_expression
	| additive_expression '-' multiplicative_expression
	;

shift_expression
	: additive_expression
	| shift_expression LEFT_OP additive_expression
	| shift_expression RIGHT_OP additive_expression
	;

relational_expression
	: shift_expression
	| relational_expression '<' shift_expression
	| relational_expression '>' shift_expression
	| relational_expression LE_OP shift_expression
	| relational_expression GE_OP shift_expression
	;

equality_expression
	: relational_expression
	| equality_expression EQ_OP relational_expression
	| equality_expression NE_OP relational_expression
	;

and_expression
	: equality_expression
	| and_expression '&' equality_expression
	;

exclusive_or_expression
	: and_expression
	| exclusive_or_expression '^' and_expression
	;

inclusive_or_expression
	: exclusive_or_expression
	| inclusive_or_expression '|' exclusive_or_expression
	;

logical_and_expression
	: inclusive_or_expression
	| logical_and_expression AND_OP inclusive_or_expression
	;

logical_or_expression
	: logical_and_expression
	| logical_or_expression OR_OP logical_and_expression
	;

conditional_expression
	: logical_or_expression
	| logical_or_expression '?' expression ':' conditional_expression
	;

assignment_expression
	: conditional_expression
	| unary_expression assignment_operator assignment_expression
	;

assignment_operator
	: '='
	| MUL_ASSIGN
	| DIV_ASSIGN
	| MOD_ASSIGN
	| ADD_ASSIGN
	| SUB_ASSIGN
	| LEFT_ASSIGN
	| RIGHT_ASSIGN
	| AND_ASSIGN
	| XOR_ASSIGN
	| OR_ASSIGN
	;

expression
	: assignment_expression
	| expression ',' assignment_expression
	;

constant_expression
	: conditional_expression	/* with constraints */
	;

declaration
	: declaration_specifiers ';'
	| declaration_specifiers init_declarator_list ';'
	| declaration_specifiers error { yyerrok; }
    | declaration_specifiers init_declarator_list error {yyerrok;}
	;

declaration_specifiers
	: storage_class_specifier declaration_specifiers
	      { /* You may choose to ignore storage class in the complete type, or handle it separately. */
			char *combined = (char*)malloc(strlen($1) + strlen($2) + 2);
              sprintf(combined, "%s %s", $1, $2);
              $$ = combined;
              currentType = $$;
              free($1);
              free($2);
		   }
	| storage_class_specifier{
		$$ = $1;
        currentType = $$;
	}
	| type_specifier declaration_specifiers
	      {
              /* Concatenate the current type_specifier ($1) with the following declaration_specifiers ($2).
                 A space is inserted between the two parts. */
              char *combined = (char*)malloc(strlen($1) + strlen($2) + 2);
              sprintf(combined, "%s %s", $1, $2);
              $$ = combined;
              currentType = $$; /* Update the global currentType */
          }
	| type_specifier 
	      { 
              $$ = $1; 
              currentType = $$; 
          }
	| type_qualifier declaration_specifiers
	    { 
			char *combined = (char*)malloc(strlen($1) + strlen($2) + 2);
			sprintf(combined, "%s %s", $1, $2);
			$$ = combined;
			currentType = $$; 
			$$ = $2; 
		}
	| type_qualifier
	      { $$ = $1; 
		  currentType = $$;
		  }
	| declaration_specifiers '&'
	;


init_declarator_list
	: init_declarator
	| init_declarator_list ',' init_declarator
	;

init_declarator
	: declarator '=' initializer
	| declarator
	;

storage_class_specifier
	: TYPEDEF	{ $$ = strdup("typedef"); }/* identifiers must be flagged as TYPEDEF_NAME */
	| EXTERN { $$ = strdup("extern"); }
	| STATIC { $$ = strdup("static"); }
	| AUTO { $$ = strdup("auto"); }
	| REGISTER { $$ = strdup("register"); }
	;

type_specifier
	: VOID { currentType = strdup("VOID"); $$ = currentType; }
	| CHAR { currentType = strdup("CHAR"); $$ = currentType; }
	| SHORT { currentType = strdup("SHORT"); $$ = currentType; }
	| INT { currentType = strdup("INT"); $$ = currentType; }
	| LONG { currentType = strdup("LONG"); $$ = currentType; }
	| FLOAT { currentType = strdup("FLOAT"); $$ = currentType; }
	| DOUBLE { currentType = strdup("DOUBLE"); $$ = currentType; }
	| SIGNED { currentType = strdup("SIGNED"); $$ = currentType; }
	| UNSIGNED { currentType = strdup("UNSIGNED"); $$ = currentType; }
	| BOOL { currentType = strdup("BOOL"); $$ = currentType; }
	| struct_or_union_specifier { currentType = $1; $$ = currentType; }
	| class_specifier {currentType = $1; $$ = currentType;}
	| enum_specifier { currentType = $1; $$ = currentType; }
	| TYPEDEF_NAME	{ currentType = strdup(yytext); $$ = currentType;}
	;

class_specifier
    : CLASS '{' PushScope class_member_list '}' PopScope
         {
		 $$ = (char*)malloc(strlen("class") + 14); 
         sprintf($$, "class (anonymous)");
		   }
	| CLASS IDENTIFIER base_clause_opt  '{' PushScope class_member_list '}' PopScope
         { 
		    $$ = (char*)malloc( strlen("class") + strlen($2) + 14 ); // one space plus null
         sprintf($$, "class %s", $2);
		   
		   }
	| CLASS IDENTIFIER base_clause_opt  
         { 
           $$ = (char*)malloc(strlen("class") + strlen($2) + 14);
           sprintf($$, "class %s", $2);
		 }
    ;


class_member_list
    : class_member
    | class_member_list class_member
    ;

class_member
    : access_specifier ':' '{' translation_unit '}'
    ;


access_specifier
    : PUBLIC { $$ = strdup("public"); }
    | PRIVATE { $$ = strdup("private"); }
    | PROTECTED { $$ = strdup("protected"); }
    ;
/* Optional inheritance clause */
base_clause_opt
    : ':' base_specifier_list { $$ = $2; }
    | /* empty */ { $$ = NULL; }
    ;

base_specifier_list
    : base_specifier { $$ = $1; }
    | base_specifier_list ',' base_specifier 
         { 
           /* Concatenate the list, e.g., "public Base1, private Base2" */
           char *tmp = (char*)malloc(strlen($1) + strlen($3) + 14);
           sprintf(tmp, "%s, %s", $1, $3);
           $$ = tmp;
         }
    ;

/* A single base specifier with an optional access specifier */
base_specifier
    : access_specifier_opt IDENTIFIER
         { 
           if ($1)
           {
              $$ = (char*)malloc(strlen($1) + strlen($2) + 14);
              sprintf($$, "%s %s", $1, $2);
           }
           else
           {
              $$ = strdup($2);
           }
         }
    ;

/* Optional access specifier in the base clause */
access_specifier_opt
	: /* empty */ {$$ = NULL;}
	| access_specifier {$$ = $1;}
	;

struct_or_union_specifier
	: struct_or_union '{' PushScope struct_declaration_list '}' PopScope {
         /* Anonymous struct or union */
         $$ = (char*)malloc(strlen($1) + 14); // Enough for " (anonymous)" and '\0'
         sprintf($$, "%s (anonymous)", $1);
    }
	| struct_or_union IDENTIFIER '{' PushScope struct_declaration_list '}' PopScope {
         /* Named struct/union with body */
         $$ = (char*)malloc(strlen($1) + strlen($2) + 2); // one space plus null
         sprintf($$, "%s %s", $1, $2);
    }
	| struct_or_union IDENTIFIER {
         /* Named struct/union declaration without body */
         $$ = (char*)malloc(strlen($1) + strlen($2) + 2);
         sprintf($$, "%s %s", $1, $2);
    }
	;

struct_or_union
	: STRUCT{ $$ = strdup("struct"); }
	| UNION { $$ = strdup("union"); }
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
	: type_specifier specifier_qualifier_list
	| type_specifier
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
          $$ = strdup("enum");
      }
    | ENUM '{' enumerator_list ',' '}' { 
          $$ = strdup("enum");
      }
    | ENUM IDENTIFIER '{' enumerator_list '}' { 
          /* Named enum: produce "enum <identifier>" */
          char *tmp = (char*)malloc(strlen("enum") + strlen($2) + 2);
          sprintf(tmp, "enum %s", $2);
          $$ = tmp;
      }
    | ENUM IDENTIFIER '{' enumerator_list ',' '}' { 
          char *tmp = (char*)malloc(strlen("enum") + strlen($2) + 2);
          sprintf(tmp, "enum %s", $2);
          $$ = tmp;
      }
    | ENUM IDENTIFIER { 
          char *tmp = (char*)malloc(strlen("enum") + strlen($2) + 2);
          sprintf(tmp, "enum %s", $2);
          $$ = tmp;
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
	: CONST { $$ = strdup("CONST"); }
	| VOLATILE { $$ = strdup("VOLATILE"); }
	;



declarator
	: pointer direct_declarator {
          int idx = $2;  // $2 is the token table index from direct_declarator.
           char newType[256];
          sprintf(newType, "%s%s", st.token_table_[idx].token_type, $1);
          
          st.token_table_[idx].token_type += std::string($1);
		  if (strstr(newType, "typedef") != NULL){
            char *temp = new char[st.token_table_[idx].token.size()+1];
            std::strcpy(temp,st.token_table_[idx].token.c_str());
			update_symtab(temp);
		  }
          $$ = idx;
          free($1); /* free the pointer string */
      }
	| direct_declarator { $$ = $1; }
	;

direct_declarator
	: IDENTIFIER{
        st.insert_symbol($1, currentType ? currentType : "INVALID");
        $$ = st.token_table_.size() - 1;
    }
	| '(' declarator ')' { $$ = $2; }
	| direct_declarator '[' ']'{
          int idx = $1;
          char newType[256];
          sprintf(newType, "%s[]",st.token_table_[idx].token_type);
           st.token_table_[idx].token_type += "[]";
         
		  if (strstr(newType, "typedef") != NULL){
            char *temp = new char[st.token_table_[idx].token.size()+1];
            std::strcpy(temp,st.token_table_[idx].token.c_str());
			update_symtab(temp);
			
		  }
          $$ = idx;
      }
	| direct_declarator '[' '*' ']'
	| direct_declarator '[' STATIC type_qualifier_list assignment_expression ']'
	| direct_declarator '[' STATIC assignment_expression ']'
	| direct_declarator '[' type_qualifier_list '*' ']'
	| direct_declarator '[' type_qualifier_list STATIC assignment_expression ']'
	| direct_declarator '[' type_qualifier_list assignment_expression ']'
	| direct_declarator '[' type_qualifier_list ']'
	| direct_declarator '[' assignment_expression ']'{
          int idx = $1;
          char newType[256];
          sprintf(newType, "%s[%s]", st.token_table_[idx].token_type, $3);
          
         st.token_table_[idx].token_type += "[" + std::string($3)+ "]";
		  if (strstr(newType, "typedef") != NULL){
			char *temp = new char[st.token_table_[idx].token.size()+1];
            std::strcpy(temp,st.token_table_[idx].token.c_str());
			update_symtab(temp);
		  }
          $$ = idx;
      }
	| direct_declarator '('  parameter_type_list ')' {
		/* pushing scopes extra if there are arguments inside, eg. fun(int a, int b) */
       int idx = $1;  // $1 is now of type int (the token index)
      
       st.token_table_[idx].token_type = strdup("PROCEDURE");
       $$ = idx;
    }
	| direct_declarator '('   ')'  {
       int idx = $1;
       
       st.token_table_[idx].token_type = strdup("PROCEDURE");
       $$ = idx;
    }
	| direct_declarator '('  identifier_list ')'{
       int idx = $1;
       
       st.token_table_[idx].token_type = strdup("PROCEDURE");
       $$ = idx;
    }
	| direct_declarator '[' assignment_expression error {
          /* Catch an invalid array declaration (missing ']'). 
             Use the default error message and recover. */
          yyerrok;
          $$ = $1;  /* Propagate the index as is */
      }
	;

pointer
	: '*' type_qualifier_list pointer
	| '*' type_qualifier_list
	| '*' pointer{
          /* $2 is a string representing the pointer from the recursive production.
             Prepend a '*' to it. */
          int len = strlen($2) + 2;
          $$ = (char*)malloc(len);
          sprintf($$, "*%s", $2);
          free($2);
      }
	| '*' {
          $$ = strdup("*");
      }
	;

type_qualifier_list
	: type_qualifier
	| type_qualifier_list type_qualifier{ 
          /* For now, ignore qualifiers */
          $$ = $1; 
      }
	;


parameter_type_list
	: parameter_list
	;

parameter_list
	: parameter_declaration
	| parameter_list ',' parameter_declaration
	;

parameter_declaration
	: declaration_specifiers declarator
	| declaration_specifiers abstract_declarator
	| declaration_specifiers
	;

identifier_list
	: IDENTIFIER
	| identifier_list ',' IDENTIFIER
	;

type_name
	: specifier_qualifier_list abstract_declarator
	| specifier_qualifier_list
	;

abstract_declarator
	: pointer direct_abstract_declarator
	| pointer
	| direct_abstract_declarator
	;

direct_abstract_declarator
	: '(' abstract_declarator ')'
	| '[' ']'
	| '[' '*' ']'
	| '[' STATIC type_qualifier_list assignment_expression ']'
	| '[' STATIC assignment_expression ']'
	| '[' type_qualifier_list STATIC assignment_expression ']'
	| '[' type_qualifier_list assignment_expression ']'
	| '[' type_qualifier_list ']'
	| '[' assignment_expression ']'
	| direct_abstract_declarator '[' ']'
	| direct_abstract_declarator '[' '*' ']'
	| direct_abstract_declarator '[' STATIC type_qualifier_list assignment_expression ']'
	| direct_abstract_declarator '[' STATIC assignment_expression ']'
	| direct_abstract_declarator '[' type_qualifier_list assignment_expression ']'
	| direct_abstract_declarator '[' type_qualifier_list STATIC assignment_expression ']'
	| direct_abstract_declarator '[' type_qualifier_list ']'
	| direct_abstract_declarator '[' assignment_expression ']'
	| '(' ')'
	| '(' parameter_type_list ')'
	| direct_abstract_declarator '(' ')'
	| direct_abstract_declarator '(' parameter_type_list ')'
	;

initializer
	: '{' initializer_list '}'
	| '{' initializer_list ',' '}'
	| assignment_expression
	;

initializer_list
	: designation initializer
	| initializer
	| initializer_list ',' designation initializer
	| initializer_list ',' initializer
	;

designation
	: designator_list '='
	;

designator_list
	: designator
	| designator_list designator
	;

designator
	: '[' constant_expression ']'
	| '.' IDENTIFIER
	;

statement
	: labeled_statement
	|  compound_statement 
	| expression_statement
	| selection_statement
	| iteration_statement
	| jump_statement
	;

labeled_statement
	: IDENTIFIER ':' statement
	| CASE constant_expression ':' statement
	| DEFAULT ':' statement
	;

compound_statement
	: '{' PushScope  '}' PopScope
	| '{' PushScope block_item_list '}' PopScope
	| '{' PushScope  block_item_list error {  st.pop_scope();
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
	: GOTO IDENTIFIER ';'
	| CONTINUE ';'
	| BREAK ';'  
	| RETURN ';'  
	| RETURN expression ';'
	;

Global
	: PushScope translation_unit PopScope

translation_unit
	: external_declaration
	| translation_unit external_declaration
	;


external_declaration
	: function_definition
	| declaration
	;

function_definition
	: declaration_specifiers  declarator  declaration_list compound_statement 
	| declaration_specifiers  declarator  compound_statement 
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
	
	st.print_token_table();
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