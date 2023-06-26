#include "parser.h"
#include <stdexcept>
#include <iostream>

Token get_token()
{
  char ch;
  std::cin >> ch;

  switch (ch) {
  case '(': case ')': case '+': case '-': case '*': case '/':
    return Token(ch);
  case '.':
  case '0': case '1': case '2': case '3': case '4':
  case '5': case '6': case '7': case '8': case '9':
  {
    std::cin.putback(ch);
    double val;
    std::cin >> val;
    return Token('8', val);
  }
  default:
    throw std::runtime_error("Bad token");
  }
}

/**
 *  primary:
 *    number
 *    '(' expression ')'
 */
double primary()
{
  Token t = get_token();
  switch(t.kind) {
  case '(': {
    double d = expression();
    t = get_token();
    if (t.kind == ')') {
      return d;
    } else {
      throw std::runtime_error("')' expected");
    }
  }
  case '8':
    return t.value;
  default:
    throw std::runtime_error("primary expected");
  }
}

/**
 *  term:
 *    primary
 *    term '*' primary
 *    term '/' primary
 *    term '%' primary
 */
double term()
{
    double left = primary();
    Token t = get_token();
    while (true) {
        switch(t.kind) {
        case '*':
            left *= primary();
            t = get_token();
            break;
        case '/': {
            double d = primary();
            if (d == 0) throw std::runtime_error("div by 0");
            left /= d;
            t = get_token();
            break;
        }
        case '%':
            // the remainder operation (%) is not defined for
            // floating-point numbers
            // leave '%' out of our calculator for now
        default:
            return left;
        }
    }
}

/**
 *  expression:
 *    term
 *    expression '+' term
 *    expression '-' term
 */
double expression()
{
  // any Expression starts with a Term and such a Term can be followed by
  // a + or a –.
  //
  //      expression := term[0] ( '+|-' term[1] ( '+|-' term[2] (...)))
  //
  // So, we have to look for a Term, see if it is followed by a + or a –,
  // and keep doing that until there are no more '+'s or '-'s.

  double left = term();
  Token t = get_token();
  while (true) {
    switch (t.kind) {
    case '+':
      left += term();
      t = get_token();
      break;
    case '-':
      left -= term();
      t = get_token();
      break;
    default:
      return left;
    }
  }

  // Note that – except for the loop – this is actually rather similar to
  // our first try.
  // What we have done is to remove the mention of expression() within
  // expression() and replace it with a loop.
  // In other words, we translated the Expression in the grammar rules for
  // Expression into a loop looking for a Term followed by a + or a –.
}

double expression_2nd_try()
{
  // Every Term is an Expression, but not every Expression is a Term;
  // that is, we could start looking for a Term and look for a full
  // Expression only if we found a + or a –.

  double left = term();
  Token t = get_token();
  switch(t.kind) {
  case '+':
    return left + expression();
  case '-':
    return left - expression();
  default:
    return left;
  }

  // How about 1–2–3?
  // This expression() will read the 1 as a Term, then proceed to
  // read 2–3 as an Expression (consisting of the Term 2 followed
  // by the Expression 3). It will then subtract the value of 2–3 from 1.
  // In other words, it will evaluate 1–(2–3).

  // What we did here was to define expression() to first look for
  // a Term and then, if that Term is followed by a + or a –, look for
  // an Expression. This really implements a slightly different grammar:
  //
  //      expression:
  //          term
  //          term '+' expression
  //          term '-' expression
}

double expression_1st_try()
{
  double left = expression();
  Token t = get_token();
  switch(t.kind) {
  case '+':
    return left + term();
  case '-':
    return left - term();
  default:
    return left;
  }

  // this expression() will never get beyond its first line:
  // expression() starts by calling expression() which starts
  // by calling expression() and so on “forever.”
  // This is called an infinite recursion and will in fact terminate
  // after a short while when the computer runs out of memory to hold
  // the “never-ending” sequence of calls of expression().
}
