%{
#include <iostream>
#include <string>
using namespace std;

extern "C" int yylex();
void yyerror(const char *s);
%}

%%

[a-zA-Z_][a-zA-Z0-9_]*  { cout << "Identifier: " << yytext << endl; }
[0-9]+                  { cout << "Number: " << yytext << endl; }

"+"     { cout << "Operator: " << yytext << endl; }
"-"     { cout << "Operator: " << yytext << endl; }
"*"     { cout << "Operator: " << yytext << endl; }

";"     { cout << "Delimiter: " << yytext << endl; }
"{"     { cout << "Delimiter: " << yytext << endl; }
"}"     { cout << "Delimiter: " << yytext << endl; }
"("     { cout << "Delimiter: " << yytext << endl; }
")"     { cout << "Delimiter: " << yytext << endl; }

[ \t\n\r]+  { /* Skip whitespace */ }

.         { cout << "Unknown: " << yytext << endl; }

%%

int main() {
    cout << "Enter C code (Ctrl+D to end):" << endl;
    yylex();  // Start lexical analysis
    return 0;
}

void yyerror(const char *s) {
    cerr << "Error: " << s << endl;
}
