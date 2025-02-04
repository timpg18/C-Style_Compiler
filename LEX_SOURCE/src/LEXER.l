%option noyywrap
%{

/*
current bugs: sole ' . " gives no error 
            : single and multiline comments not recognized
            : typedef no implemented
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_SYMBOLS 20000
#define MAX_ERRORS 1000
int line_no = 1;

typedef struct{
    char* lexeme;
    char* token;
    int line_num;
}SYMTAB;

SYMTAB Symboltable[MAX_SYMBOLS];
int symcount = 0;

typedef struct{
    char* message;
    char* token;
    int line_num;
}Error;

Error error_list[MAX_ERRORS];
int error_count = 0;

/* Function to add an error to the error list */
void add_error(const char* msg, const char* token, int line_no) {
    if (error_count < MAX_ERRORS) {
        error_list[error_count].message = strdup(msg);
        error_list[error_count].token = strdup(token);
        error_list[error_count].line_num = line_no;
        error_count++;
    } else {
        printf("Error list is full. Cannot add more errors.\n");
    }
}

/* Function to print all errors in a structured format */
void print_errors() {
    printf("\n==================================================================================================================\n");
    printf("                                              ERROR REPORT\n");
    printf("==================================================================================================================\n");
    printf("| %-5s | %-40s | %-40s | %-15s |\n", "ID", "Description", "Token", "Line Number");
    printf("------------------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < error_count; i++) {
        printf("| %-5d | %-40s | %-40s | %-15d |\n", 
               i + 1, 
               error_list[i].message, 
               error_list[i].token, 
               error_list[i].line_num);
    }

    printf("==================================================================================================================\n");
}

void addtosymtab(const char* lex, const char* tok, int line_no){
    if(symcount >= MAX_SYMBOLS){
        add_error("full symbol table", lex, line_no);
    return;
    }
    Symboltable[symcount].lexeme = strdup(lex);
    Symboltable[symcount].token = strdup(tok);
    Symboltable[symcount].line_num = line_no;

    if(strcmp(Symboltable[symcount].lexeme,lex) != 0 || strcmp(Symboltable[symcount].token,tok) != 0){
        add_error("allocation failure", lex, line_no);
        return;
    }
    symcount++;
}

void print_symtab() {
    printf("\n=========================================================================================================\n");
    printf("|                                            SYMBOL TABLE                                               |\n");
    printf("=========================================================================================================\n");
    printf("| %-40s | %-40s | %-15s |\n", "LEXEME", "TOKEN", "LINE");
    printf("---------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < symcount; i++) {
        printf("| %-40s | %-40s | %-15d |\n", 
               Symboltable[i].lexeme, 
               Symboltable[i].token, 
               Symboltable[i].line_num);
    }

    printf("=========================================================================================================\n");
}



%}



/* Define keyword tokens */
INT      "int"
BOOL "bool"
DOUBLE   "double"
CHAR     "char"
FLOAT    "float"
LONG     "long"
SHORT    "short"
SIGNED   "signed"
UNSIGNED "unsigned"
CONST    "const"
STATIC   "static"
VOLATILE "volatile"
EXTERN   "extern"
REGISTER "register"
AUTO     "auto"
IF       "if"
ELSE     "else"
SWITCH   "switch"
FOR      "for"
WHILE    "while"
DO       "do"
GOTO     "goto"
BREAK    "break"
CONTINUE "continue"
STRUCT   "struct"
ENUM     "enum"
UNION    "union"
VOID     "void"
DEFAULT  "default"
CASE     "case"
RETURN   "return"
CLASS    "class"
PUBLIC   "public"
PRIVATE  "private"
PROTECTED "protected"

/*regex for literals*/
IDENTIFIER [a-zA-Z_][a-zA-Z0-9_]*
INT_CONST [+-]?[0-9]+
OCTAL_CONST [+-]?0[0-7]+ 
HEX_CONST 0[xX][0-9a-fA-F]+
NULL "NULL"
FLOAT_CONST ([+-]?[0-9]*\.[0-9]+([eE][+-]?[0-9]+)?)|([+-]?[0-9]+([eE][+-]?[0-9]+)?)
CHAR_CONST '(\\.|[^\\'])'
STRING_CONST \"([^\\"\\]|\\.)*\"


