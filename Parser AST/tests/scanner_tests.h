#include <cxxtest/TestSuite.h>

#include <stdio.h>
#include "include/read_input.h"
#include "include/scanner.h"
#include "include/token_class.h"
#include "include/scanner_class.h"

using namespace std;
namespace scanner = fcal::scanner;
class ScannerTestSuite : public CxxTest::TestSuite
{
public:
    /* A Scanner object is created in the test_setup_code method.
       This is a test, beginning with "test_", so that it is executed
       by the testing framework.  The scanner is used in some tests of
       the method "scan".
     */
    scanner::Scanner *s;
				scanner::Token *tk;
				scanner::Token *next;
    void test_setup_code () {
      s = new scanner::Scanner();
						tk = new scanner::Token();
						next = new scanner::Token();
    }
				

    // Tests for components and functions used by "scan"
    // --------------------------------------------------

    /* You should test the regular expressions you use to make and match
       regular expressions,and the functions used by makeRegex and
       matchRegex regex_tests.h.
       However, You will likely need to write several tests to
       adequately test  the functions that are called from "scan".
       Once you are confident that the components used by "scan"
       work properly, then you can run tests on the "scan" method itself.
    */

    // You should place tests for these other functions used by
    // scan below.


				void test_scan_find_terminal() {
								scanner::TokenType t[] = {scanner::kIntKwd};
								scanner::TokenType terminal;
								int index = 0;
								terminal = static_cast<scanner::TokenType>(index);
        TS_ASSERT(t != NULL);
        
        TS_ASSERT_EQUALS( terminal, t[0]);
    }

				void test_scan_find_terminal_no_match() {
								scanner::TokenType t[] = {scanner::kIntKwd};
								scanner::TokenType terminal;
								int index = 22;
								terminal = static_cast<scanner::TokenType>(index);
        TS_ASSERT(t != NULL);
        
        TS_ASSERT_DIFFERS( terminal, t[0]);
    }

    void test_set_token() {
      TS_ASSERT(tk != NULL);
      tk->set_token(scanner::kIntKwd, "int");
      TS_ASSERT_EQUALS(tk->lexeme(), "int");
      TS_ASSERT_EQUALS(tk->terminal(), scanner::kIntKwd);
    }

    void test_set_next() {
      TS_ASSERT(tk != NULL);
      TS_ASSERT(tk != NULL);
      tk->set_next(next);
      TS_ASSERT_EQUALS(tk->next(), next);
    }

    /* Below is one of the tests for these components in the project
       solution created by your instructor.  It uses a helper
       function function called "tokenMaker_tester", which you have
       not been given.  You are expected to design your own components
       for "scan" and your own mechanisms for easily testing them.

        void xtest_TokenMaker_leftCurly () {
            tokenMaker_tester ("{ ", "^\\{", leftCurly, "{");
        }

        Note that this test is here named "xtest_Token..." The leading
        "x" is so that cxxTest doesn't scan the line above and think
        it is an actual test that isn't commented out.  cxxTest is
        very simple and doesn't even process block comments.
    */

    void test_TokenMaker (const char* text, scanner::TokenType terminal, const std::string lexeme,scanner::Token *tks) {
      tks = s->Scan(text);
      TS_ASSERT(tks != NULL);
      TS_ASSERT_EQUALS(tks->terminal(), terminal);
      TS_ASSERT_EQUALS(tks->lexeme(), lexeme);
    }


    /* You must have at least one separate test case for each terminal
       symbol.  Thus, you need a test that will pass or fail based
       solely on the regular expression (and its corresponding code)
       for each terminal symbol.

       This requires a test case for each element of the enumerated
       type tokenType.  This may look something like the sample test
       shown in the comment above.
    */

