/*******************************************************************************
 * Name            : Token.h
 * Project         : scanner
 * Module          : Token
 * Description     : Token class to store token values
 * Creation Date   : 02/24/17
 * Copyright       : Copyright John Carter Rohith Anil, All rights reserved
 *
 ******************************************************************************/

#ifndef INCLUDE_TOKEN_CLASS_H_
#define INCLUDE_TOKEN_CLASS_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string>
#include "include/scanner.h"

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
namespace fcal {
namespace scanner {

/*! This class holds information about a Token.
 */
class Token {
 private:
  /// Type of the Token
  TokenType terminal_;
  /// lexeme of the Token
  std::string lexeme_;
  /// Next Token in the Token Array returned by Scan of Scanner
  Token *next_;

 public:
  //! Default constructor for Token
  Token();

  //! Constructor which initializes all attributes of Token
  Token(std::string, const TokenType, Token*);

  // ! Default destructor for Token
  ~Token();
  void set_token(TokenType t, std::string lex);
  void set_next(Token *current);
  TokenType terminal();
  std::string lexeme();
  Token* next();
};  // end Token
}  // namespace scanner
}  // namespace fcal
#endif  // INCLUDE_TOKEN_CLASS_H_
