/*******************************************************************************
 * Name              : regex.h
 * Project           : fcal
 * Module            : ast
 * Description       : Header file for AST
 * Copyright         : 2017 CSCI3081W Staff. All rights reserved.
 * Original Author   : Eric Van Wyk
 * Modifications by  : John Harwell, John Carter, Rohith Anil
 ******************************************************************************/

#ifndef INCLUDE_AST_H_
#define INCLUDE_AST_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <iostream>
#include <string>
#include "include/scanner.h"
#include "include/scanner_class.h"
#include "include/token_class.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
namespace fcal {
namespace ast {

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/

/*! This is the abstract class for ast heirarchy.
 Root, Stmts, Stmt, Decl, Expr and varName are derived from this class.
*/

class Node {
 public:
  /*! UnParse is a polymorphic function which is implemented by concrete
  classes in the ast.
  */
  virtual std::string UnParse(void) { return " This should be pure virtual "; }

  /*! CppCode is a polymorphic function which is implemented by concrete
  classes in the ast.
  */
  virtual std::string CppCode(void) { return " This should be pure virtual"; }

  //! virtual destructor for polymorphism
  virtual ~Node(void) {}
};

//! This class holds the lexeme details of a variable name.
class varName : public Node {
 public:
  /*! This constructor requires one parameter: lexeme
      \param lexeme a string which is the variable name
  */
  explicit varName(std::string lexeme) : lexeme_(lexeme) {}

  //! This function returns the variable name's lexeme.
  std::string UnParse(void);

  std::string CppCode(void);

 private:
  //! A string holding the lexeme of varName
  std::string lexeme_;
};

/*! This is the abstract class which inherits from Node.
 It has no implementation for any functions derived from Node.
 It is the base class for classes which implement the productions
 which are derived from the nonterminal 'Stmts' in the FCAL grammar.
*/
class Stmts : public Node {};

/*! This is the abstract class which inherits from Node.
 It has no implementation for any functions derived from Node.
 It is the base class for classes which implement the productions
 which are derived from the nonterminal 'Stmt' in the FCAL grammar.
*/
class Stmt : public Node {};

/*! This is the abstract class which inherits from Node.
 It has no implementation for any functions derived from Node.
 It is the base class for classes which implement the productions
 which are derived from the nonterminal 'Decl' in the FCAL grammar.
*/
class Decl : public Node {};

/*! This is the abstract class which inherits from Node.
 It has no implementation for any functions derived from Node.
 It is the base class for classes which implement the productions
 which are derived from the nonterminal 'Expr' in the FCAL grammar.
*/
class Expr : public Node {};

/*! This is a concrete class in the ast class heirarchy.
 It implements the production, <br>
                Program ::= varName '(' ')' '{' Stmts '}'
 This class is the root of the ast tree created by the parser.
*/
class Root : public Node {
 public:
  /*! Constructor for the Root class takes parameters
   *  \param name a varName* holding a 
   *     reference to the varName representing the program
   *  \param stmts a Stmts* holding a
   *     reference to the Stmts in the program 
   */
  Root(varName* name, Stmts* stmts)
    : name_(name), stmts_(stmts) {}
  std::string UnParse(void);
  std::string CppCode(void);

 private:
  //! name_ holds the address containing details of program name
  varName* name_;

  /// stmts_ hols the address containing details of Stmts in the
  /// program
  Stmts* stmts_;
};

/*! This is a concrete class in the ast class heirarchy.
 It implements the production, <br>
                Stmts ::= <<empty>>
*/
class emptyStmts : public Stmts {
 public:
  //! Constructor for emptyStmts, it does nothing
  emptyStmts(void) {}

  //! Returns an empty String
  std::string UnParse(void);
  std::string CppCode(void);
};

/*! This is a concrete class in the ast class heirarchy.
 It implements the production, <br>
                Stmts ::= Stmt Stmts
*/
class seqStmts : public Stmts {
 public:
  /*! Constructor which takes parameters
   *  \param stmt a Stmt* which holds the address which
   *	contains details of first Stmt in a sequence of 
   *	Stmts
   *  \param stmts a Stmts* which holds the address which
   *	contains details of rest of the Stmts after the
   *	first Stmt
   */
  seqStmts(Stmt* stmt, Stmts* stmts)
    : stmt_(stmt), stmts_(stmts) {}

  /// Returns a string representing a sequence of Stmts
  std::string UnParse(void);
  std::string CppCode(void);

