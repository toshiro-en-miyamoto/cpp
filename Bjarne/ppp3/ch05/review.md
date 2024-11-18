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

### [8] The purpose of testing

What is the purpose of testing?

## Calculator

### [9] Grammar components

According to the outline in the chapter, describe the difference between a `Term`, an `Expression`, a `Number`, and a `Primary`.

### [10] Manual parsing of an example

In the chapter, an input was broken down into its components: `Term`s, `Expression`s, `Primary`s, and `Number`s. Do this for `(17+4)/(5−1)`.

### [11] Numbers

Why does the program not have a function called `number()`?

### [12] Tokens

What is a token?

### [13] Grammars

What is a grammar? A grammar rule?

### [16] Putting back

In the `expression()` function, why is the default for the `switch` statement to “put back” the token?

### [17] Look ahead

What is "look ahead"?

### [18] `putback()`

What does `putback()` do and why is it useful?

### [22] Buffer in `Token_stream`

What happens in the `Token_stream` class when there is a token in the buffer and the `get()` function is called?

