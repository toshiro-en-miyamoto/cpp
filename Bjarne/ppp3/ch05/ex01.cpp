// g++ -std=c++23 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wconversion -Wno-psabi ex01.cpp
/**
 * \brief Do ordinary arithmetic on expressions we type in.
 * 
 * Grammar:
 * 
 *  [E] Expression:
 *  [E1]  Term
 *  [E2]  Expression "+" Term     // addition
 *  [E3]  Expression "-" Term     // subtraction
 *  
 *  [T] Term:
 *  [T1]  Primary
 *  [T2]  Term "*" Primary        // multiplication
 *  [T3]  Term "/" Primary        // division
 *  [T4]  Term "%" Primary        // reminder (modulo)
 *  
 *  [P] Primary:
 *  [P1]  Number
 *  [P2]  "(" Expression ")"
 *  [P3]  "{" Expression "}"
 *  
 *  [N] Number:
 *  [N1]  Floating_point
 *  
 *  [F] Floating_point:
 *  [F1]  Integer
 *  [F2]  Integer "." Digit*
 *  
 *  [I] Integer:
 *  [I1]  Digit+                  // non negative integer
 *  [I2]  "+" Integer             // positive integer
 *  [I3]  "-" Integer             // negative integer
 *  
 *  [D] Digit
 *  [D1]  "0" | "1" | "2" | ... | "9"
 * 
 */


struct Token
{
  char kind;
  double value;
  Token(char k = '8', double v = 0.0) : kind{k}, value{v} {}
};

class Token_stream
{
public:
  void putback(Token t);
  Token get();
private:
  bool full = false;
  Token buffer;
};

#include <stdexcept>
#include <iostream>

void Token_stream::putback(Token t)
{
  if (full) {
    throw std::runtime_error("putback() into a full buffer");
  }
  buffer = t;
  full = true;
}

Token Token_stream::get()
{
  if (full) {
    full = false;
    return buffer;
  }
  char ch = 0;
  std::cin >> ch;
  if (!std::cin) {
    throw std::runtime_error("no input");
  }
  switch (ch) {
    case ';':
    case 'q':
    case '(': case ')': case '+': case '-': case '*': case '/':
      return Token(ch);
    case '.':
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
    {
      std::cin.putback(ch);
      double val = 0.0;
      std::cin >> val;
      return Token{'8', val};
    }
    default:
      throw std::runtime_error("Bad token");
  }
}

Token_stream ts;

double expression();

double primary()
{
  Token t = ts.get();
  switch (t.kind) {
    case '{':
    {
      double d = expression();
      t = ts.get();
      if (t.kind != '}') {
        throw std::runtime_error("'}' expected");
      }
      return d;
    }
    case '(':
    {
      double d = expression();
      t = ts.get();
      if (t.kind != ')') {
        throw std::runtime_error("')' expected");
      }
      return d;
    }
    case '8':
      return t.value;
    default:
      throw std::runtime_error("primary expected");
  }
}

double term()
{
  double left = primary();
  Token t = ts.get();
  while (true) {
    switch(t.kind) {
      case '*':
        left *= primary();
        t = ts.get();
        break;
      case '/':
      {
        double d = primary();
        if (d == 0.0) {
          throw std::runtime_error("divide by zero");
        }
        left /= d;
        t = ts.get();
        break;
      }
      default:
        ts.putback(t);
        return left;
    }
  }
}

double expression()
{
  double left = term();
  Token t = ts.get();
  while (true) {
    switch (t.kind) {
      case '+':
        left += term();
        t = ts.get();
        break;
      case '-':
        left -= term();
        t = ts.get();
        break;
      default:
        ts.putback(t);
        return left;
    }
  }
}

int main()
try {
  double val = 0.0;
  while (std::cin) {
    Token t = ts.get();
    if (t.kind == 'q') break;
    if (t.kind == ';') {
      std::cout << "=" << val << '\n';
    } else {
      ts.putback(t);
      val = expression();
    }
  }
  return 0;
} catch (const std::runtime_error& e) {
  std::cerr << e.what() << '\n';
  return 1;
} catch (...) {
  std::cerr << "unkown exception\n";
  return 2;
}
