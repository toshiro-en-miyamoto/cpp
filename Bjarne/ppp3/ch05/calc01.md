# Calculator v1

Do ordinary arithmetic on expressions we type in.

## Grammar


Expression:
  Term
  Expression "+" Term     // addition
  Expression "-" Term     // subtraction
Term:
  Primary
  Term "*" Primary        // multiplication
  Term "/" Primary        // division
Primary:
  Number
  "(" Expression ")"
Number:
  Non-negative-fractional-number
Non-negative-fractional-number:
  Whole-number
  "." Digit-sequence
  Whole-number "." Digit-sequence
Whole-number:
  "0"
  Natural-number
Natural-number:
  Nonzero-digit
  Natural-number Digit-sequence
Digit-sequence:
  Digit
  Digit-sequence Digit
Digit:
  "0" | Nonzero-digit
Nonzero-digit:
  "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"


> The grammar above cannot express negative numbers such as `-1`.

## Test cases &mdash; A

| TC#     | rule                      | test case   | expr  |
|---------|---------------------------|-------------|------:|
| A-01-01 | Primary: Number           | 2.0         |   2.0 |
| A-01-02 | Primary: "(" Number ")"   | (3)         |   3.0 |
| A-02-01 | Term: Primary "*" Primary | 2.0*3       |   6.0 |
| A-02-02 | Term: Term "*" Primary    | 2*3*4       |  24.0 |
| A-02-03 | Term: Term "*" Primary    | 3/2*4       |   6.0 |
| A-03-01 | Term: Primary "/" Primary | 3/2.0       |   1.5 |
| A-03-02 | Term: Term "/" Primary    | 2*3/4       |   1.5 |
| A-03-03 | Term: Term "/" Primary    | 12/2/4      |   1.5 |
| A-04-01 | Primary: "(" Term ")"     | (2*3)       |   6.0 |
| A-04-02 | Primary: "(" Term ")"     | (3/2)       |   1.5 |
| A-05-01 | Expr: Primary "+" Primary | 2+3         |   5.0 |
| A-05-02 | Expr: Primary "+" Term    | 2+3/2       |   3.5 |
| A-05-03 | Expr: Primary "+" Term    | 2+2*3*4     |  26.0 |
| A-05-04 | Expr: Primary "+" Term    | 2+3/2*4     |   8.0 |
| A-05-05 | Expr: Term "+" Primary    | 4/2+3       |   5.0 |
| A-05-06 | Expr: Term "+" Primary    | 1*2*3+4     |  10.0 |
| A-05-07 | Expr: Term "+" Primary    | 3/2*4+1     |   7.0 |
| A-05-08 | Expr: Term "+" Term       | 3/2+2*3     |   7.5 |
| A-06-01 | Expr: Primary "-" Primary | 2-3         |  -1.0 |
| A-06-02 | Expr: Primary "-" Term    | 2-3/2       |   1.5 |
| A-06-03 | Expr: Primary "-" Term    | 2-2*3*4     | -22.0 |
| A-06-04 | Expr: Primary "-" Term    | 2-3/2*4     |  -4.0 |
| A-06-05 | Expr: Term "-" Primary    | 4/2-3       |  -1.0 |
| A-06-06 | Expr: Term "-" Primary    | 1*2*3-4     |   2.0 |
| A-06-07 | Expr: Term "-" Primary    | 3/2*4-1     |   5.0 |
| A-06-08 | Expr: Term "-" Term       | 3/2-2*3     |  -4.5 |
| A-07-01 | Primary: "(" Expr ")"     | (2+3)       |   5.0 |
| A-07-02 | Primary: "(" Expr ")"     | (2-3)       |  -1.0 |
| A-08-01 | Term: Primary "*" Primary | (2+3)*(2-3) |  -5.0 |
| A-08-02 | Term: Term "*" Primary    | 2*3*(2-3)   |  -6.0 |
| A-08-03 | Term: Term "*" Primary    | 3/2*(2-4)   |  -3.0 |
| A-09-01 | Term: Primary "/" Primary | (2+3)/(2-4) |  -2.5 |
| A-09-02 | Term: Term "/" Primary    | 2*3/(2-4)   |  -3.0 |
| A-09-03 | Term: Term "/" Primary    | 2*3/(2-2)   | DIV-0 |
| A-10-01 | Expr: Primary "+" Primary | (2+3)+(2-3) |   4.0 |
| A-10-02 | Expr: Primary "+" Term    | (2-3)+3/2   |   0.5 |
| A-10-03 | Expr: Primary "+" Term    | (2+3)+2*3*4 |  29.0 |
| A-10-04 | Expr: Primary "+" Term    | (2-3)+3/2*4 |   5.0 |
| A-10-05 | Expr: Term "+" Primary    | 4/2+(2+3)   |   7.0 |
| A-11-01 | Expr: Primary "-" Primary | (2+3)-(2-3) |   6.0 |
| A-11-02 | Expr: Primary "-" Term    | (2+3)-3/2   |   3.5 |
| A-11-03 | Expr: Primary "-" Term    | (2-3)-2*3*4 | -25.0 |
| A-11-04 | Expr: Primary "-" Term    | (2+3)-3/2*4 |  -1.0 |
| A-11-05 | Expr: Term "-" Primary    | 4/2-(2-3)   |   3.0 |

