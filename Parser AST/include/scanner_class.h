/*******************************************************************************
 * Name            : Scanner.h
 * Project         : scanner
 * Module          : Scanner
 * Description     : Scanner class for extracting tokens from text
 * Creation Date   : 02/24/17
 * Copyright       : Copyright John Carter Rohith Anil, All rights reserved
 *
 ******************************************************************************/

#ifndef INCLUDE_SCANNER_CLASS_H_
#define INCLUDE_SCANNER_CLASS_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/token_class.h"
#include "include/read_input.h"
#include "include/regex.h"

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
namespace fcal {
namespace scanner {

class Token;

/*! This is the Scanner class which implements the scanner of the FCAL program.
 *  Scanner has the method Scan which returns the front of the List of Tokens
 *  generated from scanning a file. It implements the first stage of the parsing
 *  process.
 */
class Scanner {
 public:
  //! Default constructor for Scanner
  Scanner();
  ~Scanner();

  Token *Scan(const char *);
  int consume_whitespace_and_comments(regex_t*, regex_t*,
                                      regex_t*, const char*);

 private:
  /// Token* representing front of the list
  Token *return_token;
  /// Token* representing previous Token in the list
  Token *previous_token;
  /// Token* representing current Token in the list
  Token *current_token;
  /// A regex which matches white space
  regex_t *white_space;
  /// A regex which matches comments
  regex_t *comments;
  /// A regex which matches line comments
  regex_t *line_comment;
  /// String representing contents of the File
  const char *text;
  /// A regex array created for matching lexeme of Tokens
  regex_t *regex_strings[1+static_cast<int>(kNotOp)];
};  // end Scanner
}  // namespace scanner
}  // namespace fcal

#endif  // INCLUDE_SCANNER_CLASS_H_

