# Lambda Expressions

Lambda expressions —*lambdas*— are such a convenient way to create function objects.

- A *lambda expression* is just that: an expression. It’s part of the source code.
- A *closure* is the runtime object created by a lambda. Depending on the capture mode, closures hold copies of or references to the captured data.
- A *closure class* is a class from which a closure is instantiated. Each lambda causes compilers to generate a unique closure class.

Closures may generally be copied, so it’s usually possible to have multiple closures of a closure type corresponding to a single lambda.

```c++
{
    int x;
    // do something

    // a copy of the closure produced by the lambda
    auto c1 = [x](int y) { return x * y > 55; };

    auto c2 = c1;       // a copy of c1
    auto c3 = c2;       // a copy of c2
}
```

- Item 31: Avoid default capture modes.
