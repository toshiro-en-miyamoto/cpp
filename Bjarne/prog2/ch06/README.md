# Writing a Program

## Grammars

```
expression:
    term
    expression + term
    expression - term
term:
    primary
    term + primary
    term / primary
    term % primary
primary:
    number
    ( expression )
number:
    floating-point-literal
```

Reading a stream of tokens according to a grammar is called *parsing*, and a program that does that is often called a *parser* or a *syntax analyzer*.

Is `2` an expression?

- Going down:
  1. An `expression` must be a `term` or end with a `term`.
  2. That `term` must be a `primary` or end with a `primary`.
  3. That `primary` must start with a `'('` or be a `number`.
  4. Obviously, `2` is not a `'('`, but a `floating-point-literal`, which is a `number`, which is a `primary`.
- Going up:
  1. That `primary` (the `number 2`) isn’t preceded by a `'/'`, `'*'`, or `'%'`, so it is a complete `term` (rather than the end of a `'/'`, `'*'`, or `'%'` expression).
  2. That `term` (the `primary 2`) isn’t preceded by a `'+'` or `'-'`, so it is a complete `expression` (rather than the end of a `'+'` or `'%'` expression).

So yes, according to our grammar, `2` is an expression.

Is `2+3` an expression?

- Going down:
  1. An `expression` must be a `term` or end with a `term`.
  2. That `term` must be a `primary` or end with a `primary`.
  3. That `primary` must start with a `'('` or be a `number`.
  4. Obviously, `2` is not a `'('`, but a `floating-point-literal`, which is a `number`, which is a `primary`.
- Going up:
  1. That `primary` (the `number 2`) isn’t preceded by a `'/'`, `'*'`, or `'%'`, so it is a complete `term` (rather than the end of a `'/'`, `'*'`, or `'%'` expression).
  2. That `term` (the `primary 2`) is followed by a `'+'`, so it is the end of the first part of an `expression` and we must look for the `term` after the `'+'`.
- Going down:
  1. In exactly the same was as we found that `2` was a `term`, we found that `3` is a `term`.
- Going up:
  1. Since `3` is not followed by a `'+'` or a `'-'`, it is a complete `term` (rather than the first part of a `'+'` or `'-'` expression).
  2. Therefore, `2+3` matches the `expression + term` rule.

Thus, `2+3` is an expression.

Is `45+11.5*7` an expression?

1. `45` is an `expression` followed by a `'+'`, so we look for a `term` to finish the `expression + term` rule.
2. `11.5` is a `term` followed by `'*'`, so we look for a `primary` to sinish the `term * primary` rule.
3. `7` is a `primary`, so `11.5*7` is a `term` according to the `term * primary` rule.
4. Now we can see that `45+11.5*7` is an `expression` according to the `expression + term` rule.

In particular, it is an `expression` that
- first does the multiplication `11.5*7` and then
- the addition `45+11.5*7`,
- just as if we had written `45+(11.5*7)`.


