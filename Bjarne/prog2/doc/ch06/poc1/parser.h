#ifndef PARSER
#define PARSER

class Token
{
public:
    char kind;
    double value;
    Token(char k) : kind{k}, value{0.0} {}
    Token(char k, double v) : kind{k}, value{v} {}
};

Token get_token();
double expression();
double term();
double primary();

#endif