   void test_terminals () {
     const char* text_strings[] = {"int", "float", "boolean", "True",
                                   "False", "string", "matrix",
                                   "let", "in", "end", "if", "then",
                                   "else", "repeat", "while", "print",
                                   "to", "239847", "234.23455",
                                   "\"This is a String\"",
                                   "var_name_001", "(", ")", "{", "}",
                                   "[", "]", ";", ":", "=", "+", "*",
                                   "-", "/", "<", "<=", ">", ">=", "==",
                                   "!=", "&&", "||", "!", "", "$"};
     for (int i=0; i<=44; i++) {
       test_TokenMaker(text_strings[i], static_cast<scanner::TokenType>(i), text_strings[i], tk);
     }
   }

/*
    void xtest_terminal_intKwd () {
      scanner::Token *tks = s->Scan("int ");
      TS_ASSERT(tks != NULL);
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kIntKwd);
      TS_ASSERT_EQUALS(tks->lexeme(), "int");
      TS_ASSERT(tks->next() != NULL);
      tks = tks->next();
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
      TS_ASSERT(tks->next() == NULL);
    }

    void xtest_terminal_floatKwd () {
      scanner::Token *tks = s->Scan("float ");
      TS_ASSERT(tks != NULL);
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kFloatKwd);
      TS_ASSERT_EQUALS(tks->lexeme(), "float");
      TS_ASSERT(tks->next() != NULL);
      tks = tks->next();
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
      TS_ASSERT(tks->next() == NULL);
    }

    void xtest_terminal_stringKwd () {
      scanner::Token *tks = s->Scan("string ");
      TS_ASSERT(tks != NULL);
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kStringKwd);
      TS_ASSERT_EQUALS(tks->lexeme(), "string");
      TS_ASSERT(tks->next() != NULL);
      tks = tks->next();
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
      TS_ASSERT(tks->next() == NULL);
    }


    void xtest_terminal_matrixKwd () {
      scanner::Token *tks = s->Scan("matrix ");
      TS_ASSERT(tks != NULL);
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kMatrixKwd);
      TS_ASSERT_EQUALS(tks->lexeme(), "matrix");
      TS_ASSERT(tks->next() != NULL);
      tks = tks->next();
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
      TS_ASSERT(tks->next() == NULL);
    }

    void xtest_terminal_letKwd () {
      scanner::Token *tks = s->Scan("let ");
      TS_ASSERT(tks != NULL);
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kLetKwd);
      TS_ASSERT_EQUALS(tks->lexeme(), "let");
      TS_ASSERT(tks->next() != NULL);
      tks = tks->next();
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
      TS_ASSERT(tks->next() == NULL);
    }

    void xtest_terminal_inKwd () {
      scanner::Token *tks = s->Scan("in ");
      TS_ASSERT(tks != NULL);
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kInKwd);
      TS_ASSERT_EQUALS(tks->lexeme(), "in");
      TS_ASSERT(tks->next() != NULL);
      tks = tks->next();
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
      TS_ASSERT(tks->next() == NULL);
    }

    void xtest_terminal_endKwd () {
      scanner::Token *tks = s->Scan("end ");
      TS_ASSERT(tks != NULL);
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndKwd);
      TS_ASSERT_EQUALS(tks->lexeme(), "end");
      TS_ASSERT(tks->next() != NULL);
      tks = tks->next();
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
      TS_ASSERT(tks->next() == NULL);
    }

    void xtest_terminal_ifKwd () {
      scanner::Token *tks = s->Scan("if ");
      TS_ASSERT(tks != NULL);
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kIfKwd);
      TS_ASSERT_EQUALS(tks->lexeme(), "if");
      TS_ASSERT(tks->next() != NULL);
      tks = tks->next();
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
      TS_ASSERT(tks->next() == NULL);
    }

    void xtest_terminal_thenKwd () {
      scanner::Token *tks = s->Scan("then ");
      TS_ASSERT(tks != NULL);
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kThenKwd);
      TS_ASSERT_EQUALS(tks->lexeme(), "then");
      TS_ASSERT(tks->next() != NULL);
      tks = tks->next();
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
      TS_ASSERT(tks->next() == NULL);
    }

    void xtest_terminal_elseKwd () {
      scanner::Token *tks = s->Scan("else ");
      TS_ASSERT(tks != NULL);
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kElseKwd);
      TS_ASSERT_EQUALS(tks->lexeme(), "else");
      TS_ASSERT(tks->next() != NULL);
      tks = tks->next();
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
      TS_ASSERT(tks->next() == NULL);
    }

    void xtest_terminal_repeatKwd () {
      scanner::Token *tks = s->Scan("repeat ");
      TS_ASSERT(tks != NULL);
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kRepeatKwd);
      TS_ASSERT_EQUALS(tks->lexeme(), "repeat");
      TS_ASSERT(tks->next() != NULL);
      tks = tks->next();
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
      TS_ASSERT(tks->next() == NULL);
    }

    void xtest_terminal_printKwd () {
      scanner::Token *tks = s->Scan("print ");
      TS_ASSERT(tks != NULL);
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kPrintKwd);
      TS_ASSERT_EQUALS(tks->lexeme(), "print");
      TS_ASSERT(tks->next() != NULL);
      tks = tks->next();
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
      TS_ASSERT(tks->next() == NULL);
    }

    void xtest_terminal_toKwd () {
      scanner::Token *tks = s->Scan("to ");
      TS_ASSERT(tks != NULL);
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kToKwd);
      TS_ASSERT_EQUALS(tks->lexeme(), "to");
      TS_ASSERT(tks->next() != NULL);
      tks = tks->next();
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
      TS_ASSERT(tks->next() == NULL);
    }

    void xtest_terminal_intConst () {
      scanner::Token *tks = s->Scan("29865 ");
      TS_ASSERT(tks != NULL);
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kIntConst);
      TS_ASSERT_EQUALS(tks->lexeme(), "29865");
      TS_ASSERT(tks->next() != NULL);
      tks = tks->next();
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
      TS_ASSERT(tks->next() == NULL);
    }

    void xtest_terminal_floatConst () {
      scanner::Token *tks = s->Scan("234.2565 ");
      TS_ASSERT(tks != NULL);
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kFloatConst);
      TS_ASSERT_EQUALS(tks->lexeme(), "234.2565");
      TS_ASSERT(tks->next() != NULL);
      tks = tks->next();
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
      TS_ASSERT(tks->next() == NULL);
    }

    void xtest_terminal_stringConst () {
      scanner::Token *tks = s->Scan("\"this\"");
      TS_ASSERT(tks != NULL);
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kStringConst);
      TS_ASSERT_EQUALS(tks->lexeme(), "\"this\"");
      TS_ASSERT(tks->next() != NULL);
      tks = tks->next();
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
      TS_ASSERT(tks->next() == NULL);
    }

    void xtest_terminal_variableName () {
      scanner::Token *tks = s->Scan("temp_var ");
      TS_ASSERT(tks != NULL);
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kVariableName);
      TS_ASSERT_EQUALS(tks->lexeme(), "temp_var");
      TS_ASSERT(tks->next() != NULL);
      tks = tks->next();
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
      TS_ASSERT(tks->next() == NULL);
    }

    void xtest_terminal_leftParen () {
      scanner::Token *tks = s->Scan("( ");
      TS_ASSERT(tks != NULL);
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kLeftParen);
      TS_ASSERT_EQUALS(tks->lexeme(), "(");
      TS_ASSERT(tks->next() != NULL);
      tks = tks->next();
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
      TS_ASSERT(tks->next() == NULL);
    }

    void xtest_terminal_rightParen () {
      scanner::Token *tks = s->Scan(") ");
      TS_ASSERT(tks != NULL);
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kRightParen);
      TS_ASSERT_EQUALS(tks->lexeme(), ")");
      TS_ASSERT(tks->next() != NULL);
      tks = tks->next();
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
      TS_ASSERT(tks->next() == NULL);
    }

    void xtest_terminal_leftCurly () {
      scanner::Token *tks = s->Scan("{ ");
      TS_ASSERT(tks != NULL);
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kLeftCurly);
      TS_ASSERT_EQUALS(tks->lexeme(), "{");
      TS_ASSERT(tks->next() != NULL);
      tks = tks->next();
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
      TS_ASSERT(tks->next() == NULL);
    }

    void xtest_terminal_rightCurly () {
      scanner::Token *tks = s->Scan("} ");
      TS_ASSERT(tks != NULL);
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kRightCurly);
      TS_ASSERT_EQUALS(tks->lexeme(), "}");
      TS_ASSERT(tks->next() != NULL);
      tks = tks->next();
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
      TS_ASSERT(tks->next() == NULL);
    }

    void xtest_terminal_leftSquare () {
      scanner::Token *tks = s->Scan("[ ");
      TS_ASSERT(tks != NULL);
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kLeftSquare);
      TS_ASSERT_EQUALS(tks->lexeme(), "[");
      TS_ASSERT(tks->next() != NULL);
      tks = tks->next();
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
      TS_ASSERT(tks->next() == NULL);
    }

    void xtest_terminal_rightSquare () {
      scanner::Token *tks = s->Scan("] ");
      TS_ASSERT(tks != NULL);
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kRightSquare);
      TS_ASSERT_EQUALS(tks->lexeme(), "]");
      TS_ASSERT(tks->next() != NULL);
      tks = tks->next();
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
      TS_ASSERT(tks->next() == NULL);
    }

    void xtest_terminal_semiColon () {
      scanner::Token *tks = s->Scan("; ");
      TS_ASSERT(tks != NULL);
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kSemiColon);
      TS_ASSERT_EQUALS(tks->lexeme(), ";");
      TS_ASSERT(tks->next() != NULL);
      tks = tks->next();
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
      TS_ASSERT(tks->next() == NULL);
    }

    void xtest_terminal_colon () {
      scanner::Token *tks = s->Scan(": ");
      TS_ASSERT(tks != NULL);
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kColon);
      TS_ASSERT_EQUALS(tks->lexeme(), ":");
      TS_ASSERT(tks->next() != NULL);
      tks = tks->next();
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
      TS_ASSERT(tks->next() == NULL);
    }

    void xtest_terminal_assign () {
      scanner::Token *tks = s->Scan("= ");
      TS_ASSERT(tks != NULL);
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kAssign);
      TS_ASSERT_EQUALS(tks->lexeme(), "=");
      TS_ASSERT(tks->next() != NULL);
      tks = tks->next();
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
      TS_ASSERT(tks->next() == NULL);
    }

    void xtest_terminal_plusSign () {
      scanner::Token *tks = s->Scan("+ ");
      TS_ASSERT(tks != NULL);
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kPlusSign);
      TS_ASSERT_EQUALS(tks->lexeme(), "+");
      TS_ASSERT(tks->next() != NULL);
      tks = tks->next();
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
      TS_ASSERT(tks->next() == NULL);
    }

    void xtest_terminal_star () {
      scanner::Token *tks = s->Scan("* ");
      TS_ASSERT(tks != NULL);
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kStar);
      TS_ASSERT_EQUALS(tks->lexeme(), "*");
      TS_ASSERT(tks->next() != NULL);
      tks = tks->next();
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
      TS_ASSERT(tks->next() == NULL);
    }

    void xtest_terminal_dash () {
      scanner::Token *tks = s->Scan("- ");
      TS_ASSERT(tks != NULL);
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kDash);
      TS_ASSERT_EQUALS(tks->lexeme(), "-");
      TS_ASSERT(tks->next() != NULL);
      tks = tks->next();
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
      TS_ASSERT(tks->next() == NULL);
    }

    void xtest_terminal_forwardSlash () {
      scanner::Token *tks = s->Scan("/ ");
      TS_ASSERT(tks != NULL);
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kForwardSlash);
      TS_ASSERT_EQUALS(tks->lexeme(), "/");
      TS_ASSERT(tks->next() != NULL);
      tks = tks->next();
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
      TS_ASSERT(tks->next() == NULL);
    }

    void xtest_terminal_lessThan () {
      scanner::Token *tks = s->Scan("< ");
      TS_ASSERT(tks != NULL);
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kLessThan);
      TS_ASSERT_EQUALS(tks->lexeme(), "<");
      TS_ASSERT(tks->next() != NULL);
      tks = tks->next();
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
      TS_ASSERT(tks->next() == NULL);
    }

    void xtest_terminal_lessThanEqual () {
      scanner::Token *tks = s->Scan("<= ");
      TS_ASSERT(tks != NULL);
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kLessThanEqual);
      TS_ASSERT_EQUALS(tks->lexeme(), "<=");
      TS_ASSERT(tks->next() != NULL);
      tks = tks->next();
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
      TS_ASSERT(tks->next() == NULL);
    }

    void xtest_terminal_greaterThan () {
      scanner::Token *tks = s->Scan("> ");
      TS_ASSERT(tks != NULL);
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kGreaterThan);
      TS_ASSERT_EQUALS(tks->lexeme(), ">");
      TS_ASSERT(tks->next() != NULL);
      tks = tks->next();
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
      TS_ASSERT(tks->next() == NULL);
    }

    void xtest_terminal_greaterThanEqual () {
      scanner::Token *tks = s->Scan(">= ");
      TS_ASSERT(tks != NULL);
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kGreaterThanEqual);
      TS_ASSERT_EQUALS(tks->lexeme(), ">=");
      TS_ASSERT(tks->next() != NULL);
      tks = tks->next();
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
      TS_ASSERT(tks->next() == NULL);
    }

    void xtest_terminal_equalsEquals () {
      scanner::Token *tks = s->Scan("== ");
      TS_ASSERT(tks != NULL);
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kEqualsEquals);
      TS_ASSERT_EQUALS(tks->lexeme(), "==");
      TS_ASSERT(tks->next() != NULL);
      tks = tks->next();
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
      TS_ASSERT(tks->next() == NULL);
    }

    void xtest_terminal_notEquals () {
      scanner::Token *tks = s->Scan("!= ");
      TS_ASSERT(tks != NULL);
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kNotEquals);
      TS_ASSERT_EQUALS(tks->lexeme(), "!=");
      TS_ASSERT(tks->next() != NULL);
      tks = tks->next();
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
      TS_ASSERT(tks->next() == NULL);
    }

    void xtest_terminal_andOp () {
      scanner::Token *tks = s->Scan("&& ");
      TS_ASSERT(tks != NULL);
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kAndOp);
      TS_ASSERT_EQUALS(tks->lexeme(), "&&");
      TS_ASSERT(tks->next() != NULL);
      tks = tks->next();
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
      TS_ASSERT(tks->next() == NULL);
    }

    void xtest_terminal_orOp () {
      scanner::Token *tks = s->Scan("|| ");
      TS_ASSERT(tks != NULL);
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kOrOp);
      TS_ASSERT_EQUALS(tks->lexeme(), "||");
      TS_ASSERT(tks->next() != NULL);
      tks = tks->next();
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
      TS_ASSERT(tks->next() == NULL);
    }

    void xtest_terminal_notOp () {
      scanner::Token *tks = s->Scan("! ");
      TS_ASSERT(tks != NULL);
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kNotOp);
      TS_ASSERT_EQUALS(tks->lexeme(), "!");
      TS_ASSERT(tks->next() != NULL);
      tks = tks->next();
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
      TS_ASSERT(tks->next() == NULL);
    }

    void xtest_terminal_endOfFile () {
      scanner::Token *tks = s->Scan("");
      TS_ASSERT(tks != NULL);
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
      TS_ASSERT(tks->next() == NULL);
    }

    void xtest_terminal_lexicalError () {      
      scanner::Token *tks = s->Scan("$ ");
      TS_ASSERT(tks != NULL);
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kLexicalError);
      TS_ASSERT_EQUALS(tks->lexeme(), "$");
      TS_ASSERT(tks->next() != NULL);
      tks = tks->next();
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
      TS_ASSERT(tks->next() == NULL);
    }

    // Tests for terminals added during iter2

    void xtest_terminal_boolKwd () {
      scanner::Token *tks = s->Scan("boolean ");
      TS_ASSERT(tks != NULL);
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kBoolKwd);
      TS_ASSERT_EQUALS(tks->lexeme(), "boolean");
      TS_ASSERT(tks->next() != NULL);
      tks = tks->next();
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
      TS_ASSERT(tks->next() == NULL);
    }

    void xtest_terminal_trueKwd () {
      scanner::Token *tks = s->Scan("True ");
      TS_ASSERT(tks != NULL);
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kTrueKwd);
      TS_ASSERT_EQUALS(tks->lexeme(), "True");
      TS_ASSERT(tks->next() != NULL);
      tks = tks->next();
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
      TS_ASSERT(tks->next() == NULL);
    }

    void xtest_terminal_falseKwd () {
      scanner::Token *tks = s->Scan("False ");
      TS_ASSERT(tks != NULL);
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kFalseKwd);
      TS_ASSERT_EQUALS(tks->lexeme(), "False");
      TS_ASSERT(tks->next() != NULL);
      tks = tks->next();
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
      TS_ASSERT(tks->next() == NULL);
    }

    void xtest_terminal_whileKwd () {
      scanner::Token *tks = s->Scan("while ");
      TS_ASSERT(tks != NULL);
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kWhileKwd);
      TS_ASSERT_EQUALS(tks->lexeme(), "while");
      TS_ASSERT(tks->next() != NULL);
      tks = tks->next();
      TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
      TS_ASSERT(tks->next() == NULL);
    } */