/* Define punctuation macros */
LEFT_SQ_BRACKET "["
RIGHT_SQ_BRACKET "]"
LEFT_BRACKET "("
RIGHT_BRACKET ")"
LEFT_CURLY_BRACKET "{"
RIGHT_CURLY_BRACKET "}"
DOT "."
ARROW "->"
INCREMENT "++"
DECREMENT "--"
BITWISE_AND "&"
STAR "*"
PLUS "+"
MINUS "-"
BITWISE_NOT "~"
NOT "!"
SLASH "/"
MOD "%"
LSHIFT "<<"
RSHIFT ">>"
LESS_THAN_OR_EQUAL "<="
GREATER_THAN_OR_EQUAL ">="
LESS_THAN "<"
GREATER_THAN ">"
EQUALS "=="
NOT_EQUAL "!="
BITWISE_XOR "^"
BITWISE_OR "|"
AND "&&"
OR "||"
TERNARY "?"
SEMICOLON ";"
ASSIGN "="
MULT_ASSIGN "*="
SLASH_ASSIGN "/="
MOD_ASSIGN "%="
PLUS_ASSIGN "+="
MINUS_ASSIGN "-="
LSHIFT_ASSIGN "<<="
RSHIFT_ASSIGN ">>="
AND_ASSIGN "&="
XOR_ASSIGN "^="
OR_ASSIGN "|="
COMMA ","
PREPROCESSOR_TOKEN "#"
DOUBLE_HASH "##"



%%
{INT} { addtosymtab(yytext, "Keyword", line_no); }
{DOUBLE} { addtosymtab(yytext, "Keyword", line_no); }
{CHAR} { addtosymtab(yytext, "Keyword", line_no); }
{FLOAT} { addtosymtab(yytext, "Keyword", line_no); }
{LONG} { addtosymtab(yytext, "Keyword", line_no); }
{SHORT} { addtosymtab(yytext, "Keyword", line_no); }
{BOOL} { addtosymtab(yytext, "Keyword", line_no); }

{SIGNED} { addtosymtab(yytext, "Keyword", line_no); }
{UNSIGNED} { addtosymtab(yytext, "Keyword", line_no); }
{CONST} { addtosymtab(yytext, "Keyword", line_no); }
{STATIC} { addtosymtab(yytext, "Keyword", line_no); }
{VOLATILE} { addtosymtab(yytext, "Keyword", line_no); }
{EXTERN} { addtosymtab(yytext, "Keyword", line_no); }
{REGISTER} { addtosymtab(yytext, "Keyword", line_no); }
{AUTO} { addtosymtab(yytext, "Keyword", line_no); }

{IF} { addtosymtab(yytext, "Keyword", line_no); }
{ELSE} { addtosymtab(yytext, "Keyword", line_no); }
{SWITCH} { addtosymtab(yytext, "Keyword", line_no); }

{FOR} { addtosymtab(yytext, "Keyword", line_no); }
{WHILE} { addtosymtab(yytext, "Keyword", line_no); }
{DO} { addtosymtab(yytext, "Keyword", line_no); }

{GOTO} { addtosymtab(yytext, "Keyword", line_no); }
{BREAK} { addtosymtab(yytext, "Keyword", line_no); }
{CONTINUE} { addtosymtab(yytext, "Keyword", line_no); }

{STRUCT} { addtosymtab(yytext, "Keyword", line_no); }
{ENUM} { addtosymtab(yytext, "Keyword", line_no); }
{UNION} { addtosymtab(yytext, "Keyword", line_no); }

{VOID} { addtosymtab(yytext, "Keyword", line_no); }
{DEFAULT} { addtosymtab(yytext, "Keyword", line_no); }

{CASE} { addtosymtab(yytext, "Keyword", line_no); }

{RETURN} { addtosymtab("RETURN", "Keyword", line_no); }  // Explicitly display RETURN instead of yytext

{CLASS} { addtosymtab(yytext, "Keyword", line_no); }
{PUBLIC} { addtosymtab(yytext, "Keyword", line_no); }
{PRIVATE} { addtosymtab(yytext, "Keyword", line_no); }
{PROTECTED} { addtosymtab(yytext, "Keyword", line_no); }

"INT_(MAX|MIN)" { addtosymtab(yytext, "CONST_INT", line_no); }
{NULL} { addtosymtab(yytext, "CONST_NULL", line_no); }
{OCTAL_CONST} { addtosymtab(yytext, "OCTAL_CONST", line_no); } /* put octal rule above int to prioritise leading 0's*/
{INT_CONST} { addtosymtab(yytext, "INTEGER", line_no); }
{FLOAT_CONST} { addtosymtab(yytext, "FLOAT", line_no); } /*also matches 1e3 as float which is intended (without decimal) */
{HEX_CONST} { addtosymtab(yytext, "HEX_CONST", line_no); }
{CHAR_CONST} { addtosymtab(yytext, "CHARACTER CONSTANT", line_no); }
{STRING_CONST} { addtosymtab(yytext, "STRING LITERAL", line_no); }



{IDENTIFIER} {addtosymtab(yytext, "ID", line_no);}

