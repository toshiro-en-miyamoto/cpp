# Chapter 5. Review

## General

### [1] Programming is understanding

What do we mean by "Programming is understanding"?

Writing a program starts with a problem; that is, you have a problem that you'd like a program to help solve. Understanding that problem is key to a good program. After all, a program that solves the wrong problem is likely to be of little use to you, however elegant it is.

When we start, we rarely know the problem well. We often think we do (don’t we all know what a calculator program is?), but we don’t. Only a combination of thinking about the problem (analysis) and experimentation (design and implementation) gives us the solid understanding that we need to write a good program.

### [2] Analysis of the calculator

The chapter details the creation of a calculator program. Write a short analysis of what the calculator should be able to do.

The calculator should be able to "do ordinary arithmetic on expressions we type in". We mean the following operations by *ordinary arithmetic*:

- addition: `+`
- subtraction: `-`
- multiplication: `*`
- division: `/`
- grouping: `(` and `)`
- operator precedence
  1. grouping
  2. multiplication and division
  3. addition and subtraction

### [3] Smaller manageable parts

How do you break a problem up into smaller manageable parts?

- Take advantage of existing libraries, such as `vector` and `string` of the C++ standard library.
- Look for parts of a solution that can be separately described, for example syntax grammars and parsers.

### [4] A small, limited version of a program

Why is creating a small, limited version of a program a good idea?

Build a small, limited version of the program that solves a key part of the problem. When we start, we rarely know the problem well. We often think we do (don’t we all know what a calculator program is?), but we don’t. Only a combination of thinking about the problem (analysis) and experimentation (design and implementation) gives us the solid understanding that we need to write a good program. So first, we build a small, limited version.

- To bring out problems in our understanding, ideas, and tools.
- To see if details of the problem statement need changing to make the problem manageable. It is rare to find that we had anticipated everything when we analyzed the problem and made the initial design. We should take advantage of the feedback that writing code and testing give us.

Sometimes, such a limited initial version aimed at experimentation is called *prototype*. If (as is likely) our first version doesn't work or us so ugly and awkward that we don't want to work with it, we throw it away and make another limited version based on our experience. Repeat until we find a version that we are happy with. Do not proceed with a mess; messes just grow with time.

### [5] Feature creep

Why is feature creep a bad idea?

It is far easier to build a program in stages than all at once.

Feature creep would have made it hard to resist the temptation to add further *neat features* later. How about adding the usual mathematical functions? How about adding loops? Once we start adding neat features it is hard to stop.

### [6] Phases of software development?

What are the three main phases of software development?

- Analysis: Figure out what should be done and write a description of your current understanding of that. Such a description is called a set of requirements or a specification.
- Design: Create an overall structure of the system, deciding which parts the implementation should have and how those parts should communicate. As part of the design consider which tools, such as libraries, can help you structure the program.
- Implementation: Write the code, debug it, and test that it does what it is supposed to do.

### [7] Use case

What is a use case?

Sketching out examples of use (of a program) early on provides a very practical definition of what the program should minimally do. When discussing design and analysis, such examples of use are called *use cases*.

### [8] The purpose of testing

What is the purpose of testing?

We build a small, limited version (of a program) so as to see if details of the problem statement need changing to make the problem manageable. It is rare to find that we had anticipated everything when we analyzed the problem and made the initial design. We should take advantage of the feedback that writing code and testing give us.

## Calculator

### [9] Grammar components

According to the simple arithmetic grammar in this chapter:

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

describe the difference between a `Term`, an `Expression`, a `Number`, and a `Primary`:

- A `Number` is a floating point literal.
- A `Primary` represents a `Number` or an `Expression` surrounded by a pair of parentheses.
- A `Term` represents a `Primary` or the result of a multiplication or division.
- A `Expression` represents a `Term` or the result of an addition or subtraction.

### [10] Manual parsing of an example

In the chapter, an input was broken down into its components: `Term`s, `Expression`s, `Primary`s, and `Number`s.

Firstly, let’s try `45 + 11.5 * 7`:

1. `45` is an `Expression` followed by a `+`, the addition operator. So we look for a `Term` to finish the `Expression + Term` rule.
2. `11.5` is a `Term` followed by `*`, the multiplication operator. So we look for a `Primary` to finish the `Term * Primary` rule. Yes, `11.5 * 7` is a `Term`.
3. Now we can see that `45 + 11.5 * 7` is an `Expression` according to the `Expression + Term` rule.

In particular, it is an `Expression` that first does the multiplication `11.5 * 7` and then the addition `45 + 11.5 * 7`, just as if we had written `45 + (11.5 * 7)`.

Secondly, do this for `(17+4)/(5−1)`.

