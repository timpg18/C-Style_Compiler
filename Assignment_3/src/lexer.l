%option noyywrap
%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "grammar.tab.h"

extern void yyerror(const char *);
extern int sym_type(const char *);
static int check_type(void);
//#define sym_type(identifier) IDENTIFIER /* with no symbol table, fake it */

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

void update_symtab(char* ttoken){
    for(int i=0;i<symcount;i++){
        if(strcmp(Symboltable[i].lexeme,ttoken) == 0){
            strcpy(Symboltable[i].token, "typedef");
            break;
        }
    }
}



%}




/*regex for literals*/

INT_CONST [0-9]+
OCTAL_CONST 0[0-7]+ 
HEX_CONST 0[xX][0-9a-fA-F]+
FLOAT_CONST ([0-9]*\.[0-9]+([eE][+-]?[0-9]+)?)|([0-9]+([eE][+-]?[0-9]+)?)
LINE_COMMENT "//".*
MULTILINE_COMMENT "/*"([^*]|\*+[^*/])*\*+"/"
CHAR_CONST  \'([^\\'\n]|\\[abfnrtv0\'\"\\])\' 
STRING_CONST \"([^"\\\n]|\\.)*\"
UNTERMINATED_MULTILINE_COMMENT "/*"([^*]|\*+[^*/])*(\n|$)    


%%

"int"       { return INT; }
"double"    { return DOUBLE; }
"char"      { return CHAR; }
"float"     { return FLOAT; }
"long"      { return LONG; }
"short"     { return SHORT; }
"bool"      { return BOOL; }
"signed"    { return SIGNED; }
"sizeof"    { return SIZEOF;}
"unsigned"  { return UNSIGNED; }
"const"     { return CONST; }
"volatile"  { return VOLATILE; }
"void"      { return VOID; }

"typedef"   { return TYPEDEF; }
"extern"    { return EXTERN; }
"static"    { return STATIC; }
"register"  { return REGISTER; }
"auto"      { return AUTO; }

"struct"    { return STRUCT; }
"enum"      { return ENUM; }
"union"     { return UNION; }

"if"        { return IF; }
"else"      { return ELSE; }
"switch"    { return SWITCH; }
"for"       { return FOR; }
"while"     { return WHILE; }
"until"     { return UNTIL;}
"do"        { return DO; }
"goto"      { return GOTO; }
"break"     { return BREAK; }
"continue"  { return CONTINUE; }
"default"   { return DEFAULT; }
"case"      { return CASE; }
"return"    { return RETURN; }


"class"     { return CLASS; }
"public"    { return PUBLIC; }
"private"   { return PRIVATE; }
"protected" { return PROTECTED; }
"true" {return TRUE;}
"false" {return FALSE;}
"INT_(MAX|MIN)" { return I_CONSTANT; }
"NULL"      { return I_CONSTANT; }
{OCTAL_CONST} { return I_CONSTANT; }
{INT_CONST}   {  yylval.id = strdup(yytext); return I_CONSTANT; }
{FLOAT_CONST} { return F_CONSTANT; }
{HEX_CONST}   { return I_CONSTANT; }
{LINE_COMMENT} {} /* Ignore comments */
{CHAR_CONST}  { return CHAR_LIT; }
{STRING_CONST} { return STRING_LIT; }
{MULTILINE_COMMENT} {
    for (int i = 0; yytext[i] != '\0'; i++) {
        if (yytext[i] == '\n') {
            line_no++;  // Increment line number for each newline in the multi-line comment
        }
    }
}

{UNTERMINATED_MULTILINE_COMMENT} {
    for (int i = 0; yytext[i] != '\0'; i++) {
        if (yytext[i] == '\n') {
            line_no++;
        }
    }
    add_error("Error: Unterminated Multiline Comment", "/* ...", line_no);
}

[a-zA-Z_][a-zA-Z0-9_]* {
    yylval.id = strdup(yytext);
    // Check if the identifier already exists in the symbol table.
    for (int i = 0; i < symcount; i++) {
        if (strcmp(Symboltable[i].lexeme, yytext) == 0) {
            if(strcmp(Symboltable[i].token,"typedef")==0){
               printf("%s    %s\n",Symboltable[i].lexeme,Symboltable[i].token);
               return TYPEDEF_NAME;
        }
        }
    }
    addtosymtab(yytext, "identifier", line_no);
    return IDENTIFIER;
}
"..."			{ return(ELLIPSIS); }
";"          { return(';'); }
"{"          { return('{'); }
"}"          { return('}'); }
","          { return(','); }
":"          { return(':'); }
"="          { return('='); }
"("          { return('('); }
")"          { return(')'); }
"["          { return('['); }
"]"          { return(']'); }
"."          { return('.'); }
"&"          { return('&'); }
"!"          { return('!'); }
"~"          { return('~'); }
"-"          { return('-'); }
"+"          { return('+'); }
"*"          { return('*'); }
"/"          { return('/'); }
"%"          { return('%'); }
"<"          { return('<'); }
">"          { return('>'); }
"^"          { return('^'); }
"|"          { return('|'); }
"?"          { return('?'); }
">>="        { return(RIGHT_ASSIGN); }
"<<="        { return(LEFT_ASSIGN); }
"-="         { return(SUB_ASSIGN); }
"*="         { return(MUL_ASSIGN); }
"/="         { return(DIV_ASSIGN); }
"&="         { return(AND_ASSIGN); }
"^="         { return XOR_ASSIGN; }
"|="         { return OR_ASSIGN; }
">>"         { return(RIGHT_OP); }
"<<"         { return(LEFT_OP); }
"->"         { return(PTR_OP); }
"&&"         { return(AND_OP); }
"||"         { return(OR_OP); }
"<="         { return(LE_OP); }
">="         { return(GE_OP); }
"=="         { return(EQ_OP); }
"!="         { return(NE_OP); }
"++"         { return INC_OP; }
"--"         { return DEC_OP; }
"%="         { return MOD_ASSIGN; }
"+="         { return ADD_ASSIGN; }




[ \t\r]+    { /* Ignore whitespaces */ }
\n {line_no++;}
'(.{2,})' { 
    add_error("Error: Invalid char constant", yytext, line_no); /* is >= 2 char => invalid... ig single char always valid?*/
}
"\"" { add_error("Error: Missing Terminal \" ", yytext, line_no); }
"'" { add_error("Error: Missing Terminal \' ", yytext, line_no); }
. {
    add_error("Error: Unrecognized token",yytext,line_no); 
}

%%