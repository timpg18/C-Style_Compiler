# Variables
LEXER          = src/lexer.l
LEXER_OUT      = src/lex.yy.c
GRAMMAR        = src/grammar.y
GRAMMAR_OUT_C  = src/grammar.tab.c
GRAMMAR_OUT_H  = src/grammar.tab.h
EXECUTABLE     = parser
CC             = gcc

# Default target: build the executable
all: $(EXECUTABLE)

# Generate parser files from grammar.y using bison
$(GRAMMAR_OUT_C) $(GRAMMAR_OUT_H): $(GRAMMAR)
	@echo "Running bison on $(GRAMMAR)..."
	bison -d -o $(GRAMMAR_OUT_C) $(GRAMMAR)

# Generate the lexer C source from lexer.l using flex
$(LEXER_OUT): $(LEXER)
	@echo "Running flex on $(LEXER)..."
	flex -o $(LEXER_OUT) $(LEXER)

# Compile the generated parser (grammar.tab.c) and lexer (lex.yy.c) into an executable
$(EXECUTABLE): $(GRAMMAR_OUT_C) $(LEXER_OUT)
	@echo "Compiling $(GRAMMAR_OUT_C) and $(LEXER_OUT) into executable $(EXECUTABLE)..."
	$(CC) $(GRAMMAR_OUT_C) $(LEXER_OUT) -o $(EXECUTABLE) -lfl

# Clean up generated files
clean:
	@echo "Cleaning up..."
	rm -f $(LEXER_OUT) $(GRAMMAR_OUT_C) $(GRAMMAR_OUT_H) $(EXECUTABLE)

.PHONY: all clean
