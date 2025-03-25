%{

#include "symbol.h"
#include "Utility_func.h"
SymbolTable st;


%}

%debug
%union{
	char *id;
    int index;
	typedef struct {
    char* type;
    char* kind;
	int index;
	char* name;
	} attribute_t;
	attribute_t atr;
}

%token <atr> IDENTIFIER 
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
%token <atr> CLASS 
%token PUBLIC PRIVATE PROTECTED

%type <atr> type_specifier declaration_specifiers
%type <atr> direct_declarator declarator 
%type <atr> struct_or_union_specifier struct_or_union
%type <atr> enum_specifier
%type <atr> constant_expression 
%type <atr> pointer type_qualifier_list
%type <atr>   storage_class_specifier type_qualifier 
%type <atr> class_specifier access_specifier base_clause_opt base_specifier_list base_specifier  access_specifier_opt

%type <atr> primary_expression constant 
%type <atr>  unary_expression postfix_expression cast_expression multiplicative_expression additive_expression shift_expression relational_expression equality_expression and_expression exclusive_or_expression
%type <atr>  inclusive_or_expression logical_and_expression logical_or_expression conditional_expression assignment_expression
%type <atr> init_declarator
%type <atr> initializer assignment_operator

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
		
		}
		
		
		
		
	}
	| constant{
		$$.type = $1.type;
		$$.kind = $1.kind;
	}
	| string {
		$$.type = "STRING";
		$$.kind = "IDENTIFIER";
	}
	| '(' expression ')'
	| '(' expression error { yyerrok; }
	;

constant
	: I_CONSTANT{
		$$.type = "INT";
		$$.kind = "CONST";
	}		/* includes character_constant */
	| F_CONSTANT{
		$$.type = "FLOAT";
		$$.kind = "CONST";
	}
	| ENUMERATION_CONSTANT{
		$$.type = "INT";
		$$.kind = "ENUM_CONST";
	}	/* after it has been defined as such */
	;

enumeration_constant		/* before it has been defined as such */
	: IDENTIFIER {
		st.insert_symbol($1.type,"INT" , "enumeration_constant");
	}
	;

string
	: STRING_LIT
	| CHAR_LIT   /* BEEP BEEP , adding char literals here */
	| FUNC_NAME
	;


postfix_expression
	: primary_expression {
		$$.type = $1.type;
		$$.kind = $1.kind;
	}
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
	: postfix_expression {
		$$.type = $1.type;
		$$.kind = $1.kind;
	}
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
	: unary_expression {
		$$.type = $1.type;
		$$.kind = $1.kind;
	}
	| '(' type_name ')' cast_expression
	| '(' type_name error cast_expression { yyerrok; }
	;

multiplicative_expression
	: cast_expression {
		$$.type = $1.type;
		$$.kind = $1.kind;
	}
	| multiplicative_expression '*' cast_expression {
		check_type($1.type, $3.type, "incompatible type expression involved in *: ");
	}
	| multiplicative_expression '/' cast_expression{
		check_type($1.type, $3.type, "incompatible type expression involved in *: ");
	}
	| multiplicative_expression '%' cast_expression{
		check_type($1.type, $3.type, "incompatible type expression involved in %: ");
	}
	;

additive_expression
	: multiplicative_expression {
		$$.type = $1.type;
		$$.kind = $1.kind;
	}
	| additive_expression '+' multiplicative_expression{
		check_type($1.type, $3.type, "incompatible type expression involved in + : ");
	}
	| additive_expression '-' multiplicative_expression{
		check_type($1.type, $3.type,"incompatible type expression involved in - : ");
	}
	;

shift_expression
	: additive_expression {
		$$.type = $1.type;
		$$.kind = $1.kind;
	}
	| shift_expression LEFT_OP additive_expression{
		//NEWWWW
		
		if(eq($1.type, "INT") == false || eq($3.type, "INT")==false){
			char *err = "both operands must be int, int but are ";
        	err = concat(err,$1.type);
        	err = concat(err,  $3.type);
        yyerror(err);
		}
		
	}
	| shift_expression RIGHT_OP additive_expression{
		//NEWWW
		if(eq($1.type, "INT") == false || eq($3.type, "INT")==false){
			char *err = "both operands must be int, int but are ";
        	err = concat(err,$1.type);
        	err = concat(err, $3.type );
        yyerror(err);
		}
	}
	;

relational_expression
	: shift_expression {
		$$.type = $1.type;
		$$.kind = $1.kind;
	}
	| relational_expression '<' shift_expression{
		check_type($1.type, $3.type,"incompatible type expression involved in < : ");
	}
	| relational_expression '>' shift_expression{
		check_type($1.type, $3.type,"incompatible type expression involved in >: ");
	}
	| relational_expression LE_OP shift_expression{
		check_type($1.type, $3.type,"incompatible type expression involved in <=: ");
	}
	| relational_expression GE_OP shift_expression{
		check_type($1.type, $3.type,"incompatible type expression involved in >=: ");
	}
	;