1. `(` is an opening parenthesis operator. So we look for an `Expression` followed by a closing parenthesis operator to finish `( Expression )` rule.
2. `17` is an `Expression` followed by a `+`, the addition operator. So we look for a `Term` to finish the `Expression + Term` rule.
3. We can see that `17 + 4` is an `Expression` according to the `Expression + Term` rule.
4. The `Expression: 17 + 4 = 21` is followed by a closing parenthesis operator, so `(17 + 4) = 21` is a `Primary: 21` according to the `( Expression )` rule.
5. The `Primary: 21` is followed by a `/`, the division operator. So we look for a `Primary` to finish the `Term / Primary` rule. Yes, `Primary: 21` is a `Term`.
6. `(` is an opening parenthesis operator. So we look for an `Expression` followed by a closing parenthesis operator to finish `( Expression )` rule.
7. `5` is an `Expression` followed by a `-`, the subtraction operator. So we look for a `Term` to finish the `Expression - Term` rule.
8. We can see that `5 - 1` is an `Expression` according to the `Expression + Term` rule.
9. The `Expression: 5 - 1 = 4` is followed by a closing parenthesis operator, so `(5 - 1) = 4` is a `Primary: 4` according to the `( Expression )` rule.
10. We can see that `21 / 4 = 5.25` is an `Term` according to the `Term / Primary`.
11. The input ends. The `Term: 5.25` is an `Expression`.

Lastly, play computer for `(17+4)/(5−1)`.

- the next token; `'('`, an opening parenthesis
- the only applicable rule is `Primary: '(' Expression ')'`
- find an `Expression`
  - the next token; `17`, a `Number`
  - the next token; `'+'`, an addition operator
  - the only applicable rule is `Expression: Expression '+' Term`
  - find a `Term`
    - the next token; `4`, a `Number`
    - `Term: 4` is found
  - with the rule `Expression: Expression '+' Term`
    - the left-hand `Expression` is `Number: 17`
    - the right-hand `Term` is `Number: 4`
    - resulting in `Expression: 17 + 4 = 21`
  - the next token; `)`, a closing parenthesis
  - resulting in `Primary: '(' 21 ')'`, which is `Term: 21`
- the next token; `'/'`, a division operator
- the only applicable rule is `Term: Term '/' Primary`
- find a `Primary`
  - the next token; `'('`, an opening parenthesis
  - the only applicable rule is `Primary: '(' Expression ')'`
  - find an `Expression`
    - the next token; `5`, a `Number`
    - the next token; `'-'`, a subtraction operator
    - the only applicable rule is `Expression: Expression '-' Term`
    - find a `Term`
      - the next token; `1`, a `Number`
      - `Term: 1` is found
    - with the rule `Expression: Expression '-' Term`
      - the left-hand `Expression` is `Number: 5`
      - the right-hand `Term` is `Number: 1`
      - resulting in `Expression: 5 - 1 = 4`
    - the next token; `)`, a closing parenthesis
    - resulting in `Primary: '(' 4 ')'`, which is `Term: 4`
  - the next token; the end of input
  - with the rule `Term: Term '/' Primary`
    - the left-hand `Term` is `Term: 21`
    - the right-hand `Primary` is `Term: 4`
  - resulting in `Term: 21 / 4 = 5.25`, which is `Expression: 5.25`

### [11] Numbers

Why does the program not have a function called `number()`?

- the one reason is because `Number` has the only alternative `floating-point-literal`, and
- another reason is because `std::cin` allows you to read a `floating-point-literal` with one operation `>>`, e.g.
  ```c++
  cin.putback(ch);
  double val = 0;
  cin >> val;
  ```

### [12] Tokens

What is a token?

A token is a sequence of characters that represents something we consider a unit, such as a number or an operator.

### [13] Grammars

What is a grammar? A grammar rule?

You have no problem calculating `1−2*3` and `1+2−3` and `3*2+4/2`. It seems hardwired in your brain. However,

- could you explain how you do it?
- Could you explain it well enough for someone who had never seen conventional arithmetic to grasp?
- Could you do so for every combination of operators and operands? 

To articulate an explanation in sufficient detail and precisely enough for a computer to understand, we need a notation &mdash; and a grammar is a most powerful and conventional tool for that.

A grammar is a list of grammar rules:

- a rule may be followed by other rules
- a rule express alternatives and/or a repeating pattern
- the first rule in a list is where the grammar starts

### [16] Putting back

In the `expression()` function, why is the default for the `switch` statement to “put back” the token?

How do we know that a number is coming? A numeric literal must start with a digit or `.` (the decimal point). So, we test for that. Next, we want to let `cin` read the number, but we have already read the first character (a digit or dot), so just letting `cin` loose on the rest will give a wrong result. (...) Fortunately (and not by accident), `cin` works much like `Token_stream` in that you can put a character back into it. So (...), we just put the initial character back into `cin` and then let `cin` read the whole number.

### [17] Look ahead

What is "look ahead"?

We might consider the convention that “multiplication binds tighter than addition”. So (somehow), we have to “look ahead” on the line to see if there is a `*` (or a `/`). If so, we have to (somehow) adjust the evaluation order from the simple and obvious left-to-right order.

### [18] `putback()`

What does `putback()` do and why is it useful?

```c++
if (full) {
  error();
}
buffer = token;
full = true;
```

When you see `1.5+4` reading strictly left to right, how could you know that the number `1.5` had been completely read without reading the `+`? Until we see the `+` we might be on our way to reading `1.55555`. So, we need a “stream” that produces a token when we ask for one using `get()` and where we can put a token back into the stream using `putback()`.

### [22] Buffer in `Token_stream`

What happens in the `Token_stream` class when there is a token in the buffer and the `get()` function is called?

```c++
if (full) {
  full = false;
  return buffer;
}
```
