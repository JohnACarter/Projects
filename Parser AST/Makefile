#Note, you may have to update CXX_DIR and
#CXXTEXT to point the right locations and executables
#to get it to work - look at your Lab 3 Makefile
CXX_DIR = ./lib/cxxtest
CXXTEST = $(CXX_DIR)/bin/cxxtestgen
CXXFLAGS = --error-printer --abort-on-fail --have-eh

FLAGS = -g -Wall -I.

scanner: scanner_class.o token_class.o regex.o read_input.o parser.o ast.o
# Program files.
read_input.o:	src/read_input.cc include/read_input.h
	g++ $(FLAGS) -c src/read_input.cc

regex.o:	src/regex.cc include/regex.h
	g++ $(FLAGS) -c src/regex.cc


scanner_class.o: src/scanner_class.cc include/read_input.h include/regex.h include/scanner_class.h \
																	include/token_class.h
	g++ $(FLAGS) -c src/scanner_class.cc

token_class.o: src/token_class.cc include/token_class.h include/scanner.h
	g++ $(FLAGS) -c src/token_class.cc

parser.o: src/parser.cc include/parser.h include/scanner_class.h include/token_class.h include/ast.h 
	g++ $(FLAGS) -c src/parser.cc

ext_token.o: src/ext_token.cc include/ext_token.h
	g++ $(FLAGS) -c src/ext_token.cc

ast.o: src/ast.cc include/ast.h include/scanner_class.h include/token_class.h 
	g++ $(FLAGS) -c src/ast.cc
# Below is a possible way to make scanner
# Yours may vary depending on your implementation




# Testing files and targets.
# run-tests should work once
# and run regex_tests once you put the files
# we gave you in the right places
# and update the CXX_DIR and CXXTEST variables so they
# reference the correct directory locations
# Add scanner_tests to the dependency list and uncomment when
# you are ready to start testing units with scanner_tests.
run-tests:	regex_tests scanner_tests parser_tests ast_tests codegeneration_tests
	./regex_tests
	./scanner_tests
	./parser_tests
	./ast_tests
	./codegeneration_tests

#This should work once you put the files
#we gave you in the right places
regex_tests:	regex_tests.cc regex.o
	g++ $(FLAGS) -I$(CXX_DIR) -I. -o regex_tests regex.o regex_tests.cc

regex_tests.cc:	include/regex.h tests/regex_tests.h
	$(CXXTEST) $(CXXFLAGS) -o regex_tests.cc tests/regex_tests.h

# Below is a possible way to make scanner_tests and scanner_tests.cc
# Yours may vary depending on your design and implementation
scanner_tests:	scanner_tests.cc scanner_class.o token_class.o regex.o read_input.o
	g++ $(FLAGS) -I$(CXX_DIR) -I. -o scanner_tests \
		scanner_class.o token_class.o regex.o read_input.o scanner_tests.cc

scanner_tests.cc:	scanner_class.o token_class.o tests/scanner_tests.h include/read_input.h
	$(CXXTEST) $(CXXFLAGS) -o scanner_tests.cc tests/scanner_tests.h

parser_tests: parser_tests.cc scanner_class.o token_class.o read_input.o parser.o regex.o ext_token.o ast.o
	g++ $(FLAGS) -I$(CXX_DIR) -I. -o parser_tests parser_tests.cc scanner_class.o token_class.o \
						read_input.o parser.o regex.o ext_token.o ast.o

parser_tests.cc: scanner_class.o token_class.o read_input.o parser.o ext_token.o ast.o regex.o \
tests/parser_tests.h include/ext_token.h include/parser.h include/read_input.h include/scanner.h \
																	include/regex.h include/token_class.h include/scanner_class.h include/ast.h
	$(CXXTEST) $(CXXFLAGS) -o parser_tests.cc tests/parser_tests.h

ast_tests: ast_tests.cc ast.o parser.o scanner_class.o token_class.o read_input.o ext_token.o regex.o
	g++ $(FLAGS) -I$(CXX_DIR) -I. -o ast_tests ast_tests.cc \
		ast.o parser.o scanner_class.o token_class.o read_input.o  ext_token.o regex.o

ast_tests.cc: ast.o parser.o scanner_class.o token_class.o read_input.o ext_token.o regex.o \
		tests/ast_tests.h include/ast.h include/ext_token.h include/parser.h include/read_input.h \
		include/scanner.h include/regex.h include/token_class.h include/scanner_class.h
	$(CXXTEST) $(CXXFLAGS) -o ast_tests.cc tests/ast_tests.h

codegeneration_tests: codegeneration_tests.cc ast.o parser.o scanner_class.o token_class.o \
		read_input.o ext_token.o regex.o
	g++ $(FLAGS) -I$(CXX_DIR) -I. -o codegeneration_tests codegeneration_tests.cc \
		ast.o parser.o scanner_class.o token_class.o read_input.o  ext_token.o regex.o

codegeneration_tests.cc: ast.o parser.o scanner_class.o token_class.o read_input.o ext_token.o regex.o \
		tests/codegeneration_tests.h include/ast.h include/ext_token.h include/parser.h \
		include/read_input.h include/scanner.h include/regex.h \
		include/token_class.h include/scanner_class.h
	$(CXXTEST) $(CXXFLAGS) -o codegeneration_tests.cc tests/codegeneration_tests.h

clean:
	rm -Rf *.o \
		regex_tests regex_tests.cc \
		scanner_tests scanner_tests.cc \
		parser_tests parser_tests.cc \
		ast_tests ast_tests.cc \
		codegeneration_tests codegeneration_tests.cc \
		samples/forest_loss_v2 samples/my_code_1 samples/my_code_2\
		samples/sample_1 samples/sample_2 samples/sample_3 \
		samples/sample_7 samples/sample_8 \
		samples/*.output samples/*.cc \
		samples/*.diff samples/*.dslup1 \
		samples/*.dslup2 samples/*.dslup3

