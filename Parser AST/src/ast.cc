/*******************************************************************************
 * Name            : ast.cc
 * Project         : fcal
 * Module          : ast
 * Description     : Source file for AST
 * Copyright       : 
 * Original Author : John Carter, Rohith Anil
 * Modifications by: 
 ******************************************************************************/

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <iostream>
#include <string>
#include "include/scanner.h"
#include "include/scanner_class.h"
#include "include/token_class.h"
#include "include/ast.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
namespace fcal {
namespace ast {

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/

// varName Class --------------------------------------------------------------
//! returns the string : lexeme_
std::string varName::UnParse(void) {
  return lexeme_;
}

/// returns the string : lexeme_
std::string varName::CppCode(void) {
  return lexeme_;
}

// Root Class -----------------------------------------------------------------
/*! Returns the string : 
 *     name_->UnParse() + " () " + "{\n" + stmts_->UnParse() + "}\n"
 */
std::string Root::UnParse(void) {
  std::string a = " () ";
  std::string b = "{\n";
  std::string c = "}\n";
  return name_->UnParse() + a + b + stmts_->UnParse() + c;
}

/*! Returns the string:
 *      headers + name_->CppCode() + " () " + "{\n" + stmts_->CppCode() + "}\n"
 */
std::string Root::CppCode(void) {
  std::string iostream_header = "#include <iostream>\n";
  std::string Matrix_header = "#include \"include/Matrix.h\"\n";
  std::string math_header = "#include <math.h>\n";
  std::string namespace_header = "using namespace std;\n";
  std::string a = " () ";
  std::string b = "{\n";
  std::string c = "}\n";
  return iostream_header + math_header + Matrix_header + namespace_header
         + "int " + name_->CppCode() + a + b + stmts_->CppCode() + c;
}

// emptyStmts Class -----------------------------------------------------------
//! Returns the string : ""
std::string emptyStmts::UnParse(void) {
  std::string a = "";
  return a;
}

//! Returns the string : ""
std::string emptyStmts::CppCode(void) {
  std::string a = "";
  return a;
}

// seqStmts Class -------------------------------------------------------------
//! Returns the string : stmt_->UnParse() + stmts_->UnParse()
std::string seqStmts::UnParse(void) {
  return stmt_->UnParse() + stmts_->UnParse();
}

//! Returns the string : stmt_->CppCode() + stmts_->CppCode()
std::string seqStmts::CppCode(void) {
  return stmt_->CppCode() + stmts_->CppCode();
}

// declStmt Class -------------------------------------------------------------
//! Returns the string : decl_->UnParse()
std::string declStmt::UnParse(void) {
  return decl_->UnParse();
}

//! Returns the string : decl_->CppCode()
std::string declStmt::CppCode(void) {
  return decl_->CppCode();
}

// bracketStmt Class ----------------------------------------------------------
//! Returns the string : " {" + stmts_->UnParse() + "}"
std::string bracketStmt::UnParse(void) {
  std::string a = "{\n";
  std::string b = "}\n";
  return a + stmts_->UnParse() + b;
}

//! Returns the string : " {" + stmts_->CppCode() + "}"
std::string bracketStmt::CppCode(void) {
  std::string a = "{\n";
  std::string b = "}\n";
  return a + stmts_->CppCode() + b;
}

// ifStmt Class ---------------------------------------------------------------
//! Returns the string : "if (" + expr_->UnParse() + ")\n" + stmt_->UnParse()
std::string ifStmt::UnParse() {
  std::string a = "if (";
  std::string b = ")\n";
  return a + expr_->UnParse() + b + stmt_->UnParse();
}

//! Returns the string : "if (" + expr_->CppCode() + ")\n" + stmt_->CppCode()
std::string ifStmt::CppCode() {
  std::string a = "if ( ";
  std::string b = " )\n";
  return a + expr_->CppCode() + b + stmt_->CppCode();
}

// ifElseStmt Class -----------------------------------------------------------
/*! Returns the string : 
 *     "if (" + expr_->UnParse() + ")\n" 
 *          + stmt1_->UnParse() + "else " + stmt2_->UnParse()
 */
std::string ifElseStmt::UnParse() {
  std::string a = "if (";
  std::string b = ")\n";
  std::string c = "else\n";
  return a + expr_->UnParse() + b + stmt1_->UnParse()
          + c + stmt2_->UnParse();
}

/*! Returns the string : 
 *     "if (" + expr_->CppCode() + ")\n" 
 *          + stmt1_->CppCode() + "else " + stmt2_->CppCode()
 */
std::string ifElseStmt::CppCode() {
  std::string a = "if (";
  std::string b = " )\n";
  std::string c = "else\n";
  return a + expr_->CppCode() + b + stmt1_->CppCode()
          + c + stmt2_->CppCode();
}

// equalsStmt Class -----------------------------------------------------------
/*! Returns the string : 
 *       var_->UnParse() + " = " + expr1_->UnParse() + ";\n" , 
 *  if isMatrix_ is False.<br>
 *  Returns the string :
 *       var_->UnParse() + " [" + expr1_->UnParse() + ":"
 *            + expr2_->UnParse() +  + "] " + "=" + expr3_->UnParse() + ";\n" ,
 *  if isMatrix_ is True.
 */
std::string equalsStmt::UnParse() {
  if (!isMatrix_) {
      std::string a = " = ";
      std::string b = ";\n";
      return var_->UnParse() + a + expr1_->UnParse() + b;
    } else {
      std::string a = " [";
      std::string b = ":";
      std::string c = "] ";
      std::string d = " = ";
      std::string e = ";\n";
      return var_->UnParse() + a + expr1_->UnParse() + b
          + expr2_->UnParse() + c + d + expr3_->UnParse() + e;
    }
}

/// Returns a C/C++ equivalent assignment Statement
std::string equalsStmt::CppCode() {
  if (!isMatrix_) {
      std::string a = " = ";
      std::string b = ";\n";
      return var_->CppCode() + a + expr1_->CppCode() + b;
    } else {
      return "*(" + var_->CppCode() + ".access(" + expr2_->CppCode() + ", "
          + expr3_->CppCode() + ")) = " + expr1_->CppCode() + ";\n";
    }
}

// printStmt Class ------------------------------------------------------------
//! Returns the string: "print(" + expr_->UnParse() + ");\n"
std::string printStmt::UnParse() {
  std::string a = "print(";
  std::string b = ");\n";
  return a + expr_->UnParse() + b;
}

//! Returns the string: "cout << " + expr_->CppCode() + " ;\n"
std::string printStmt::CppCode() {
  std::string a = "cout << ";
  std::string b = ";\n";
  return a + expr_->CppCode() + b;
}

// repeatStmt Class -----------------------------------------------------------
/*! Returns the string : "repeat (" + var_->UnParse() + " = "
 *  + expr1_->UnParse() + " to " + expr2_->UnParse() + ")\n" + stmt_->UnParse()
 */
std::string repeatStmt::UnParse() {
  std::string a = "repeat (";
  std::string b = " = ";
  std::string c = " to ";
  std::string d = ")\n";
  return a + var_->UnParse() + b + expr1_->UnParse()
      + c + expr2_->UnParse() + d + stmt_->UnParse();
}

/// Returns a for loop
std::string repeatStmt::CppCode() {
  return "for ( " + var_->CppCode() + " = " + expr1_->CppCode() + "; "
          + var_->CppCode() + " <= " + expr2_->CppCode() + "; "
          + var_->CppCode() + "++ )\n" + stmt_->CppCode();
}

// whileStmt Class ------------------------------------------------------------
//! Returns the string : "while (" + expr_->UnParse() + ")\n" + stmt_->UnParse()
std::string whileStmt::UnParse() {
  std::string a = "while (";
  std::string b = ")\n";
  return "while (" + expr_->UnParse() + ")\n" + stmt_->UnParse();
}

//! Returns the string : "while (" + expr_->CppCode() + ")\n" + stmt_->CppCode()
std::string whileStmt::CppCode() {
  std::string a = "while (";
  std::string b = ")\n";
  return "while (" + expr_->CppCode() + ")\n" + stmt_->CppCode();
}

// semiColonStmt Class --------------------------------------------------------
//! Returns the string : ";"
std::string semiColonStmt::UnParse() {
  std::string a = ";\n";
  return a;
}

//! Returns the string : ";"
std::string semiColonStmt::CppCode() {
  std::string a = ";\n";
  return a;
}

// varDecl Class --------------------------------------------------------------
/*! Returns a string depending on the value of type_.<br>
 *  If type_ = int_ then return string : "int " + name_->UnParse() + ";"<br>
 *  If type_ = float_ then return string : "float " + name_->UnParse() + ";"<br>
 *  If type_ = string_ then return string : "string " + name_->UnParse() + ";"<br>
 *  If type_ = boolean_ then return string : "boolean " + name_->UnParse() + ";"
 */
std::string varDecl::UnParse(void) {
  std::string semi = ";\n";
  std::string a = "int ";
  std::string b = "float ";
  std::string c = "string ";
  std::string d = "boolean ";
  switch (type_) {
    case int_:
      return a + name_->UnParse() + semi;
      break;
    case float_:
      return b + name_->UnParse() + semi;
      break;
    case string_:
      return c + name_->UnParse() + semi;
      break;
    case boolean_:
      return d + name_->UnParse() + semi;
      break;
  }
  std::string e = "";
  return e;
}

/// Returns a variable declaration in C/C++
std::string varDecl::CppCode(void) {
  std::string semi = ";\n";
  std::string a = "int ";
  std::string b = "float ";
  std::string c = "char* ";
  std::string d = "bool ";
  switch (type_) {
    case int_:
      return a + name_->CppCode() + semi;
      break;
    case float_:
      return b + name_->CppCode() + semi;
      break;
    case string_:
      return c + name_->CppCode() + semi;
      break;
    case boolean_:
      return d + name_->CppCode() + semi;
      break;
  }
  std::string e = "";
  return e;
}

// matrixDecl Class -----------------------------------------------------------
/*! If simpleMatrix_ is True then return string : 
        "matrix " + var1_->UnParse() + " = " + expr1_->UnParse() + ";"<br>
 Else return : "matrix " + var1_->UnParse() + "[" + expr1_->UnParse() + ":" 
 + expr2_->UnParse() + "] " + var2_->UnParse() + ":" + var3_->UnParse() + " = "
 + expr3_->UnParse() + ";"
 */
std::string matrixDecl::UnParse(void) {
  if (simpleMatrix_) {
      std::string a = "matrix ";
      std::string b = " = ";
      std::string c = ";\n";
      return a + var1_->UnParse() + b + expr1_->UnParse() + c;
    } else {
      std::string a = "matrix ";
      std::string b = "[";
      std::string c = ":";
      std::string d = "] ";
      std::string e = ";\n";
      std::string f = " = ";
      return a + var1_->UnParse() + b + expr1_->UnParse() + c
          + expr2_->UnParse() + d + var2_->UnParse() + c + var3_->UnParse()
          + f + expr3_->UnParse() + e;
    }
}

/*! Returns a matrix Declaration in C/C++ using the matrix class
 *  predefined in Matrix.h
 */
std::string matrixDecl::CppCode(void) {
  if (simpleMatrix_) {
      std::string a = "matrix ";
      std::string b = " = ";
      return a + var1_->CppCode() + "( matrix::" + expr1_->CppCode() + " );\n";
    } else {
      std::string i = var2_->CppCode();
      std::string j = var3_->CppCode();
      std::string x = expr1_->CppCode();
      std::string y = expr2_->CppCode();
      return "matrix " + var1_->CppCode() + "(" + x + ", " + y + ");\n"
          + "for ( int " + i + " = 0; " + i + " < " + x + "; " + i + "++ ) {\n"
          + "for ( int " + j + " = 0; " + j + " < " + y + "; " + j + "++ ) {\n"
          + "*(" + var1_->CppCode() + ".access(" + i + ", " + j + "))"
          + " = " + expr3_->CppCode() + ";\n}\n}\n";
    }
}

// constantExpr Class ---------------------------------------------------------
//! Returns the string : constant_
std::string constantExpr::UnParse(void) {
  return constant_;
}

//! Returns a constant c++ string
std::string constantExpr::CppCode(void) {
  if (constant_ == "True") {
      constant_ = "true";
    } else if (constant_ == "False") {
      constant_ = "false";
    }
  return constant_;
}

// binaryExpr Class -----------------------------------------------------------
//! Retruns the string : expr1_->UnParse() + binaryOp_ + expr2_->UnParse()
std::string binaryExpr::UnParse(void) {
  return expr1_->UnParse() + " " + binaryOp_ + " " + expr2_->UnParse();
}

//! Retruns the string :
/// "( " + expr1_->CppCode() + binaryOp_ + expr2_->CppCode() + " )"
std::string binaryExpr::CppCode(void) {
  return "(" + expr1_->CppCode() + " " + binaryOp_
             + " " + expr2_->CppCode() + ")";
}

// boolExpr Class -------------------------------------------------------------
//! Returns the string : expr1_->UnParse() + boolean_ + expr2_->UnParse()
std::string boolExpr::UnParse() {
  return expr1_->UnParse() + " " + boolean_ + " " + expr2_->UnParse();
}

//! Retruns the string :
/// "( " + expr1_->CppCode() + boolean_ + expr2_->CppCode() + " )"
std::string boolExpr::CppCode(void) {
  return "(" + expr1_->CppCode() + " " + boolean_
             + " " + expr2_->CppCode() + ")";
}

// matrixExpr Class -----------------------------------------------------------
/*! Returns the string : 
 *    var_->UnParse() + " [" + expr1_->UnParse() + ":" + expr2_->UnParse() + "]"
 */
std::string matrixExpr::UnParse() {
  std::string a = " [";
  std::string b = ":";
  std::string c = "]";
  return var_->UnParse() + a + expr1_->UnParse() + b
      + expr2_->UnParse() + c;
}

/// Returns matrix referencing in C/C++ using matrix class in Matrix.h
std::string matrixExpr::CppCode() {
  return "*(" + var_->CppCode() + ".access(" + expr1_->CppCode() + ", "
          + expr2_->CppCode() + "))";
}

// nestedOrExpr Class ---------------------------------------------------------
//! Returns the string : var_->UnParse() + "( " + expr_->UnParse() + " )"
std::string nestedOrExpr::UnParse() {
  std::string a = "(";
  std::string b = ")";
  return var_->UnParse() + "( " + expr_->UnParse() + " )";
}

//! Returns the string : var_->CppCode() + "( " + expr_->CppCode() + " )"
std::string nestedOrExpr::CppCode() {
  if (var_->CppCode() == "n_rows" || var_->CppCode() == "n_cols") {
    return expr_->CppCode() + "." + var_->CppCode() + "()";
  }
  std::string a = "(";
  std::string b = ")";
  return var_->CppCode() + "( " + expr_->CppCode() + " )";
}

// parenthesisExpr Class ------------------------------------------------------
//! Returns the string : "( " + expr_->UnParse() + " )"
std::string parenthesisExpr::UnParse() {
  std::string a = "( ";
  std::string b = " )";
  return a + expr_->UnParse() + b;
}

//! Returns the string : "( " + expr_->CppCode() + " )"
std::string parenthesisExpr::CppCode() {
  std::string a = "(";
  std::string b = ")";
  return a + expr_->CppCode() + b;
}

// letExpr Class --------------------------------------------------------------
/*! Returns the string :
 *      "let " + stmts_->UnParse() + " in " + expr_->UnParse() + " end"
 */
std::string letExpr::UnParse() {
  std::string a = "let ";
  std::string b = " in ";
  std::string c = " end ";
  return a + stmts_->UnParse() + b + expr_->UnParse() + c;
}

/// Returns a string in C/C++ equivalent to letExpr of FCAL
std::string letExpr::CppCode() {
  return "({ " + stmts_->CppCode() + expr_->CppCode() + "; })";
}

// ifExpr Class ---------------------------------------------------------------
/*! Returns the string : "if " + expr1_->UnParse() +  
 *  " then " + expr2_->UnParse() + " else " + expr3_->UnParse();
 */
std::string ifExpr::UnParse() {
  std::string a = "if ";
  std::string b = " then ";
  std::string c = " else ";
  return a + expr1_->UnParse() + b + expr2_->UnParse()
      + c + expr3_->UnParse();
}

/*! Returns ifExpr equivalent in C/C++ which is the conditional
 *  operator i.e. Expr ? Expr : Expr
 */
std::string ifExpr::CppCode() {
  return "( " + expr1_->CppCode() + " ? " + expr2_->CppCode()
          + " : " + expr3_->CppCode() + " )";
}

// notExpr Class --------------------------------------------------------------
//! Returns the string : "!" + expr_->UnParse();
std::string notExpr::UnParse() {
  std::string a = "!";
  return a + expr_->UnParse();
}

//! Returns the string : "!( " + expr_->CppCode() + " )";
std::string notExpr::CppCode() {
  std::string a = "!";
  return a + "( " + expr_->CppCode() + " )";
}

} /* namespace ast */
} /* namespace fcal */
