/****************************************************************************
 * Name            : parser.h
 * Project         : fcal
 * Module          : parser
 * Copyright       : 2017 CSCI3081W Staff. All rights reserved.
 * Original Author : Erik Van Wyk
 * Modifications by: Dan Challou, John Harwell,
 *                   Team Nostromo: Rohith Anil, John Carter
 *
 ******************************************************************************/

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "include/parser.h"
#include <assert.h>
#include <stdio.h>
#include "include/ext_token.h"
#include "include/scanner.h"
#include "include/scanner_class.h"
#include "include/token_class.h"
#include "include/ast.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
/// Namespaces
namespace fcal {
namespace parser {

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
/// Parser constructors/destructors
Parser::~Parser() {
  if (scanner_) delete scanner_;

  scanner::ExtToken *to_delete1;
  curr_token_ = tokens_;
  while (curr_token_) {
    to_delete1 = curr_token_;
    curr_token_ = curr_token_->next();
    delete to_delete1;
  } /** while() */

  scanner::Token *curr_scanner_token = stokens_;
  scanner::Token *to_delete2;
  while (curr_scanner_token) {
    to_delete2 = curr_scanner_token;
    curr_scanner_token = curr_scanner_token->next();
    delete to_delete2;
  } /** while() */
} /** Parser::~Parser() */

ParseResult Parser::Parse(const char *text) {
  assert(text != NULL);

  ParseResult pr;
  try {
    scanner_ = new scanner::Scanner();
    stokens_ = scanner_->Scan(text);
    tokens_ = tokens_->ExtendTokenList(this, stokens_);

    assert(tokens_ != NULL);
    curr_token_ = tokens_;
    pr = ParseProgram();
  }
  catch (std::string errMsg) {
    pr.ok(false);
    pr.errors(errMsg);
    pr.ast(NULL);
  }
  return pr;
} /** Parser::parse() */

/**
 * parse methods for non-terminal symbols
 * --------------------------------------
 */

/// Program -- This is the root node where the entire program will be placed
/// once everything has been parsed and placed in the proper node of the
/// abstract syntax tree(AST). This function's comments describe the general
/// structure of the overall program, and specifically how each function
/// parses the program accoding to the grammar of that function and how it
/// then creates the AST from that grammar. Each function will follow this
/// same general structure.
ParseResult Parser::ParseProgram() {
  ParseResult pr;
  /// root -- the following line shows the grammar that this function is
  /// intended to parse to create the specific node needed for the AST.
  ///
  /// Program ::= varName '(' ')' '{' Stmts '}'

  /// String name holds varName in grammar after finding kVariableName,
  /// setting varName class with name. This same form will continue throughout
  /// program for any function requiring a variable name in the grammar.
  match(scanner::kVariableName);
  std::string name(prev_token_->lexeme());
  ast::varName *var = new ast::varName(name);

  match(scanner::kLeftParen);
  match(scanner::kRightParen);
  match(scanner::kLeftCurly);

  /** pr_stmts will hold all statements after 
   *  recursively calling appropriate grammar.
   *  ParseResult pr_stmts, pr_expr, pr_decl, 
   *  or similar variations will follow the
   *  same form throughout the program calling each 
   *  function recursively when a declaration,
   *  statement,or expression is needed according to 
   *  the specific grammar that the
   *  current function requires.
   *  ParseResult pr_stmts = parse_stmts();
   */
  ParseResult pr_stmts = parse_stmts();

  /** ast::Stmts stmts will hold the Node *ast 
   *  value from pr_stmts.
   *  This same form will continue throughout the 
   *  program to set the current Expr, Stmt,
   *  or Decl class *ast value. These *ast values are 
   *  what will be used to set the values for the 
   *  subclasses that will be used to create the nodes for the AST.
   */
  ast::Stmts *stmts = NULL;

  /** The following checks that pr_stmts ast value isn't null, 
   *  and that stmts was properly assigned the value from 
   *  ParseResult's Node class *ast.
   */
  if (pr_stmts.ast()) {
    stmts = dynamic_cast<ast::Stmts *>(pr_stmts.ast());
    if (!stmts) throw((std::string) "Bad cast of State in parseProgram");
    }

  match(scanner::kRightCurly);
  match(scanner::kEndOfFile);

  /// Set the Root node with the entire program, varName {statements}.
  /// This same form will be used throughout the program to set the respective
  /// variables, statements, expressions, and declarations to the proper node of
  /// the AST according to the grammar for the current function.
  ast::Root *root = new ast::Root(var, stmts);

  /// Set the ParseResults Node *ast with the final program once everthing
  /// is parsed. This same form will continue throughout the program,
  /// pr.ast(node) sets the primary ParseResult object with the node that holds
  /// the values acquired during the current function according to its grammar.
  pr.ast(root);
  return pr;
}  /// Parser::ParseProgram()

/// MatrixDecl
// identical purpose of parse_decl, handles special matrix syntax.
ParseResult Parser::parse_matrix_decl() {
  ParseResult pr;
  /// simpleMatirx determines which grammar to use for Matrix.
  bool simpleMatrix = true;

  match(scanner::kMatrixKwd);
  match(scanner::kVariableName);

  /// Set the varName for the matrix declaration
  std::string name1(prev_token_->lexeme());
  ast::varName *var1_ = new ast::varName(name1);

  /// First check if we have a full matrix declaration.
  /// Parse each part of the grammar below setting
  /// 3 variable names and 3 expressions.
  /// Decl ::= 'matrix' varName '[' Expr ':' Expr ']'
  ///            varName ':' varName  '=' Expr ';'
  if (attempt_match(scanner::kLeftSquare)) {
    simpleMatrix = false;

    /// expr1
    ParseResult pr_expr1 = parse_expr(0);
    ast::Expr *expr1 = NULL;
    if (pr_expr1.ast()) {
      expr1 = dynamic_cast<ast::Expr*>(pr_expr1.ast());
      if (!expr1)
        throw((std::string) "Bad cast of State in parse_matrix_decl");
      }

    match(scanner::kColon);

    /// expr2
    ParseResult pr_expr2 = parse_expr(0);
    ast::Expr *expr2 = NULL;
    if (pr_expr2.ast()) {
      expr2 = dynamic_cast<ast::Expr*>(pr_expr2.ast());
      if (!expr2)
        throw((std::string) "Bad cast of State in parse_matrix_decl");
      }

    match(scanner::kRightSquare);
    match(scanner::kVariableName);

    /// varName2
    std::string name2(prev_token_->lexeme());
    ast::varName *var2_ = new ast::varName(name2);

    match(scanner::kColon);
    match(scanner::kVariableName);

    /// varName3
    std::string name3(prev_token_->lexeme());
    ast::varName *var3_ = new ast::varName(name3);

    match(scanner::kAssign);

    /// expr3
    ParseResult pr_expr3 = parse_expr(0);
    ast::Expr *expr3 = NULL;
    if (pr_expr3.ast()) {
      expr3 = dynamic_cast<ast::Expr*>(pr_expr3.ast());
      if (!expr3)
        throw((std::string) "Bad cast of State in parse_matrix_decl");
      }

    /// Create decl node for matrix.
    ast::Decl *decl = new ast::matrixDecl(var1_, var2_, var3_,
                                          expr1, expr2, expr3,
                                          simpleMatrix);
    pr.ast(decl);

  } else if (attempt_match(scanner::kAssign)) {
    /// If we have a simple matrix dexlaration,
    /// we only need to set the first variable name and 1 expression.
    /// Decl ::= 'matrix' varName '=' Expr ';'
    simpleMatrix = true;

    /// expr1
    ParseResult pr_expr1 = parse_expr(0);
    ast::Expr *expr1 = NULL;
    if (pr_expr1.ast()) {
      expr1 = dynamic_cast<ast::Expr*>(pr_expr1.ast());
      if (!expr1) throw((std::string) "Bad cast of State in parse_matrix_decl");
      }

    /// Create decl node for simple matrix.
    ast::Decl *decl = new ast::matrixDecl(var1_, expr1, simpleMatrix);
    pr.ast(decl);

  } else {
    throw((std::string) "Bad Syntax of Matrix Decl in in parse_matrix_decl");
  }

  match(scanner::kSemiColon);
  return pr;
}  /// Parser::matrixDecl

/// standardDecl
/// Decl ::= integerKwd varName | floatKwd varName | stringKwd varName
ParseResult Parser::parse_standard_decl() {
  ParseResult pr;
  /// decType type is an enumerated type to show the variable type
  /// that is being declared.
  ast::decType type;

  if (attempt_match(scanner::kIntKwd)) {  // Type ::= intKwd
     type = ast::int_;
    } else if (attempt_match(scanner::kFloatKwd)) {  // Type ::= floatKwd
        type = ast::float_;
    } else if (attempt_match(scanner::kStringKwd)) {  // Type ::= stringKwd
        type = ast::string_;
    } else if (attempt_match(scanner::kBoolKwd)) {  // Type ::= boolKwd
        type = ast::boolean_;
    }

  match(scanner::kVariableName);

  /// varName
  std::string name(prev_token_->lexeme());
  ast::varName *var = new ast::varName(name);

  match(scanner::kSemiColon);

  /// Create decl node for variable declaration.
  ast::Decl *decl = new ast::varDecl(type, var);
  pr.ast(decl);
  return pr;
}  /// Parser::standardDecl

/// Determines whether declaration is a matrix
/// or standard variable declaration.
ParseResult Parser::parse_decl() {
  ParseResult pr;
  /// Decl :: matrix variableName ....
  if (next_is(scanner::kMatrixKwd)) {
    pr = parse_matrix_decl();
  } else {
    /// Decl ::= Type variableName semiColon
    pr = parse_standard_decl();
  }
  return pr;
}

/// Parse general statements
ParseResult Parser::parse_stmts() {
  ParseResult pr;
  if (!next_is(scanner::kRightCurly) && !next_is(scanner::kInKwd)) {
    /// Stmts ::= Stmt Stmts

    /// stmt
    ParseResult pr_stmt = parse_stmt();
    ast::Stmt *stmt = NULL;
    if (pr_stmt.ast()) {
      stmt = dynamic_cast<ast::Stmt *>(pr_stmt.ast());
      if (!stmt) throw((std::string) "Bad cast of State in parse_stmts");
      }

    /// stmts
    ParseResult pr_stmts = parse_stmts();
    ast::Stmts *stmts = NULL;
    if (pr_stmts.ast()) {
      stmts = dynamic_cast<ast::Stmts *>(pr_stmts.ast());
      if (!stmts) throw((std::string) "Bad cast of State in parse_stmts");
      }

    /// Create stmts node
    ast::Stmts *statements = new ast::seqStmts(stmt, stmts);
    pr.ast(statements);
  } else {
    /// No statements
    // nothing to match.k
      ast::Stmts *empty = new ast::emptyStmts();
      pr.ast(empty);
  }
  return pr;
}

/// Parse a general statement.
ParseResult Parser::parse_stmt() {
  ParseResult pr;

  /// If we have a declaration statement:
  /// Stmt ::= Decl
  if (next_is(scanner::kIntKwd) || next_is(scanner::kFloatKwd) ||
      next_is(scanner::kMatrixKwd) || next_is(scanner::kStringKwd) ||
      next_is(scanner::kBoolKwd)) {
    /// decl
    ParseResult pr_decl = parse_decl();
    ast::Decl *decl = NULL;
    if (pr_decl.ast()) {
      decl = dynamic_cast<ast::Decl *>(pr_decl.ast());
      if (!decl) throw((std::string) "Bad cast of State in parse_stmt");
      }

    /// Create new declStmt node
    ast::Stmt *dStmt = new ast::declStmt(decl);
    pr.ast(dStmt);

  } else if (attempt_match(scanner::kLeftCurly)) {
    /// Stmt ::= '{' Stmts '}'

    /// stmts
    ParseResult pr_stmts = parse_stmts();
    ast::Stmts *stmts = NULL;
    if (pr_stmts.ast()) {
      stmts = dynamic_cast<ast::Stmts *>(pr_stmts.ast());
      if (!stmts) throw((std::string) "Bad cast of State in parse_stmt");
      }

    /// Create bracketStmt node
    ast::Stmt *bracket = new ast::bracketStmt(stmts);
    pr.ast(bracket);
    match(scanner::kRightCurly);

  } else if (attempt_match(scanner::kIfKwd)) {
    /// Stmt ::= 'if' '(' Expr ')' Stmt
    /// Stmt ::= 'if' '(' Expr ')' Stmt 'else' Stmt
    match(scanner::kLeftParen);

    /// expr
    ParseResult pr_expr = parse_expr(0);
    ast::Expr *expr = NULL;
    if (pr_expr.ast()) {
      expr = dynamic_cast<ast::Expr *>(pr_expr.ast());
      if (!expr) throw((std::string) "Bad cast of State in parse_stmt");
      }
    match(scanner::kRightParen);

    /// stmt
    ParseResult pr_stmt = parse_stmt();
    ast::Stmt *stmt = NULL;
    if (pr_stmt.ast()) {
      stmt = dynamic_cast<ast::Stmt *>(pr_stmt.ast());
      if (!stmt) throw((std::string) "Bad cast of State in parse_stmt");
      }

    /// Create ifStmt node
    ast::Stmt *if_stmt = new ast::ifStmt(expr, stmt);
    pr.ast(if_stmt);

    /// It is if-else statment
    if (attempt_match(scanner::kElseKwd)) {
      /// elseStmt
      ParseResult pr_else = parse_stmt();
      ast::Stmt *elseStmt = NULL;
      if (pr_else.ast()) {
        elseStmt = dynamic_cast<ast::Stmt *>(pr_else.ast());
        if (!elseStmt) throw((std::string) "Bad cast of State in parse_stmt");
        }

      /// Create ifElseStmt node
      ast::Stmt *else_stmt = new ast::ifElseStmt(expr, stmt, elseStmt);
      pr.ast(else_stmt);
    }
  } else if (attempt_match(scanner::kVariableName)) {
      /// We have an assignment statement
      /// Stmt ::= varName '=' Expr ';'  | varName '[' Expr ':' Expr ']'
      /// '=' Expr ';'

      /// isMatrix is used to show matrix or standard assginment
      bool isMatrix = false;

      /// varName
      std::string name(prev_token_->lexeme());
      ast::varName *var = new ast::varName(name);

      ParseResult pr_expr2;
      ParseResult pr_expr3;
      ast::Expr *expr2 = NULL;
      ast::Expr *expr3 = NULL;
    if (attempt_match(scanner::kLeftSquare)) {
      /// We have a matrix assignment
      isMatrix = true;

      /// expr2
      pr_expr2 = parse_expr(0);
      if (pr_expr2.ast()) {
        expr2 = dynamic_cast<ast::Expr*>(pr_expr2.ast());
        if (!expr2) throw((std::string) "Bad cast of State in parse_stmt");
        }
      match(scanner::kColon);

      /// expr3
      pr_expr3 = parse_expr(0);
      if (pr_expr3.ast()) {
        expr3 = dynamic_cast<ast::Expr*>(pr_expr3.ast());
        if (!expr3) throw((std::string) "Bad cast of State in parse_stmt");
        }
      match(scanner::kRightSquare);
    }
    match(scanner::kAssign);

    /// expr1
    ParseResult pr_expr1 = parse_expr(0);
    ast::Expr *expr1 = NULL;
    if (pr_expr1.ast()) {
      expr1 = dynamic_cast<ast::Expr*>(pr_expr1.ast());
      if (!expr1) throw((std::string) "Bad cast of State in parse_stmt");
      }

    /// Create equalsStmt node for matrix or standard declaration
    if (isMatrix) {
        ast::Stmt *assign2 = new ast::equalsStmt(var, expr1, expr2,
                                                 expr3, isMatrix);
        pr.ast(assign2);
      } else {
        ast::Stmt *assign1 = new ast::equalsStmt(var, expr1, isMatrix);
        pr.ast(assign1);
      }

    match(scanner::kSemiColon);

  } else if (attempt_match(scanner::kPrintKwd)) {
    /// We have a print statement
    /// Stmt ::= 'print' '(' Expr ')' ';'
    match(scanner::kLeftParen);

    /// expr
    ParseResult pr_expr = parse_expr(0);
    ast::Expr *expr = NULL;
    if (pr_expr.ast()) {
      expr = dynamic_cast<ast::Expr *>(pr_expr.ast());
      if (!expr) throw((std::string) "Bad cast of State in parse_stmt");
      }

    match(scanner::kRightParen);
    match(scanner::kSemiColon);

    /// Create printStmt node
    ast::Stmt *print_ = new ast::printStmt(expr);
    pr.ast(print_);

  } else if (attempt_match(scanner::kRepeatKwd)) {
    /// We have a repeat statement
    /// Stmt ::= 'repeat' '(' varName '=' Expr 'to' Expr ')' Stmt
    match(scanner::kLeftParen);
    match(scanner::kVariableName);

    /// varName
    std::string name(prev_token_->lexeme());
    ast::varName *var = new ast::varName(name);

    match(scanner::kAssign);

    /// expr1
    ParseResult pr_expr1 = parse_expr(0);
    ast::Expr *expr1 = NULL;
    if (pr_expr1.ast()) {
      expr1 = dynamic_cast<ast::Expr*>(pr_expr1.ast());
      if (!expr1) throw((std::string) "Bad cast of State in parse_stmt");
      }

    match(scanner::kToKwd);

    /// expr2
    ParseResult pr_expr2 = parse_expr(0);
    ast::Expr *expr2 = NULL;
    if (pr_expr2.ast()) {
      expr2 = dynamic_cast<ast::Expr*>(pr_expr2.ast());
      if (!expr2) throw((std::string) "Bad cast of State in parse_stmt");
      }

    match(scanner::kRightParen);

    /// stmt
    ParseResult pr_stmt = parse_stmt();
    ast::Stmt *stmt = NULL;
    if (pr_stmt.ast()) {
      stmt = dynamic_cast<ast::Stmt *>(pr_stmt.ast());
      if (!stmt) throw((std::string) "Bad cast of State in parse_stmt");
      }

    /// Create repeatStmt node
    ast::Stmt *repeat_ = new ast::repeatStmt(var, expr1, expr2, stmt);
    pr.ast(repeat_);

  } else if (attempt_match(scanner::kWhileKwd)) {
    /// We have a while statement
    /// Stmt ::= 'while' '(' Expr ')' Stmt
    match(scanner::kLeftParen);

    /// expr
    ParseResult pr_expr = parse_expr(0);
    ast::Expr *expr = NULL;
    if (pr_expr.ast()) {
      expr = dynamic_cast<ast::Expr *>(pr_expr.ast());
      if (!expr) throw((std::string) "Bad cast of State in parse_stmt");
      }
    match(scanner::kRightParen);

    /// stmt
    ParseResult pr_stmt = parse_stmt();
    ast::Stmt *stmt = NULL;
    if (pr_stmt.ast()) {
      stmt = dynamic_cast<ast::Stmt *>(pr_stmt.ast());
      if (!stmt) throw((std::string) "Bad cast of State in parse_stmt");
      }

    /// Create whileStmt node
    ast::Stmt *while_ = new ast::whileStmt(expr, stmt);
    pr.ast(while_);

  } else if (attempt_match(scanner::kSemiColon)) {
    /// Stmt ::= ';
    // parsed a skip
  } else {
    throw(make_error_msg(curr_token_->terminal()) +
          " while parsing a statement");
  }
  // Stmt ::= variableName assign Expr semiColon
  return pr;
}  /// Parser::parse_stmt

/// Expr
ParseResult Parser::parse_expr(int rbp) {
  /* Examine current token, without consuming it, to call its
     associated parse methods.  The ExtToken objects have 'nud' and
     'led' methods that are dispatchers that call the appropriate
     parse methods.*/
  ParseResult left = curr_token_->nud();

  while (rbp < curr_token_->lbp()) {
    left = curr_token_->led(left);
  }

  return left;
}  /// Parser::parse_expr


/// Parse each specific expression and create the proper node in the AST.

/// Expr ::= trueKwd
ParseResult Parser::parse_true_kwd() {
  ParseResult pr;
  match(scanner::kTrueKwd);

  std::string name(prev_token_->lexeme());
  ast::Expr *expr = new ast::constantExpr(name);
  pr.ast(expr);
  return pr;
}

/// Expr ::= falseKwd
ParseResult Parser::parse_false_kwd() {
  ParseResult pr;
  match(scanner::kFalseKwd);

  std::string name(prev_token_->lexeme());
  ast::Expr *expr = new ast::constantExpr(name);
  pr.ast(expr);
  return pr;
}

/// Expr ::= intConst
ParseResult Parser::parse_int_const() {
  ParseResult pr;
  match(scanner::kIntConst);

  std::string name(prev_token_->lexeme());
  ast::Expr *expr = new ast::constantExpr(name);
  pr.ast(expr);
  return pr;
}

/// Expr ::= floatConst
ParseResult Parser::parse_float_const() {
  ParseResult pr;
  match(scanner::kFloatConst);

  std::string name(prev_token_->lexeme());
  ast::Expr *expr = new ast::constantExpr(name);
  pr.ast(expr);
  return pr;
}

/// Expr ::= stringConst
ParseResult Parser::parse_string_const() {
  ParseResult pr;
  match(scanner::kStringConst);

  std::string name(prev_token_->lexeme());
  ast::Expr *expr = new ast::constantExpr(name);
  pr.ast(expr);
  return pr;
}

/// Expr ::= variableName .....
ParseResult Parser::parse_variable_name() {
  ParseResult pr;
  match(scanner::kVariableName);

  std::string name(prev_token_->lexeme());
  ast::varName *var = new ast::varName(name);
  if (attempt_match(scanner::kLeftSquare)) {
    /// Expr ::= matrix [Expr:Expr]
    ParseResult pr_expr1 = parse_expr(0);

    /// expr1
    ast::Expr *expr1 = NULL;
    if (pr_expr1.ast()) {
      expr1 = dynamic_cast<ast::Expr*>(pr_expr1.ast());
      if (!expr1)
        throw((std::string) "Bad cast of State in parse_variable_name");
      }
    match(scanner::kColon);

    /// expr2
    ParseResult pr_expr2 = parse_expr(0);
    ast::Expr *expr2 = NULL;
    if (pr_expr2.ast()) {
      expr2 = dynamic_cast<ast::Expr*>(pr_expr2.ast());
      if (!expr2)
        throw((std::string) "Bad cast of State in parse_variable_name");
      }
    match(scanner::kRightSquare);

    /// Create matrixExpr node
    ast::Expr *matrix_ = new ast::matrixExpr(var, expr1, expr2);
    pr.ast(matrix_);
  }  else if (attempt_match(scanner::kLeftParen)) {
    /// Expr ::= varableName '(' Expr ')'
    ParseResult pr_expr = parse_expr(0);

    /// expr
    ast::Expr *expr = NULL;
    if (pr_expr.ast()) {
      expr = dynamic_cast<ast::Expr *>(pr_expr.ast());
      if (!expr)
        throw((std::string) "Bad cast of State in parse_variable_name");
      }
    match(scanner::kRightParen);

    /// Create nestedExpr node
    ast::Expr *nested = new ast::nestedOrExpr(var, expr);
    pr.ast(nested);

  } else {
    /// We have right hand side variable expression
      ast::Expr *varConst = new ast::constantExpr(name);
      pr.ast(varConst);
  }
  return pr;
}

/// Expr ::= leftParen Expr rightParen
ParseResult Parser::parse_nested_expr() {
  ParseResult pr;
  match(scanner::kLeftParen);

  ParseResult pr_paren = parse_expr(0);
  ast::Expr *expr = NULL;
  if (pr_paren.ast()) {
    expr = dynamic_cast<ast::Expr *>(pr_paren.ast());
    if (!expr) throw((std::string) "Bad cast of State in parse_nested_expr");
    }
  ast::Expr *paren = new ast::parenthesisExpr(expr);
  pr.ast(paren);
  match(scanner::kRightParen);
  return pr;
}

/// Expr ::= 'if' Expr 'then' Expr 'else' Expr
ParseResult Parser::parse_if_expr() {
  ParseResult pr;

  match(scanner::kIfKwd);

  /// expr1
  ParseResult pr_expr1 = parse_expr(0);
  ast::Expr *expr1 = NULL;
  if (pr_expr1.ast()) {
    expr1 = dynamic_cast<ast::Expr*>(pr_expr1.ast());
    if (!expr1) throw((std::string) "Bad cast of State in parse_if_expr");
    }
  match(scanner::kThenKwd);

  /// expr2
  ParseResult pr_expr2 = parse_expr(0);
  ast::Expr *expr2 = NULL;
  if (pr_expr2.ast()) {
    expr2 = dynamic_cast<ast::Expr*>(pr_expr2.ast());
    if (!expr2) throw((std::string) "Bad cast of State in parse_if_expr");
    }
  match(scanner::kElseKwd);

  /// expr3
  ParseResult pr_expr3 = parse_expr(0);
  ast::Expr *expr3 = NULL;
  if (pr_expr3.ast()) {
    expr3 = dynamic_cast<ast::Expr*>(pr_expr3.ast());
    if (!expr3) throw((std::string) "Bad cast of State in parse_if_expr");
    }

  /// Create ifExpr node
  ast::Expr *if_ = new ast::ifExpr(expr1, expr2, expr3);
  pr.ast(if_);

  return pr;
}

/// Expr ::= 'let' Stmts 'in' Expr 'end'
ParseResult Parser::parse_let_expr() {
  ParseResult pr;
  match(scanner::kLetKwd);

  /// stmts
  ParseResult pr_stmts = parse_stmts();
  ast::Stmts *stmts = NULL;
  if (pr_stmts.ast()) {
    stmts = dynamic_cast<ast::Stmts *>(pr_stmts.ast());
    if (!stmts) throw((std::string) "Bad cast of State in parse_let_expr");
    }
  match(scanner::kInKwd);

  /// expr
  ParseResult pr_expr = parse_expr(0);
  ast::Expr *expr = NULL;
  if (pr_expr.ast()) {
    expr = dynamic_cast<ast::Expr *>(pr_expr.ast());
    if (!expr) throw((std::string) "Bad cast of State in parse_let_expr");
    }
  match(scanner::kEndKwd);

  /// Create letExpr node
  ast::Expr *let_ = new ast::letExpr(stmts, expr);
  pr.ast(let_);

  return pr;
}

/// Expr ::= '!' Expr
ParseResult Parser::parse_not_expr() {
  ParseResult pr;
  match(scanner::kNotOp);

  ParseResult pr_expr = parse_expr(0);
  ast::Expr *expr = NULL;
  if (pr_expr.ast()) {
    expr = dynamic_cast<ast::Expr *>(pr_expr.ast());
    if (!expr) throw((std::string) "Bad cast of State in parse_not_expr");
    }
  ast::Expr *not_ = new ast::notExpr(expr);
  pr.ast(not_);
  return pr;
}

/// Expr ::= Expr plusSign Expr
ParseResult Parser::parse_addition(ParseResult prLeft) {
  /// parser has already matched left expression
  ParseResult pr;

  /// expr1
  ast::Expr *expr1 = NULL;
  if (prLeft.ast()) {
    expr1 = dynamic_cast<ast::Expr*>(prLeft.ast());
    if (!expr1) throw((std::string) "Bad cast of State in parse_addition");
    }
  match(scanner::kPlusSign);

  /// string +
  std::string name(prev_token_->lexeme());

  /// expr2
  ParseResult pr_expr2 = parse_expr(prev_token_->lbp());
  ast::Expr *expr2 = NULL;
  if (pr_expr2.ast()) {
    expr2 = dynamic_cast<ast::Expr*>(pr_expr2.ast());
    if (!expr2) throw((std::string) "Bad cast of State in parse_addition");
    }

  /// Create binaryExpr node
  ast::Expr *add_ = new ast::binaryExpr(name, expr1, expr2);
  pr.ast(add_);
  return pr;
}

/// Expr ::= Expr star Expr
ParseResult Parser::parse_multiplication(ParseResult prLeft) {
  // parser has already matchekD left expression
  ParseResult pr;

  /// expr1
  ast::Expr *expr1 = NULL;
  if (prLeft.ast()) {
    expr1 = dynamic_cast<ast::Expr*>(prLeft.ast());
    if (!expr1)
      throw((std::string) "Bad cast of State in parse_multiplication");
    }
  match(scanner::kStar);

  /// string *
  std::string name(prev_token_->lexeme());

  /// expr1
  ParseResult pr_expr2 = parse_expr(prev_token_->lbp());
  ast::Expr *expr2 = NULL;
  if (pr_expr2.ast()) {
    expr2 = dynamic_cast<ast::Expr*>(pr_expr2.ast());
    if (!expr2)
      throw((std::string) "Bad cast of State in parse_multiplication");
    }

  /// Create binaryExpr node
  ast::Expr *star_ = new ast::binaryExpr(name, expr1, expr2);
  pr.ast(star_);
  return pr;
}

// Expr ::= Expr dash Expr
ParseResult Parser::parse_subtraction(ParseResult prLeft) {
  // parser has already matchekD left expression
  ParseResult pr;

  /// expr1
  ast::Expr *expr1 = NULL;
  if (prLeft.ast()) {
    expr1 = dynamic_cast<ast::Expr*>(prLeft.ast());
    if (!expr1) throw((std::string) "Bad cast of State in parse_subtraction");
    }
  match(scanner::kDash);

  /// string -
  std::string name(prev_token_->lexeme());

  /// expr2
  ParseResult pr_expr2 = parse_expr(prev_token_->lbp());
  ast::Expr *expr2 = NULL;
  if (pr_expr2.ast()) {
    expr2 = dynamic_cast<ast::Expr*>(pr_expr2.ast());
    if (!expr2) throw((std::string) "Bad cast of State in parse_subtraction");
    }

  /// Create binaryExpr node
  ast::Expr *dash_ = new ast::binaryExpr(name, expr1, expr2);
  pr.ast(dash_);
  return pr;
}

/// Expr ::= Expr forwardSlash Expr
ParseResult Parser::parse_division(ParseResult prLeft) {
  /// parser has already matched left expression
  ParseResult pr;

  /// expr1
  ast::Expr *expr1 = NULL;
  if (prLeft.ast()) {
    expr1 = dynamic_cast<ast::Expr*>(prLeft.ast());
    if (!expr1) throw((std::string) "Bad cast of State in parse_division");
    }
  match(scanner::kForwardSlash);

  /// string /
  std::string name(prev_token_->lexeme());

  /// expr2
  ParseResult pr_expr2 = parse_expr(prev_token_->lbp());
  ast::Expr *expr2 = NULL;
  if (pr_expr2.ast()) {
    expr2 = dynamic_cast<ast::Expr*>(pr_expr2.ast());
    if (!expr2) throw((std::string) "Bad cast of State in parse_division");
    }

  /// Create binaryExpr node
  ast::Expr *slash_ = new ast::binaryExpr(name, expr1, expr2);
  pr.ast(slash_);
  return pr;
}

/// Expr ::= Expr equalEquals Expr
/// Expr ::= Expr lessThanEquals Expr
/// Expr ::= Expr greaterThanEquals Expr
/// Expr ::= Expr notEquals Expr
/// Expr ::= Expr leftAngle Expr
/// Expr ::= Expr rightAngle Expr
/// Notice that for relational operators we use just one parse
/// function.  This shows another possible means for implementing
/// expressions, as opposed to the method used for arithmetic
/// expressions in which each operation has its own parse method.  It
/// will depend on what we do in iteration 3 in building an abstract
/// syntax tree to decide which method is better.

ParseResult Parser::parse_relational_expr(ParseResult prLeft) {
  /// parser has already matched left expression
  ParseResult pr;

  /// expr1
  ast::Expr *expr1 = NULL;
  if (prLeft.ast()) {
    expr1 = dynamic_cast<ast::Expr*>(prLeft.ast());
    if (!expr1)
      throw((std::string) "Bad cast of State in parse_relational_expr");
    }
  next_token();
  // just advance token, since examining it in parse_expr caused
  // this method being called.
  /// string boolean operation
  std::string op = prev_token_->lexeme();

  /// expr2
  ParseResult pr_expr2 = parse_expr(prev_token_->lbp());
  ast::Expr *expr2 = NULL;
  if (pr_expr2.ast()) {
    expr2 = dynamic_cast<ast::Expr*>(pr_expr2.ast());
    if (!expr2)
      throw((std::string) "Bad cast of State in parse_relational_expr");
    }

  /// Create binaryExpr node
  ast::Expr *boolOp_ = new ast::binaryExpr(op, expr1, expr2);
  pr.ast(boolOp_);
  return pr;
}

/// Helper function used by the parser.

void Parser::match(const scanner::TokenType &tt) {
  if (!attempt_match(tt)) {
    throw(make_error_msg_expected(tt));
  }
}

bool Parser::attempt_match(const scanner::TokenType &tt) {
  if (curr_token_->terminal() == tt) {
    next_token();
    return true;
  }
  return false;
}

bool Parser::next_is(const scanner::TokenType &tt) {
  return curr_token_->terminal() == tt;
}

void Parser::next_token() {
  if (curr_token_ == NULL) {
    throw(std::string(
        "Internal Error: should not call nextToken in unitialized state"));
  } else if (curr_token_->terminal() == scanner::kEndOfFile &&
           curr_token_->next() == NULL) {
    prev_token_ = curr_token_;
  } else if (curr_token_->terminal() != scanner::kEndOfFile &&
             curr_token_->next() == NULL) {
    throw(make_error_msg("Error: tokens end with endOfFile"));
  } else {
    prev_token_ = curr_token_;
    curr_token_ = curr_token_->next();
  }
}

std::string Parser::terminal_description(const scanner::TokenType &terminal) {
  scanner::Token *dummy_token = new scanner::Token("", terminal, NULL);
  scanner::ExtToken *dummy_ext_token = new scanner::ExtToken(this, dummy_token);
  dummy_ext_token->ExtendToken(this, dummy_token);
  std::string s = dummy_ext_token->description();
  delete dummy_token;
  delete dummy_ext_token;
  return s;
}

std::string Parser::make_error_msg_expected(
    const scanner::TokenType &terminal) {
  std::string s = (std::string) "Expected " + terminal_description(terminal) +
                  " but found " + curr_token_->description();
  return s;
}

std::string Parser::make_error_msg(const scanner::TokenType &terminal) {
  std::string s = "Unexpected symbol " + terminal_description(terminal);
  return s;
}

std::string Parser::make_error_msg(const char *msg) { return msg; }

}  /// namespace parser
}  /// namespace fcal