equality_expression
	: relational_expression {
		$$.type = $1.type;
		$$.kind = $1.kind;
	}
	| equality_expression EQ_OP relational_expression{
		check_type($1.type, $3.type, "incompatible type expression involved in = : ");
	}
	| equality_expression NE_OP relational_expression{
		check_type($1.type, $3.type, "incompatible type expression involved in != : ");
	}
	;

and_expression
	: equality_expression {
		$$.type = $1.type;
		$$.kind = $1.kind;
	}
	| and_expression '&' equality_expression {
		if(eq($1.type, "INT") == false || eq($3.type, "INT")==false){
			char *err = "both operands in & must be int, int but are ";
        	err = concat(err,$1.type);
        	err = concat(err, $3.type );
        yyerror(err);
		}
	}
	;

exclusive_or_expression
	: and_expression {
		$$.type = $1.type;
		$$.kind = $1.kind;
	}
	| exclusive_or_expression '^' and_expression{
		if(eq($1.type, "INT") == false || eq($3.type, "INT")==false){
			char *err = "both operands in ^ must be int, int but are ";
        	err = concat(err,$1.type);
        	err = concat(err, $3.type );
        yyerror(err);
		}
	}
	;

inclusive_or_expression
	: exclusive_or_expression {
		$$.type = $1.type;
		$$.kind = $1.kind;
	}
	| inclusive_or_expression '|' exclusive_or_expression{
		if(eq($1.type, "INT") == false || eq($3.type, "INT")==false){
			char *err = "both operands in | must be int, int but are ";
        	err = concat(err,$1.type);
        	err = concat(err, $3.type );
        yyerror(err);
		}
	}
	;

logical_and_expression
	: inclusive_or_expression {
		$$.type = $1.type;
		$$.kind = $1.kind;
	}
	| logical_and_expression AND_OP inclusive_or_expression{
		check_type($1.type, $3.type,"incompatible type expression involved in &&: ");
	}
	;

logical_or_expression
	: logical_and_expression {
		$$.type = $1.type;
		$$.kind = $1.kind;
	}
	| logical_or_expression OR_OP logical_and_expression{
		check_type($1.type, $3.type, "incompatible type expression involved in || = : ");
	}
	;

conditional_expression
	: logical_or_expression {
		$$.type = $1.type;
		$$.kind = $1.kind;
	}
	| logical_or_expression '?' expression ':' conditional_expression
	;

assignment_expression
	: conditional_expression{
		$$.type = $1.type;
		$$.kind = $1.kind;
	}
	| unary_expression assignment_operator assignment_expression {
		if(eq($2.type,"&=")||eq($2.type,"^=")||eq($2.type,"|=") || eq($2.type,"<<=") || eq($2.type,">>=")){
			if(eq($1.type, "INT") == false || eq($3.type, "INT")==false){
			char *err = "both operands must be int, int in ";
			err = concat(err, $2.type);
			err = concat(err, "but are: ");
        	err = concat(err,$1.type);
        	err = concat(err, $3.type );
       	 yyerror(err);
			}
		}
		else{
			char* s1 = "incompatible type expression involved in";
			s1 = concat(s1,$2.type);
			s1 = concat(s1," : ");
			check_type($1.type, $3.type,s1);
		}
	}
	;

assignment_operator
	: '='
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
			char *combined = (char*)malloc(strlen($1.type) + strlen($2.type) + 2);
              sprintf(combined, "%s %s", $1.type, $2.type);
              $$.type = combined;
              currentType = $$.type;
              free($1.type);
              free($2.type);
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
		}
	| type_qualifier
	      { $$.type = $1.type; 
		  currentType = $$.type;
		  }
	| declaration_specifiers '&'
	;


init_declarator_list
	: init_declarator 
	| init_declarator_list ',' init_declarator
	;

init_declarator
	: declarator '=' initializer {
		if(eq($1.type , $3.type) == false){

			char *err = "incompatible type declaration: ";
			err = concat(err,$1.type);
			err = concat(err, $3.type);
			yyerror(err);
		}
		else{
			if($1.type == "PROCEDURE"){
				std::string err = "cannot declare procedure with = ";
			yyerror(err.c_str());
			}
		}	
	}
	| declarator 
	;

storage_class_specifier
	: TYPEDEF	{ $$.type = strdup("typedef"); }/* identifiers must be flagged as TYPEDEF_NAME */
	| EXTERN { $$.type = strdup("extern"); }
	| STATIC { $$.type = strdup("static"); }
	| AUTO { $$.type = strdup("auto"); }
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
	| TYPEDEF_NAME	{ currentType = strdup(yytext); $$.type = currentType;}
	;

class_specifier
    : CLASS '{' PushScope class_member_list '}' PopScope
         {
		 $$.type = (char*)malloc(strlen("class") + 14); 
         sprintf($$.type, "class (anonymous)");
		   }
	| CLASS IDENTIFIER base_clause_opt  '{' PushScope class_member_list '}' PopScope
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
    : access_specifier ':' '{' translation_unit '}'
    ;


