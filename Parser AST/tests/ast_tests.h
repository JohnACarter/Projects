#include <cxxtest/TestSuite.h>

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fstream>

#include "include/ext_token.h"
#include "include/parse_result.h"
#include "include/parser.h"
#include "include/read_input.h"
#include "include/scanner.h"
#include "include/scanner_class.h"
#include "include/token_class.h"
#include "include/ast.h"

using namespace std;
using namespace fcal::scanner;
using namespace fcal::parser;
using namespace fcal::ast;

class AstTestSuite : public CxxTest::TestSuite
{
public:

    Parser *p;
    void test_setup_code () {
      p = new Parser();
      TS_ASSERT(p);
    }

    void writeFile ( const string text, const string filename) {
        ofstream out(filename.c_str());
        out << (string) text << endl ;
    }

    /** The code necessary to implement these test cases were done by 04-02-2017.
     *  Tests sample_1, sample_3, sample_5, mysample and iter3_sample_1 passed on 04-02-2017.
     *  Test sample_2 passed on 04-04-2017.
     *  Tests sample_4 and forest_loss passed on 05-04-2017.
     */
    void unparse_tests ( string file) {
        string path = "./samples/" + file ;

        // 1. Test that the file can be parsed.
        ParseResult pr1 = p->Parse(ReadInputFromFile( path.c_str()));
        TSM_ASSERT(file + " failed to parse.", pr1.ok()) ;

        // 2. Verify that the ast field is not null
        TSM_ASSERT(file + " failed to generate an AST.", pr1.ast() != NULL);

        // 3. Verify that the "unparsing" is non-empty.
        string up1 = pr1.ast()->UnParse() ;
        writeFile(up1,(path+"up1").c_str());
        TSM_ASSERT(file + " failed to generate non-empty unparsing.",
                     up1.length() > 0) ;

        // 4. Verify that the un-parsed string can be parsed.
        ParseResult pr2 = p->Parse(up1.c_str()) ;
        TSM_ASSERT(file + " failed to parse the first un-parsing.",
                     pr2.ok()) ;

        // 5. Verify that the ast field is not null after first unparsing
        TSM_ASSERT(file + " first unparsing failed to generate an AST.",
                     pr2.ast() != NULL);

        // 6. Verify that this second unparsing can be parsed.
        string up2 = pr2.ast()->UnParse() ;
        writeFile(up2,(path+"up2").c_str());
        ParseResult pr3 = p->Parse(up2.c_str()) ;
        TSM_ASSERT(file + " failed to unparse the second un-parsing.",
                     pr3.ok()) ;

        string up3 = pr3.ast()->UnParse() ;
        writeFile(up3,(path+"up3").c_str());
        // 7. Verify that the first and second unparsings are the same.
        TSM_ASSERT_EQUALS(file + " unparse-1 != unparse-2.", up1, up2) ;
        // 8. Verifty that the second and third unparsings are the same.
        TSM_ASSERT_EQUALS(file + " unparse-2 != unparse-3.", up2, up3) ;
    }

				// We passed sample1.dsl April 2nd. Intially we got the simple variable
				// declaration to pass. On the same day, April 2nd, we finished the rest of 
				// parser.cc and tried the other tests one by one. After trying the tests 1-7
				// in different combinations, at the end of the day, April 2nd, we had tests 
				// 1,4,5,6,7 passing by fixing problems/segmentation faults that were specified 
				// and correlating them to specific classes that were causing the problems.
    void test_iter3_sample_1(void) { unparse_tests("iter3_sample1.dsl"); }
    void test_sample_1(void) { unparse_tests("sample_1.dsl"); }

				// April 2nd we fixed a memory issue in our scanner_class.cc that was causing a 
				// segmentation fault, along with a problem with one of the constuctors in 
				// a concrete class. After fixing these problems we were able to pass tests
				// 2 and 3, along with the remaining tests, so that by the end of the day, April 3rd,
				// we had passed all tests and were satisfied that our implentation was done.
			 
    void test_sample_2(void) { unparse_tests("sample_2.dsl"); }
    void test_sample_3(void) { unparse_tests("sample_3.dsl"); }
    void test_sample_4(void) { unparse_tests("sample_4.dsl"); }
    void test_sample_5(void) { unparse_tests("sample_5.dsl"); }
    void test_mysample(void) { unparse_tests("mysample.dsl"); }
    void test_forest_loss(void) { unparse_tests("forest_loss_v2.dsl"); }
} ;
