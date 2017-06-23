/*******************************************************************************
 * Name            : Token.cc
 * Project         : Scanner
 * Module          : Token
 * Description     : token class for setting token data
 * Creation Date   : 02/24/17
 * Copyright       : Copyright 2017 John Carter Rohith Anil, All rights reserved
 *
 ******************************************************************************/

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string>
#include "include/token_class.h"
#include "include/scanner.h"

namespace fcal {
namespace scanner {

/*******************************************************************************
* Constructors/Destructor
******************************************************************************/
Token::Token() {}

/*! \param a string representing the lexeme of the Token
    \param b TokenType of the Token
    \param c Token* which holds address of the next Token in the Token list 
             returned by Scan method of Scanner
 */
Token::Token(std::string a, const TokenType b, Token *c) {
  lexeme_ = a;
  terminal_ = b;
  next_ = c;
}

Token::~Token() {}


//! Set the values of the current token
void Token::set_token(TokenType t, std::string lex) {
  terminal_ = t;
  lexeme_ = lex;
  next_ = 0;
}  /// end set_token


//! Set the value of the previous token to point to the current token
void Token::set_next(Token *current) {
  next_ = current;
}  /// end set_next

//! Get the value of terminal
TokenType Token::terminal() {
  return terminal_;
}

//! Get the value of lexeme
std::string Token::lexeme() {
  return lexeme_;
}

//! Get the value of next
Token* Token::next() {
  return next_;
}

}  // namespace scanner
}  // namespace fcal