access_specifier
    : PUBLIC { $$.type = strdup("public"); }
    | PRIVATE { $$.type = strdup("private"); }
    | PROTECTED { $$.type = strdup("protected"); }
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
	: struct_or_union '{' PushScope struct_declaration_list '}' PopScope {
         /* Anonymous struct or union */
         $$.type = (char*)malloc(strlen($1.type) + 14); // Enough for " (anonymous)" and '\0'
         sprintf($$.type, "%s (anonymous)", $1.type);
    }
	| struct_or_union IDENTIFIER '{' PushScope struct_declaration_list '}' PopScope {
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
          $$.type = strdup("enum");
      }
    | ENUM '{' enumerator_list ',' '}' { 
          $$.type = strdup("enum");
      }
    | ENUM IDENTIFIER '{' enumerator_list '}' { 
          /* Named enum: produce "enum <identifier>" */
          char *tmp = (char*)malloc(strlen("enum") + strlen($2.type) + 2);
          sprintf(tmp, "enum %s", $2.type);
          $$.type = tmp;
      }
    | ENUM IDENTIFIER '{' enumerator_list ',' '}' { 
          char *tmp = (char*)malloc(strlen("enum") + strlen($2.type) + 2);
          sprintf(tmp, "enum %s", $2.type);
          $$.type = tmp;
      }
    | ENUM IDENTIFIER { 
          char *tmp = (char*)malloc(strlen("enum") + strlen($2.type) + 2);
          sprintf(tmp, "enum %s", $2.type);
          $$.type = tmp;
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
	: pointer direct_declarator {
          int idx = $2.index;  // $2 is the token table index from direct_declarator.
           char newType[256];
          sprintf(newType, "%s%s", st.token_table_[idx].token_type, $1.type);
          st.token_table_[idx].token_type += std::string($1.type);
		  //$2.name is name of id
		  $$.name = $2.name;
		  st.lookup(std::string($2.name))->type = st.token_table_[idx].token_type;
		  $$.type = strdup(st.token_table_[idx].token_type.c_str());
		  if (strstr(newType, "typedef") != NULL){
            char *temp = new char[st.token_table_[idx].token.size()+1];
            std::strcpy(temp,st.token_table_[idx].token.c_str());
			update_symtab(temp);
		  }
          $$.index = idx;
		  $$.kind = $2.kind;
          free($1.type); /* free the pointer string */
		  
      }
	| direct_declarator {
		 $$.index = $1.index;
		 $$.type = $1.type;
		 $$.kind = $1.kind;
		  }
	;

direct_declarator
	: IDENTIFIER{
        st.insert_symbol($1.type, currentType ? currentType : "INVALID", "IDENTIFIER");
        $$.index = st.token_table_.size() - 1;
		$$.type = currentType;
		$$.kind = "IDENTIFIER";
		$$.name = $1.type;
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
	| direct_declarator '[' '*' ']'
	| direct_declarator '[' STATIC type_qualifier_list assignment_expression ']'
	| direct_declarator '[' STATIC assignment_expression ']'
	| direct_declarator '[' type_qualifier_list '*' ']'
	| direct_declarator '[' type_qualifier_list STATIC assignment_expression ']'
	| direct_declarator '[' type_qualifier_list assignment_expression ']'
	| direct_declarator '[' type_qualifier_list ']'
	| direct_declarator '[' assignment_expression ']'{
			if($3.type == "INT" && $3.kind == "CONST"){
				 int idx = $1.index;
          char newType[256];
          sprintf(newType, "%s*", st.token_table_[idx].token_type, $3.type);
		  $$.name = $1.name;
          
         st.token_table_[idx].token_type += "*";
		 st.lookup(std::string($1.name))->type = st.token_table_[idx].token_type;
		 $$.type = strdup(st.token_table_[idx].token_type.c_str());
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
      
       st.token_table_[idx].token_type = strdup(currentType);
	   st.token_table_[idx].kind = strdup("PROCEDURE");
	    st.lookup(std::string($1.name))->type = st.token_table_[idx].token_type;
		 st.lookup(std::string($1.name))->kind = st.token_table_[idx].kind;
	   $$.type = strdup(currentType);
	   $$.kind = strdup("PROCEDURE");
       $$.index = idx;
	   $$.name = $1.name;
    }
	| direct_declarator '('   ')'  {
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
	| direct_declarator '('  identifier_list ')'{
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

pointer
	: '*' type_qualifier_list pointer
	| '*' type_qualifier_list
	| '*' pointer{
          /* $2 is a string representing the pointer from the recursive production.
             Prepend a '*' to it. */
          int len = strlen($2.type) + 2;
          $$.type = (char*)malloc(len);
          sprintf($$.type, "*%s", $2.type);
          free($2.type);
      }
	| '*' {
          $$.type = strdup("*");
      }
	;

type_qualifier_list
	: type_qualifier
	| type_qualifier_list type_qualifier{ 
          /* For now, ignore qualifiers */
          $$.type = $1.type; 
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
	| assignment_expression {
		$$.type = $1.type;
		$$.kind = $1.kind;
	}
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