 private:
  //! stmt_ hols the address which contains details
  //! of first Stmt in a sequence of Stmts
  Stmt *stmt_;

  //! stmts_ hols the address which contains details
  //! of rest of Stmts after the first Stmt
  Stmts *stmts_;
};

/*! This is a concrete class in the ast class heirarchy.
 It implements the production, <br>
                Stmts ::= Stmt Stmts
*/
class declStmt : public Stmt {
 public:
  /*! This constructor takes only one parameter
   *  \param decl a Decl* holding the address which
   *   contains information reagarding a declaration
   */
  explicit declStmt(Decl* decl) : decl_(decl) {}
  std::string UnParse(void);
  std::string CppCode(void);

 private:
  //! decl_ hols the address which contains details
  //! regarding the type of Declaration
  Decl* decl_;
};

/*! This is a concrete class in the ast class heirarchy.
 It implements the production, <br>
                Stmt ::= '{' Stmts '}'
*/
class bracketStmt : public Stmt {
 public:
  /*! This constructor takes only one parameter
   *  \param smts a Stmts* holding the address which
   *  contains details of Stmts under brackects
   * '{' Stmts '}'
   */
  explicit bracketStmt(Stmts *stmts) : stmts_(stmts) {}
  std::string UnParse(void);
  std::string CppCode(void);

 private:
  //! stmts_ hols the address containing details
  //! of Stmts under brackets '{' Stmts '}'
  Stmts *stmts_;
};

/*! This is a concrete class in the ast class heirarchy.
 It implements the production, <br>
                Stmt ::= 'if' '(' Expr ')' Stmt
*/
class ifStmt : public Stmt {
 public:
  /*! This constructor takes two parameters
      \param expr a Expr* holding address which contains
	details of an Expr inside an if condition
      \param stmt a Stmt* holding address which contains
	details of a Stmt which is executed if expr is True
   */
  ifStmt(Expr *expr, Stmt *stmt) : expr_(expr), stmt_(stmt) {}
  std::string UnParse(void);
  std::string CppCode(void);

 private:
  //! Expr to be evaluated inside the if condition
  Expr *expr_;
  //! Stmt to be executed if expr_ is True
  Stmt *stmt_;
};

/*! This is a concrete class in the ast class heirarchy.
 It implements the production, <br>
                Stmt ::= 'if' '(' Expr ')' Stmt 'else' Stmt
*/
class ifElseStmt : public Stmt {
 public:
  /*! This constructor takes two parameters
      \param expr a Expr* holding the addresss which
       contains information regarding an Expr to be 
       evaluated as condition for if-else Stmt
      \param stmt1 a Stmt* holding the address which
       contains information regarding a Stmt to be executed
       if expr is True
      \param stmt2 a Stmt* holding the address which
       contains the information regarding a Stmt to be
       executed if expr is False
   */
  ifElseStmt(Expr *expr, Stmt *stmt1, Stmt *stmt2)
      : expr_(expr), stmt1_(stmt1), stmt2_(stmt2) {}
  std::string UnParse(void);
  std::string CppCode(void);

 private:
  /// Expr to be evaluated as a condition for if-else
  Expr *expr_;
  /// Stmt to be executed if expr_ is True
  Stmt *stmt1_;
  /// Stmt to be executed if expr_ is False
  Stmt *stmt2_;
};

/*! This is a concrete class in the ast class heirarchy.
 It implements the production, <br>
   Stmt ::= varName '=' Expr ';' |
             varName '[' Expr ':' Expr ']' '=' Expr ';'
*/
class equalsStmt : public Stmt {
 public:
  /*! This constructor takes three parameters
      \param var a varName* which holds the address 
       containing the information of varName 
       representing an Expr
      \param expr1 a Expr* holding the address which 
      contains the information of an Expr whose value will be
      assigned to a varName
      \param isMatrix bool value which is True if 
                      asignment is a Martix and False otherwise
   */
  equalsStmt(varName *var, Expr *expr1, bool isMatrix)
      : var_(var), expr1_(expr1), isMatrix_(isMatrix) {}

  /*! This constructor takes five parameters
      \param var varName of a matrix
      \param expr1 Expr representing rows of matrix
      \param expr2 Expr representing columns of matrix
      \param expr3 Expr whise value will be assigned to a
                   particular index in the matrix
      \param isMatrix bool value which is True if 
                      asignment is a Martix and False otherwise
   */
  equalsStmt(varName *var, Expr *expr1, Expr *expr2,
    Expr *expr3, bool isMatrix)
        : var_(var), expr1_(expr1), expr2_(expr2),
          expr3_(expr3), isMatrix_(isMatrix)  {}
  std::string UnParse(void);
  std::string CppCode(void);