## Test cases &mdash; B

| TC#     | test case | expr  |
|---------|-----------|------:|
| B-01-01 | 3         |   3.0 |
| B-01-02 | (3)       |   3.0 |
| B-02-01 | 3+1       |   4.0 |
| B-02-02 | 3-2       |   1.0 |
| B-02-03 | 3*3       |   9.0 |
| B-02-04 | 3/2       |   1.5 |
| B-03-01 | 3+1+1     |   5.0 |
| B-03-02 | 3+1-2     |   2.0 |
| B-03-03 | 3+1*3     |   6.0 |
| B-03-04 | 3+1/2     |   3.5 |
| B-04-01 | 3-2+1     |   2.0 |
| B-04-02 | 3-2-2     |  -1.0 |
| B-04-03 | 3-2*3     |  -3.0 |
| B-04-04 | 3-2/2     |   2.0 |
| B-05-01 | 3*3+1     |  10.0 |
| B-05-02 | 3*3-2     |   7.0 |
| B-05-03 | 3*3*3     |  27.0 |
| B-05-04 | 3*3/2     |   4.5 |
| B-06-01 | 3/2+1     |   2.5 |
| B-06-02 | 3/2-2     |  -0.5 |
| B-06-03 | 3/2*3     |   4.5 |
| B-06-04 | 3/2/1     |   1.5 |
| B-07-01 | (3+1)     |   4.0 |
| B-07-02 | (3-2)     |   1.0 |
| B-07-03 | (3*3)     |   9.0 |
| B-07-04 | (3/2)     |   1.5 |
| B-08-01 | (3+1)+1   |   5.0 |
| B-08-02 | (3+1)-2   |   2.0 |
| B-08-03 | (3+1)*3   |  12.0 |
| B-08-04 | (3+1)/2   |   2.0 |
| B-08-05 | 3+(1+1)   |   5.0 |
| B-08-06 | 3+(1-2)   |   2.0 |
| B-08-07 | 3+(1*3)   |   6.0 |
| B-08-08 | 3+(1/2)   |   3.5 |
| B-09-01 | (3-2)+1   |   2.0 |
| B-09-02 | (3-2)-2   |  -3.0 |
| B-09-03 | (3-2)*3   |   3.0 |
| B-09-04 | (3-2)/2   |   1.5 |
| B-09-05 | 3-(2+1)   |   0.0 |
| B-09-06 | 3-(2-2)   |   3.0 |
| B-09-07 | 3-(2*3)   |  -3.0 |
| B-09-08 | 3-(2/2)   |   2.0 |
| B-10-01 | (3*3)+1   |  10.0 |
| B-10-02 | (3*3)-2   |   7.0 |
| B-10-03 | (3*3)*3   |  27.0 |
| B-10-04 | (3*3)/2   |   4.5 |
| B-10-05 | 3*(3+1)   |  12.0 |
| B-10-06 | 3*(3-2)   |   3.0 |
| B-10-07 | 3*(3*3)   |  27.0 |
| B-10-08 | 3*(3/2)   |   4.5 |
| B-11-01 | (3/2)+1   |   2.5 |
| B-11-02 | (3/2)-2   |  -0.5 |
| B-11-03 | (3/2)*3   |   4.5 |
| B-11-04 | (3/2)/1   |   1.5 |
| B-11-05 | 3/(2+1)   |   1.0 |
| B-11-06 | 3/(2-2)   | DIV-0 |
| B-11-07 | 3/(2*3)   |   0.5 |
| B-11-08 | 3/(2/1)   |   1.5 |
