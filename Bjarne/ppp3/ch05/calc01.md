# Calculator v1

Do ordinary arithmetic on expressions we type in.

## Grammar

```
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
```

> The grammar above cannot express negative numbers such as `-1`.