    // Tests for "scan"
    // --------------------------------------------------

    /* Below are some helper functions and sample tests for testing the
       "scan" method on Scanner.
    */

    // Test that a list of tokens has no lexicalError tokens.
    bool noLexicalErrors (scanner::Token *tks) {
        scanner::Token *currentToken = tks;
        while (currentToken != NULL) {
          if (currentToken->terminal() == scanner::kLexicalError) {
              printf("problem: %s\n",currentToken->lexeme().c_str());
              fflush(stdout);
                                return false;
            }
            else {
              currentToken = currentToken->next();
            }
        }
        return true;
    }

    /* A quick, but inaccurate, test for scanning files.  It only
       checks that no lexical errors occurred, not that the right
       tokens were returned.
    */
    void scanFileNoLexicalErrors(const char* filename) {
        char *text =  scanner::ReadInputFromFile(filename);
        TS_ASSERT (text);
        scanner::Token *tks = s->Scan(text);
        TS_ASSERT(tks != NULL);
        TS_ASSERT(noLexicalErrors(tks));
    }

				

    /* This function checks that the terminal fields in the list of
       tokens matches a list of terminals.
    */
  bool sameTerminals(scanner::Token *tks, int numTerms,
                     scanner::TokenType *ts) {
        scanner::Token *currentToken = tks;
        int termIndex = 0;
        while(currentToken != NULL && termIndex < numTerms) {
			
            if(currentToken->terminal() != ts[termIndex]) {
                printf("item at index: %i: lexeme:%s terminal: %i should be terminal: %i\n",termIndex,currentToken->lexeme().c_str(),currentToken->terminal(), ts[termIndex]);
				fflush(stdout);
                return false;
            }
            else {
                ++ termIndex;
                currentToken = currentToken->next();
            }
        }
        return true;
    }


