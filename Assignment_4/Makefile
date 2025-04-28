SRC_DIR := src
BIN_DIR := .

all: $(BIN_DIR)/parser

$(BIN_DIR)/parser: $(SRC_DIR)/grammar.tab.c $(SRC_DIR)/lex.yy.c
	g++ -w -o $(BIN_DIR)/parser $(SRC_DIR)/grammar.tab.c $(SRC_DIR)/lex.yy.c $(SRC_DIR)/types.cpp $(SRC_DIR)/dependencyAnalyzer.cpp $(SRC_DIR)/IR_utility.cpp $(SRC_DIR)/symbol.cpp  $(SRC_DIR)/AddressAllocationTable.cpp $(SRC_DIR)/BasicBlockConstructor.cpp $(SRC_DIR)/CodeGenerator.cpp $(SRC_DIR)/RegisterDescriptor.cpp -ll

$(SRC_DIR)/grammar.tab.c $(SRC_DIR)/grammar.tab.h: $(SRC_DIR)/grammar.y
	bison -d -v -t -o $(SRC_DIR)/grammar.tab.c $(SRC_DIR)/grammar.y

$(SRC_DIR)/lex.yy.c: $(SRC_DIR)/lexer.l
	flex -o $(SRC_DIR)/lex.yy.c $(SRC_DIR)/lexer.l

run: $(BIN_DIR)/parser

clean:
	rm -f $(BIN_DIR)/parser \
	      $(SRC_DIR)/grammar.tab.c \
	      $(SRC_DIR)/grammar.tab.cc \
	      $(SRC_DIR)/grammar.tab.h \
	      $(SRC_DIR)/grammar.output \
	      $(SRC_DIR)/lex.yy.c