 private:
  /// variable name or that of a matrix
  varName *var_;
  /// Expr representing first element of Matrix if isMatrix is True
  /// else it represents an Expr to be assigned to a variable
  Expr *expr1_;
  /// Expr representing second element of Matrix if isMatrix is True
  Expr *expr2_;
  /// Expr which will b assigned to a position in the Matrix if
  /// isMatrix is True
  Expr *expr3_;
  //! isMatrix distinguishes between regular variable or matrix constructor
  bool isMatrix_;
};

/*! This is a concrete class in the ast class heirarchy.
 It implements the production, <br>
                Stmt ::= 'print' '(' Expr ')' ';'
*/
class printStmt : public Stmt {
 public:
  /*! This constructor takes one arguement
      \param expr an expression which should be printed
   */
  explicit printStmt(Expr *expr) : expr_(expr) {}
  std::string UnParse(void);
  std::string CppCode(void);

 private:
  //! An Expr which will be printed
  Expr *expr_;
};

/*! This is a concrete class in the ast class heirarchy.
 It implements the production, <br>
       Stmt ::= 'repeat' '(' varName '=' Expr 'to' Expr ')' Stmt
*/
class repeatStmt : public Stmt {
 public:
  /*! This constructor takes four parameters
      \param var a variable representing the index
      \param expr1 an Expr which is the lower bound of repeat loop
      \param expr2 an Expr which is the upper bound of repeat loop
      \param stmt a Stmt which is executed while the index is within
                  the limits specified by the expr1 and expr2
   */
  repeatStmt(varName *var, Expr *expr1, Expr *expr2, Stmt *stmt)
      : var_(var), expr1_(expr1), expr2_(expr2), stmt_(stmt) {}
  std::string UnParse(void);
  std::string CppCode(void);

 private:
  /// a variable representing the index
  varName *var_;
  /// an Expr which is the lower bound of repeat loop
  Expr *expr1_;
  /// an Expr which is the upper bound of repeat loop
  Expr *expr2_;
  /// a Stmt which is executed while the index is within
  /// the limits specified by the expr1 and expr2
  Stmt *stmt_;
};

/*! This is a concrete class in the ast class heirarchy.
 It implements the production, <br>
          Stmt ::= 'while' '(' Expr ')' Stmt
*/
class whileStmt : public Stmt {
 public:
  /*! The constructor takes two arguements
      \param expr an Expr evluated as a condition in while
      \param stmt a Stmt executed while the Expr is still True
   */
  whileStmt(Expr *expr, Stmt *stmt) : expr_(expr), stmt_(stmt) {}
  std::string UnParse(void);
  std::string CppCode(void);

 private:
  /// an Expr evluated as a condition in while
  Expr *expr_;
  /// stmt_ executed while expr_ is still True
  Stmt *stmt_;
};

/*! This is a concrete class in the ast class heirarchy.
 It implements the production, <br>
                Stmt ::= ';'
*/
class semiColonStmt : public Stmt {
 public:
  /// This is a default constructor and does nothing
  semiColonStmt() {}
  std::string UnParse(void);
  std::string CppCode(void);
};


/*! decType has four possible values based on declaration types.
*/
enum decType {int_, float_, string_, boolean_};

/*! This is a concrete class in the ast class heirarchy.
 It implements the productions, <br>
                Decl ::= 'int' varName ';' <br>
                Decl ::= 'float' varName ';' <br>
                Decl ::= 'string' varName ';' <br>
                Decl ::= 'boolean' varName ';' 
*/
class varDecl : public Decl {
 public:
  /*! This constructor takes two parameters
      \param type a decType which tells the type of declaration
      \param name a varName used as a declaration 
   */
  varDecl(decType type, varName* name)
    : type_(type), name_(name) {}
  std::string UnParse(void);
  std::string CppCode(void);