    /* Below are the provided test files that your code should also
       pass.

       You may initially want to rename these tests to "xtest_..." so
       that the CxxTest framework does not see it as a test and won't
       run it as one.  This way you can focus on the tests that you'll
       write above for the individual terminal types first. Then focus
       on these tests.

    */

    // The "endOfFile" token is always the last one in the list of tokens.
    void test_scan_empty() {
        scanner::Token *tks = s->Scan("  ");
        TS_ASSERT(tks != NULL);
        TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
        TS_ASSERT(tks->next() == NULL);
    }

    void test_scan_empty_comment() {
        scanner::Token *tks = s->Scan(" /* a comment */ ");
        TS_ASSERT(tks != NULL);
        TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
        TS_ASSERT(tks->next() == NULL);
    }

    // When a lexical error occurs, the scanner creates a token with a
    // single-character lexeme and lexicalError as the terminal.
   void test_scan_lexicalErrors() {
        scanner::Token *tks = s->Scan("$&1  ");
        TS_ASSERT(tks != NULL);
        TS_ASSERT_EQUALS(tks->terminal(), scanner::kLexicalError);
        TS_ASSERT_EQUALS(tks->lexeme(), "$");
        tks = tks->next();
        TS_ASSERT(tks != NULL);
        TS_ASSERT_EQUALS(tks->terminal(), scanner::kLexicalError);
        TS_ASSERT_EQUALS(tks->lexeme(), "&");
        tks = tks->next();
        TS_ASSERT(tks != NULL);
        TS_ASSERT_EQUALS(tks->terminal(), scanner::kIntConst);
        TS_ASSERT_EQUALS(tks->lexeme(), "1");
        tks = tks->next();
        TS_ASSERT(tks != NULL);
        TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
        TS_ASSERT(tks->next() == NULL);
    }


