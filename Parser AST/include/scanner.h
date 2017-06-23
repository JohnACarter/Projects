/*******************************************************************************
 * Name            : scanner.h
 * Project         : fcal
 * Module          : scanner
 * Description     : Header file for scanner module
 * Copyright       : 2017 CSCI3081W Staff. All rights reserved.
 * Original Author : Eric Van Wyk
 * Modifications by: John Harwell
 ******************************************************************************/

#ifndef INCLUDE_SCANNER_H_
#define INCLUDE_SCANNER_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <regex.h>
#include <string>

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
namespace fcal {
namespace scanner {

/* below are the possible terminal types for each token */

enum kTokenEnumType {
  kIntKwd,
  kFloatKwd,
  kBoolKwd,
  kTrueKwd,
  kFalseKwd,
  kStringKwd,
  kMatrixKwd,
  kLetKwd,
  kInKwd,
  kEndKwd,
  kIfKwd,
  kThenKwd,
  kElseKwd,
  kRepeatKwd,
  kWhileKwd,
  kPrintKwd,
  kToKwd,

  // Constants
  kIntConst,
  kFloatConst,
  kStringConst,

  // Names
  kVariableName,

  // Punctuation
  kLeftParen,
  kRightParen,
  kLeftCurly,
  kRightCurly,
  kLeftSquare,
  kRightSquare,
  kSemiColon,
  kColon,

  // Operators
  kAssign,
  kPlusSign,
  kStar,
  kDash,
  kForwardSlash,
  kLessThan,
  kLessThanEqual,
  kGreaterThan,
  kGreaterThanEqual,
  kEqualsEquals,
  kNotEquals,
  kAndOp,
  kOrOp,
  kNotOp,

  // Special terminal types
  kEndOfFile,
  kLexicalError
};
typedef enum kTokenEnumType TokenType;

/*******************************************************************************
 * Class Definitions
 * 
 * You will need to define a Token class (remember, a pointer of type Token *
 * is returned by the scan method of the scanner class.) 
 * Also, Classes are typically declared (and sometimes also defined) in
 * their own header files
 * 
 * You will also need to define a Scanner class with a scan and other methods
 ******************************************************************************/

} /* namespace scanner */
} /* namespace fcal */

#endif  // INCLUDE_SCANNER_H_