{LEFT_SQ_BRACKET} { addtosymtab(yytext, "PUNC_LEFT_SQ_BRACKET", line_no); }
{RIGHT_SQ_BRACKET} { addtosymtab(yytext, "PUNC_RIGHT_SQ_BRACKET", line_no); }
{LEFT_BRACKET} { addtosymtab(yytext, "PUNC_LEFT_BRACKET", line_no); }
{RIGHT_BRACKET} { addtosymtab(yytext, "PUNC_RIGHT_BRACKET", line_no); }
{LEFT_CURLY_BRACKET} { addtosymtab(yytext, "PUNC_LEFT_CURLY_BRACKET", line_no); }
{RIGHT_CURLY_BRACKET} { addtosymtab(yytext, "PUNC_RIGHT_CURLY_BRACKET", line_no); }
{DOT} { addtosymtab(yytext, "PUNC_DOT", line_no); }
{ARROW} { addtosymtab(yytext, "PUNC_ARROW", line_no); }
{INCREMENT} { addtosymtab(yytext, "PUNC_INCREMENT", line_no); }
{DECREMENT} { addtosymtab(yytext, "PUNC_DECREMENT", line_no); }
{BITWISE_AND} { addtosymtab(yytext, "PUNC_BITWISE_AND", line_no); }
{STAR} { addtosymtab(yytext, "PUNC_STAR", line_no); }
{PLUS} { addtosymtab(yytext, "PUNC_PLUS", line_no); }
{MINUS} { addtosymtab(yytext, "PUNC_MINUS", line_no); }
{BITWISE_NOT} { addtosymtab(yytext, "PUNC_BITWISE_NOT", line_no); }
{NOT} { addtosymtab(yytext, "PUNC_NOT", line_no); }
{SLASH} { addtosymtab(yytext, "PUNC_SLASH", line_no); }
{MOD} { addtosymtab(yytext, "PUNC_MOD", line_no); }
{LSHIFT} { addtosymtab(yytext, "PUNC_LSHIFT", line_no); }
{RSHIFT} { addtosymtab(yytext, "PUNC_RSHIFT", line_no); }
{LESS_THAN_OR_EQUAL} { addtosymtab(yytext, "PUNC_LESS_THAN_OR_EQUAL", line_no); }
{GREATER_THAN_OR_EQUAL} { addtosymtab(yytext, "PUNC_GREATER_THAN_OR_EQUAL", line_no); }
{LESS_THAN} { addtosymtab(yytext, "PUNC_LESS_THAN", line_no); }
{GREATER_THAN} { addtosymtab(yytext, "PUNC_GREATER_THAN", line_no); }
{EQUALS} { addtosymtab(yytext, "PUNC_EQUALS", line_no); }
{NOT_EQUAL} { addtosymtab(yytext, "PUNC_NOT_EQUAL", line_no); }
{BITWISE_XOR} { addtosymtab(yytext, "PUNC_BITWISE_XOR", line_no); }
{BITWISE_OR} { addtosymtab(yytext, "PUNC_BITWISE_OR", line_no); }
{AND} { addtosymtab(yytext, "PUNC_AND", line_no); }
{OR} { addtosymtab(yytext, "PUNC_OR", line_no); }
{TERNARY} { addtosymtab(yytext, "PUNC_TERNARY", line_no); }
{SEMICOLON} { addtosymtab(yytext, "PUNC_SEMICOLON", line_no); }
{ASSIGN} { addtosymtab(yytext, "PUNC_ASSIGN", line_no); }
{MULT_ASSIGN} { addtosymtab(yytext, "PUNC_MULT_ASSIGN", line_no); }
{SLASH_ASSIGN} { addtosymtab(yytext, "PUNC_SLASH_ASSIGN", line_no); }
{MOD_ASSIGN} { addtosymtab(yytext, "PUNC_MOD_ASSIGN", line_no); }
{PLUS_ASSIGN} { addtosymtab(yytext, "PUNC_PLUS_ASSIGN", line_no); }
{MINUS_ASSIGN} { addtosymtab(yytext, "PUNC_MINUS_ASSIGN", line_no); }
{LSHIFT_ASSIGN} { addtosymtab(yytext, "PUNC_LSHIFT_ASSIGN", line_no); }
{RSHIFT_ASSIGN} { addtosymtab(yytext, "PUNC_RSHIFT_ASSIGN", line_no); }
{AND_ASSIGN} { addtosymtab(yytext, "PUNC_AND_ASSIGN", line_no); }
{XOR_ASSIGN} { addtosymtab(yytext, "PUNC_XOR_ASSIGN", line_no); }
{OR_ASSIGN} { addtosymtab(yytext, "PUNC_OR_ASSIGN", line_no); }
{COMMA} { addtosymtab(yytext, "PUNC_COMMA", line_no); }
{PREPROCESSOR_TOKEN} { addtosymtab(yytext, "PUNC_PREPROCESSOR_TOKEN", line_no); }
{DOUBLE_HASH} { addtosymtab(yytext, "PUNC_DOUBLE_HASH", line_no); }

[ \t\r]+    { /* Ignore whitespaces */ }
\n {line_no++;}
. {
    add_error("Error: Unrecognized token",yytext,line_no); 
}
%%
main(int argc, char **argv)
{
yylex();
if(error_count == 0)print_symtab();
else print_errors();
return 0;
}
 