    // A test for scanning numbers and a variable.

    void test_scan_nums_vars() {
        scanner::Token *tks = s->Scan(" 123 x 1.23 ");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = { scanner::kIntConst, scanner::kVariableName,
                                           scanner::kFloatConst, scanner::kEndOfFile };
        TS_ASSERT( sameTerminals( tks, 4, ts));
    }


    /* This test checks that the scanner returns a list of tokens with
       the correct terminal fields.  It doesn't check that the lexemes
       are correct.
     */

    void test_scan_bad_syntax_good_tokens() {
        const char *filename = "./samples/bad_syntax_good_tokens.dsl";
        char *text =  scanner::ReadInputFromFile(filename);
        TS_ASSERT(text);
        scanner::Token *tks = NULL;
        tks = s->Scan( text);
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = {
          scanner::kIntConst, scanner::kIntConst, scanner::kIntConst, scanner::kIntConst,
          scanner::kStringConst, scanner::kStringConst, scanner::kStringConst,
          scanner::kFloatConst, scanner::kFloatConst, scanner::kFloatConst,
          scanner::kMatrixKwd,
          scanner::kSemiColon, scanner::kColon, scanner::kToKwd,
          scanner::kLeftCurly, scanner::kLeftParen, scanner::kRightCurly, scanner::kRightParen,
          scanner::kPlusSign, scanner::kStar, scanner::kDash, scanner::kForwardSlash,
          scanner::kEqualsEquals, scanner::kLessThanEqual,
          scanner::kGreaterThanEqual, scanner::kNotEquals,
          scanner::kAssign,
          scanner::kVariableName, scanner::kVariableName, scanner::kVariableName, scanner::kVariableName,
          scanner::kVariableName, scanner::kVariableName, scanner::kVariableName,
          scanner::kIntKwd, scanner::kFloatKwd, scanner::kStringKwd,
          scanner::kEndOfFile
        };
        int count = 38;
        TS_ASSERT( sameTerminals( tks, count, ts));
    }

    void test_scan_sample_forestLoss() {
        scanFileNoLexicalErrors("./samples/forest_loss_v2.dsl");
    }



};
