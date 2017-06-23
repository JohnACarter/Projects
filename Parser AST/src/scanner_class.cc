/*******************************************************************************
 * Name            : Scanner.cc
 * Project         : Scanner
 * Module          : Scanner
 * Description     : A scanner for extracting tokens
 * Creation Date   : 02/24/17
 * Copyright       : Copyright 2017 John Carter Rohith Anil, All rights reserved
 *
 ******************************************************************************/

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "include/scanner.h"
#include "include/scanner_class.h"
#include "include/token_class.h"
#include "include/regex.h"
#include "include/read_input.h"

class Token;

namespace fcal {
namespace scanner {

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/

/*! Initialize token_strings, white_space, comments, line_comment 
 *  and regex_strings
 */
Scanner::Scanner() {
  const char *token_strings[] = { "^int", "^float", "^boolean", "^True",
                                "^False", "^string", "^matrix",
                                "^let", "^in", "^end", "^if", "^then",
                                "^else", "^repeat", "^while", "^print",
                                "^to", "^[0-9]+", "^-?[0-9]*\\.[0-9]+",
                                "^\"(\\\\.|[^\"])*\"",
                                "^([a-zA-Z_][a-zA-Z_0-9]*)",
                                "^\\(", "^\\)",
                                "^\\{", "^\\}", "^\\[", "^\\]", "^;",
                                "^:", "^=", "^\\+", "^\\*", "^\\-", "^\\/",
                                "^<", "^<=", "^>", "^>=", "^==",
                                "^!=", "^(&&)", "^\\|\\|", "^!"};

  white_space = make_regex("^[\n\t\r ]+");

  comments = make_regex("^/\\*([^\\*]|\\*+[^\\*/])*\\*+/");

  line_comment = make_regex("^//[^\n\r]*[\n\r]");

  for (int i = 0; i <= kNotOp; i++) {
  regex_strings[i] = make_regex(token_strings[i]);
  }
}

Scanner::~Scanner() {}

/*! Scans the input string and return's the front of the list of Token's.
 *  \param s input string representing the contents of a file to be scanned
 */
Token *Scanner::Scan(const char *s) {
  text = s;

  //! Token pointers to create list of tokens
  Token *previous = new Token;
  Token *return_token = previous;

  //! consume white_space and comments
  int num_matched_chars = 0;
  num_matched_chars =
     consume_whitespace_and_comments(white_space, comments, line_comment, text);
  text = text + num_matched_chars;
  int max_num_matched_chars = 0;

  //! Check the text for qualified tokens or lexical errors
  while (text[0] != '\0') {
    int index = 0;
    max_num_matched_chars = 0;
    //! current Token is where we store token information
    Token *current = new Token;
    TokenType terminal;

    /*! Check if next text string matches a token, if so add
     * terminal and lexeme null pointer. If a token isn't found
     * add lexeme and lexical error.
     */
    while (index <= kNotOp) {
      num_matched_chars = match_regex(regex_strings[index], text);
      if (num_matched_chars > max_num_matched_chars) {
        max_num_matched_chars = num_matched_chars;
        terminal = static_cast<TokenType>(index);
      }
      index++;
    }
    if (max_num_matched_chars == 0 && text[0] != '\0') {
        //! If no token match, set lexical error
        terminal = kLexicalError;
        max_num_matched_chars = 1;
    }

    //! Find the current lexeme using the number of matched characters
    std::string lexeme(text, max_num_matched_chars);

    //! Set the current token values
    current->set_token(terminal, lexeme);

    //! Set previous token to point to current
    previous->set_next(current);
    previous = current;
    text = text + max_num_matched_chars;

    num_matched_chars =
       consume_whitespace_and_comments(white_space, comments,
         line_comment, text);
    text = text + num_matched_chars;
  }

  //! Add end of file token once all matches are found
  TokenType terminal = kEndOfFile;
  std::string lexeme("");

  Token *last_token = new Token;
  last_token->set_token(terminal, lexeme);
  previous->set_next(last_token);

  return return_token->next();
}  /// end scan

/*! Consumes white spaces and comments
    \param white_space regex for matching white space
    \param block_comment regex for matching block comment
    \param line_comment regex for matching line comment
    \param text A string representing File input
 */
int Scanner::consume_whitespace_and_comments(regex_t *white_space,
                                    regex_t *block_comment,
                                    regex_t *line_comment,
                                    const char *text) {
  int num_matched_chars = 0;
  int total_num_matched_chars = 0;
  int still_consuming_white_space;

  do {
    still_consuming_white_space = 0;  /// exit loop if not reset by a match

    //! Try to match white space
    num_matched_chars = match_regex(white_space, text);
    total_num_matched_chars += num_matched_chars;
    if (num_matched_chars > 0) {
      text = text + num_matched_chars;
      still_consuming_white_space = 1;
    }

    //! Try to match block comments
    num_matched_chars = match_regex(block_comment, text);
    total_num_matched_chars += num_matched_chars;
    if (num_matched_chars > 0) {
      text = text + num_matched_chars;
      still_consuming_white_space = 1;
    }

    //! Try to match line comment comments
    num_matched_chars = match_regex(line_comment, text);
    total_num_matched_chars += num_matched_chars;
    if (num_matched_chars > 0) {
      text = text + num_matched_chars;
      still_consuming_white_space = 1;
    }
  } while (still_consuming_white_space);

  return total_num_matched_chars;
} /** consume_whitespace_and_comments() */
}  // namespace scanner
}  // namespace fcal
