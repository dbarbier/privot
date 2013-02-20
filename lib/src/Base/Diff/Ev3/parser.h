/*
** Name:      parser.h
** Author:    Leo Liberti
** Purpose:   C++ class that builds an Expression v3
**            n-ary tree from a string containing a
**            mathematical expression in n variables
** Source:    GNU C++
** History:   010624 derived from project Eval (class/EvalClass.cc)
** License:    (C) Leo Liberti, all rights reserved. Code published under the
Common Public License.
*/

#ifndef __EV3PARSERH__
#define __EV3PARSERH__

#define RCS9 "$Id: parser.h,v 1.7 2006/07/30 05:36:42 liberti Exp liberti $"

// includes
#include <iostream>  // I/O
#if __GNUC__ >= 3
#include <sstream>
#else
#include <strstream>
#endif
#include <string>    // strings
#include <map>       // map
#include <cctype>    // isalpha(), etc.
#include <cmath>

#include "expression.h"

namespace Ev3 {
  // global declarations

  enum Token_value {
    PEV3NAME, PEV3NUMBER, PEV3END, PEV3PLUS = '+', PEV3NLPLUS = '|',
    PEV3MINUS = '-', PEV3MUL = '*', PEV3DIV = '/', PEV3POWER = '^',
    PEV3PRINT = ';', PEV3ASSIGN = '=', PEV3LP = '(', PEV3RP = ')'
  };

  // constants
  const double PEV3infinity = 1.0e300;
#define PEV3PI 3.1415926535897932385
#define PEV3E 2.7182818284590452354
#define PEV3NOVARIABLE -1
#define PEV3UNKNOWNVAR -2

  // the Eval class
  class ExpressionParser
  {

  private:
    // user data
    std::map<std::string, int> variable_;
    std::map<int, std::string> varname_;

    // internal parser/lexer data
    Token_value curr_tok_;
    // contains the map varnames -> varindices
    std::map<std::string, double> table_;
    double number_value_;
    char string_value_[1024];
    int no_of_errors_;
    int no_of_functions_;
    std::istream *input_;
    // if parser hasn't been initialized with SetVariableIDs, false
    bool isinitialized_;

  public:

    // constructors & destructors (public methods)
    ExpressionParser();

    // set variable ID
    void SetVariableID(const std::string & vname,
                       const int vid);

    // get variable ID
    int GetVariableID(const std::string & vname);

    // get variable name
    std::string GetVariableName(const int vid);

    // driver evaluating routine (public method)
    Expression Parse(const char* buf,
                     int& nerrors);

  private:
    // parser: report error (private method)
    double error(const std::string& s);
    double error(const std::string& s,
                 const Token_value tk);

    // parser: primary expressions (private method)
    Expression prim(const bool get);

    // parser: power
    Expression power(const bool get);

    // parser: products and fractions (private method)
    Expression term(const bool get);

    // parser: sums and subtractions (private method)
    Expression expr(const bool get);

    // lexical analyser (private method)
    Token_value get_token();

    // set variable ID (for internal use - doesn't set isinitialized)
    void InternalSetVariableID(const std::string & vname,
                               const int vid);

    // check whether a string is a function name or an unknown variable
    bool IsVariableName(const std::string & vname);

    // progressive vid
    int currentvid_;

  }; // end class ExpressionParser
} /* namespace Ev3 */
#endif /* __EV3PARSERH__ */