 private:
  /// type of declaration
  decType type_;
  /// varName declared as a type of type_
  varName* name_;
};

/*! This is a concrete class in the ast class heirarchy.
 It implements the production, <br> 
      Decl ::= 'matrix' varName '[' Expr ':' Expr ']' 
                varName ':' varName  '=' Expr ';' <br>
      Decl ::= 'matrix' varName '=' Expr ';'
*/
class matrixDecl : public Decl {
 public:
  /*! This constructor takes three parameters
      \param var1 a varName repersenting the matrix
      \param expr1 expression assigned to a matrix
      \param simpleMatrix bool which is True if 
             Matrix Declaration is
              Decl ::= 'matrix' varName '=' Expr ';'
   */
  matrixDecl(varName *var1, Expr * expr1, bool simpleMatrix)
      : var1_(var1), expr1_(expr1), simpleMatrix_(simpleMatrix) {}

  /*! This constructor takes seven parameters
      \param var1 a varName repersenting the matrix
      \param var2 a varName repersenting row of matrix
      \param var3 a varName repersenting column of matrix
      \param expr1 expression for row of Matrix
      \param expr2 expression for column of Matrix
      \param expr3 expression assigned to the matrix
      \param simpleMatrix bool which is False if 
             Matrix Declaration is
             Decl ::= 'matrix' varName '[' Expr ':' Expr ']'
             varName ':' varName  '=' Expr ';'
   */
  matrixDecl(varName *var1, varName *var2, varName *var3,
    Expr *expr1, Expr *expr2, Expr *expr3, bool simpleMatrix)
      : var1_(var1), var2_(var2), var3_(var3),
        expr1_(expr1), expr2_(expr2), expr3_(expr3),
        simpleMatrix_(simpleMatrix) {}
  std::string UnParse(void);
  std::string CppCode(void);

 private:
  /// varName representing Matrix name
  varName *var1_;
  /// varName representing Matrix row if simpleMatrix_ is False
  varName *var2_;
  /// varName representing Matrix column if simpleMatrix_ is False
  varName *var3_;
  /// expr assigned to matrix if simpleMatrix_ is True
  /// expr assigned to row of Matrix if simpleMatrix_ is False
  Expr *expr1_;
  /// expr assigned to column of Matrix if simpleMatrix_ is False
  Expr *expr2_;
  /// expr assigned to matrix if simpleMatrix_ is False
  Expr *expr3_;

  //! simpleMatrix distinguishes which constructor is used
  bool simpleMatrix_;
};

/*! This is a concrete class in the ast class heirarchy.
 It implements the productions, <br>
                Expr ::= varName <br>
                Expr ::= integerConst | floatConst |  stringConst <br>
                Expr ::= 'True' | 'False' 
*/
class constantExpr : public Expr {
 public:
  /*! This constructor takes one parameter
      \param const_ takes a string constant representing a
             varName , integer, float , string , True or False
   */
  explicit constantExpr(std::string const_) : constant_(const_) {}
  std::string UnParse(void);
  std::string CppCode(void);

 private:
  /// a string constant representing either one of the following :
  /// varName , integer, float , string , True , False
  std::string constant_;
};

/*! This is a concrete class in the ast class heirarchy.
 It implements the productions,<br>
                Expr ::= Expr '*' Expr <br>
                Expr ::= Expr '/' Expr <br>
                Expr ::= Expr '+' Expr <br>
                Expr ::= Expr '-' Expr 
*/
class binaryExpr : public Expr {
 public:
  /*! This constructor takes three parameters
      \param binaryOp string representing +,/,-,*
      \param expr1 left Expr of binaryOp
      \param expr2 Right Expr of binaryOp
   */
  binaryExpr(std::string binaryOp, Expr *expr1, Expr *expr2)
      : binaryOp_(binaryOp), expr1_(expr1), expr2_(expr2) {}
  std::string UnParse(void);
  std::string CppCode(void);

 private:
  /// string representing +,/,-,*
  std::string binaryOp_;
  /// Expr representing Left Operand in this Binary Operation
  Expr *expr1_;
  /// Expr representing Right Operand in this Binary Operation
  Expr *expr2_;
};

/*! This is a concrete class in the ast class heirarchy.
 It implements the productions,<br>
                Expr ::= Expr '>' Expr <br>
                Expr ::= Expr '>=' Expr <br>
                Expr ::= Expr '<' Expr <br>
                Expr ::= Expr '<=' Expr <br> 
                Expr ::= Expr '==' Expr <br>
                Expr ::= Expr '!=' Expr <br>
                Expr ::= Expr '&&' Expr <br>
                Expr ::= Expr '||' Expr 
*/
class boolExpr : public Expr {
 public:
  /*! This constructor takes three parameters
      \param boolean string representing relational operators
      \param expr1 Expr representing Left Operand 
      \param expr2 Expr representing Right Operand
   */
  boolExpr(std::string boolean, Expr *expr1, Expr *expr2)
      : boolean_(boolean), expr1_(expr1), expr2_(expr2) {}
  std::string UnParse(void);
  std::string CppCode(void);

