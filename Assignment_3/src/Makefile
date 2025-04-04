all: parser

parser: grammar.tab.c lex.yy.c
	g++ -w -o parser grammar.tab.c lex.yy.c -ll

grammar.tab.c: grammar.y
	bison -d -v -t grammar.y

lex.yy.c: lexer.l
	flex lexer.l

run: parser
	./parser < test7.c

clean:
	rm -f parser grammar.tab.c grammar.tab.h grammar.output lex.yy.c
