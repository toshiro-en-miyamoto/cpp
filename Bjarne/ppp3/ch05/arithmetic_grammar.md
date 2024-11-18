# Arithmetic Grammar

A simple arithmetic grammar is:

```
[E] Expression:
[E1]  Term
[E2]  Expression "+" Term     // addition
[E3]  Expression "-" Term     // subtraction

[T] Term:
[T1]  Primary
[T2]  Term "*" Primary        // multiplication
[T3]  Term "/" Primary        // division
[T4]  Term "%" Primary        // reminder (modulo)

[P] Primary:
[P1]  Number
[P2]  "(" Expression ")"

[N] Number:
[N1]  Floating_point

[F] Floating_point:
[F1]  Integer
[F2]  Integer "." Digit*

[I] Integer:
[I1]  Digit+                  // non negative integer
[I2]  "-" Digit+              // negative integer

[D] Digit
[D1]  "0" | "1" | "2" | ... | "9"
```

## Examples:

Is `"2"` an `Expression`?

- the first token is `"2"`
- no more token
- the end of input triggers the process of
  - finding applicable rules, and
  - checking if the input is an `Expression`
- `"2"` matches `[I1] Integer : Digit+` 
  - which matches `[F1] Floating_point : Integer`
  - which matches `[N1] Number : Floating_point`
  - which matches `[P1] Primary : Number`
  - which matches `[T1] Term : Primary`
  - which matches `[E1] Expression : Term`
- thus, `"2"` has been found to be an `Expression`
- the expression evaluation produces a floating-point `2`

Is `"-2"` an `Expression`?

- the first token is `"-"`
- the next token is `"2"`
- no more token
- the end of input triggers the process of
  - finding applicable rules, and
  - checking if the input is an `Expression`
- `"-" "2"` matches `[I2] Integer : "-" Digit+`
- the same reasoning as `"2"` goes
- thus, `"-2"` has been found to be an `Expression`
- the expression evaluation produces a floating-point `-2`

Is `"-24+3"` an `Expression`?

- the first token is `"-"`
- the next token is `"24"`
- the next token is `"+"`

Is `45+11.5*7` an `Expression`?

- the first token, `45`, is a `Floating_point` [F1]
- the next token is `"+"`
- the only rule applicable is `Expression "+" Term` [E2]
- the question: is `45` valid as the left-hand `Expression`?
  - the token, `45`, is a `Number` [N1]
  - which is a `Primary` [P1]
  - which is a `Term` [T1]
  - which is an `Expression` [E1]
  - the expression evaluation produces a floating-point `45`
- the question: is the next entity a `Term`?
  - the next token, `11.5`, is a `Floating_point` [F1]
  - the next token is `"*"`
  - the only rule applicable is `Term "*" Primary` [T2]
  - the question: is `11.5` valid as the left-hand `Term`?
    - the token, `11.5`, is a `Number` [N1]
    - which is a `Primary` [P1]
    - which is a `Term` [T1]
  - the question: is the next entity a `Primary`?
    - the next token, `7`, is a `Floating_point` [F1]
    - no more token
    - applicable rules are
      - the token, `7`, is a `Number` [N1]
      - which is a `Primary` [P1]
  - thus, `11.5*7` is a `Term` by the rule `Term "*" Primary` [T2]
  - evaluation of the `Term` produces a floating-point `80.5`, note that a `Term` is an `Expression` [E1]
- thus, `45+11.5*7` is an `Expression`
- evaluation of the expression `45+80.5` produces a floating-point `125.5`

Is `1+2*(3+4)` and `Expression`?

- the first token, `1`, is a `Floating_point` [F1]
- the next token is `"+"`
- the only rule applicable is `Expression "+" Term` [E2]
- the question: is `1` valid as the left-hand `Expression`?
  - the token, `1`, is a `Number` [N1]
  - which is a `Primary` [P1]
  - which is a `Term` [T1]
  - which is an `Expression` [E1]
  - the expression evaluation produces a floating-point `1`
- the question: is the next entity a `Term`?
  - the next token, `2`, is a `Floating_point` [F1]
  - the next token is `"*"`
  - the only rule applicable is `Term "*" Primary` [T2]
  - the question: is `2` valid as the left-hand `Term`?
    - the token, `2`, is a `Number` [N1]
    - which is a `Primary` [P1]
    - which is a `Term` [T1]
  - the question: is the next entity a `Primary`?
    - the next token `"("`
    - the only rule applicable is `"(" Expression ")"` [P2]
    - the question: is the next entity an `Expression`?
      - the next token, `3`, is a `Floating_point` [F1]
      - the next token is `"+"`
      - the only rule applicable is `Expression "+" Term` [E2]
      - the question: is `3` valid as the left-hand `Expression`?
        - the token, `3`, is a `Number` [N1]
        - which is a `Primary` [P1]
        - which is a `Term` [T1]
        - which is an `Expression` [E1]
        - the expression evaluation produces a floating-point `3`
      - the question: is the next entity is a `Term`?
        - the next token, `4`, is a `Floating_point`
        - the next token, `")"`
    - the `")"` triggers the process of checking 
