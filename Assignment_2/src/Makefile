# Variables
LEXER           = lexer.l
LEXER_OUT       = lex.yy.c
GRAMMAR         = grammar.y
GRAMMAR_OUT_C   = grammar.tab.c
GRAMMAR_OUT_H   = grammar.tab.h
GRAMMAR_OUT_OUT = grammar.output
EXECUTABLE      = parser
CXX             = gcc
CXXFLAGS        = -w
LINK_LIBS       = -ll
# Default target
all: $(EXECUTABLE)

# Build parser
$(EXECUTABLE): $(GRAMMAR_OUT_C) $(LEXER_OUT)
	@echo "Compiling $(EXECUTABLE)..."
	$(CXX) $(CXXFLAGS) -o $(EXECUTABLE) $(GRAMMAR_OUT_C) $(LEXER_OUT)  $(LINK_LIBS)

# Generate grammar.tab.c/h and grammar.output with tracing
$(GRAMMAR_OUT_C): $(GRAMMAR)
	@echo "Running bison with debug and output..."
	bison -d -v -t $(GRAMMAR)


# Generate lex.yy.c
$(LEXER_OUT): $(LEXER)
	@echo "Running flex..."
	flex $(LEXER)

# Run the parser on a test file
run: $(EXECUTABLE)
	@echo "Running parser on test7.c..."
	./$(EXECUTABLE) < test7.c

# Clean up all generated files
clean:
	@echo "Cleaning up..."
	rm -f $(EXECUTABLE) $(GRAMMAR_OUT_C) $(GRAMMAR_OUT_H) $(GRAMMAR_OUT_OUT) $(LEXER_OUT)

.PHONY: all clean run