 private:
  /// string representing relational operators
  std::string boolean_;
  /// Expr representing Left Operand in this realtional Operation
  Expr *expr1_;
  /// Expr representing Right Operand in this relational Operation
  Expr *expr2_;
};

/*! This is a concrete class in the ast class heirarchy.
 It implements the production, <br> <br>
                Expr ::= varName '[' Expr ':' Expr ']'
*/
class matrixExpr : public Expr {
 public:
  /*! This constructor takes three parameters
      \param var varName representing matrix name
      \param expr1 Expr which represents row of Matrix
      \param expr2 Expr which represents column of Matrix
   */
  matrixExpr(varName *var, Expr *expr1, Expr *expr2)
      : var_(var), expr1_(expr1), expr2_(expr2) {}
  std::string UnParse(void);
  std::string CppCode(void);

 private:
  /// varName representing matrix name
  varName *var_;
  /// Expr which represents row of Matrix
  Expr *expr1_;
  /// Expr which represents column of Matrix
  Expr *expr2_;
};

/*! This is a concrete class in the ast class heirarchy.
 It implements the production, <br>
                Expr ::= varName '(' Expr ')'
*/
class nestedOrExpr : public Expr {
 public:
  /*! This constructor takes 2 parameters
      \param var varName before Expr enclosed in paranthesis
      \param expr Expr contained in paranthesis
   */
  nestedOrExpr(varName *var, Expr *expr) : var_(var), expr_(expr) {}
  std::string UnParse(void);
  std::string CppCode(void);

 private:
  /// varName before Expr enclosed in paranthesis
  varName *var_;
  /// Expr contained in paranthesis
  Expr *expr_;
};

/*! This is a concrete class in the ast class heirarchy.
 It implements the production, <br>
                Expr ::= '(' Expr ')'
*/
class parenthesisExpr : public Expr {
 public:
  /*! This constructor takes one paramters
      \param expr an Expr enclosed in parenthesis
   */
  explicit parenthesisExpr(Expr *expr) : expr_(expr) {}
  std::string UnParse(void);
  std::string CppCode(void);

 private:
  /// an Expr enclosed in parenthesis
  Expr *expr_;
};

/*! This is a concrete class in the ast class heirarchy.
 It implements the production, <br>
                Expr ::= 'let' Stmts 'in' Expr 'end'
*/
class letExpr : public Expr {
 public:
  /*! This constructor takes two parameters
      \param stmts Stmts in a Let 
      \param expr Expr in a Let 
   */
  letExpr(Stmts *stmts, Expr *expr) : stmts_(stmts), expr_(expr) {}
  std::string UnParse(void);
  std::string CppCode(void);

 private:
  /// Stmts in a Let
  Stmts *stmts_;
  /// Expr in a let
  Expr *expr_;
};

/*! This is a concrete class in the ast class heirarchy.
 It implements the production, <br>
                Expr ::= 'if' Expr 'then' Expr 'else' Expr
*/
class ifExpr : public Expr {
 public:
  /*! This constructor takes three parameters
      \param expr1 Expr to be evaluated as a condition
      \param expr2 Expr to be evaluated if expr1 is True
      \param expr3 Expr to be evaluated if expr1 is False
   */
  ifExpr(Expr *expr1, Expr *expr2, Expr *expr3)
      : expr1_(expr1), expr2_(expr2), expr3_(expr3) {}
  std::string UnParse(void);
  std::string CppCode(void);

 private:
  /// Expr to be evaluated as a condition
  Expr *expr1_;
  /// Expr to be evaluated if expr1_ is True
  Expr *expr2_;
  /// Expr to be evaluated if expr1_ is False
  Expr *expr3_;
};

/*! This is a concrete class in the ast class heirarchy.
 It implements the production, <br> <br>
               Expr ::= '!' Expr
*/
class notExpr : public Expr {
 public:
  /*! This constructor takes one parameter
      \param expr Expr whose boolean result will be negated
   */
  explicit notExpr(Expr *expr) : expr_(expr) {}
  std::string UnParse(void);
  std::string CppCode(void);

 private:
  /// Expr whose boolean result will be negated
  Expr *expr_;
};

} /* namespace ast */
} /* namespace fcal */

#endif  // INCLUDE_AST_H_
