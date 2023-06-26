# Errors

When we write programs, errors are natural and unavoidable; the question is: How do we deal with them? Our guess is that avoiding, finding, and correcting errors takes 90% or more of the effort when developing serious software.

Basically, we offer three approaches to producing acceptable software:

- Organize software to minimize errors.
- Eliminate most of the errors we made through debugging and testing.
- Make sure the remaining errors are not serious.

## Sources of errors

Here are some sources of errors:

- Poor specification
- Incomplete programs
- Unexpected arguments
- Unexpected input
- Unexpected state
- Logical errors

## Run-time errors

```c++
int area(int length, int width)
{
  return length * width;
}

int framed_area(int x, int y)
{
  return area(x-2, y-2);
}

int main()
{
  int y = 2;
  int z = 4;
  // ...
  int area2 = framed_area(1, z);  // area(-1, 4) -> -4
  int area3 = framed_area(y, z);  // area( 0, 4) ->  0
  double(area2)/area3;            // divide by zero
}
```

Let the caller of `area()` deal with bad arguments.

```c++
int main()
{
  constexpr int frame_width = 2;

  if (1 - frame_width <= 0 || z - frame_width <= 0) {
    error("non-positive argument for area()");
  }
  int area2 = framed_area(1, z);

  if (y - frame_width <= 0 || z - frame_width <= 0) {
    error("non-positive argument for area()")
  }
  int area3 = framed_area(y, z);
}
```

Look at that code! Are you sure it is correct? Do you find it pretty? Is it easy to read? Actually, we find it ugly (and therefore error-prone). We have more than trebled the size of the code and exposed an implementation detail of `framed_area()`.

Let `area() (the called function) deal with bad arguments.

```c++
int area(int length, int width)
{
  return length * width;
}

int framed_area(int x, int y)
{
  constexpr int frame_width = 2;

  if (x - frame_width <= 0 || y - frame_width <= 0) {
    error("non-positive argument for area()")
  }
  return area(x - frame_width, y - frame_width);
}
```

Checking arguments in the function seems so simple, so why don’t people do that always? Inattention to error handling is one answer, sloppiness is another, but there are also respectable reasons:

- We can't modify the function definition.
- The called function doesn't know what to do in case of error.
- The called function doesn't know where it was called from.
- Performance

Check your arguments in a function unless you have a good reason not to.

## Exceptions

```c++
void error(std::string s)
{
  throw std::runtime_exception(s);
}

int main()
try {
  int y = 2;
  int z = 4;
  // ...
  int area2 = framed_area(1, z);  // area(-1, 4) -> -4
  int area3 = framed_area(y, z);  // area( 0, 4) ->  0
  double(area2)/area3;            // divide by zero
  return 0;
}
catch (std::runtime_exception& e) {
  std::cerr << "runtime error: " << e.what() << '\n';
  return 1;
}
catch (...) {
  std::cerr << "Unknown exception\n";
}
```

## Pre- and post-conditions

### Pre-conditions

```c++
int complicated_function(int a, int b, int c)
// the arguments are positive and a < b < c
{
  if (!(0 < a && a < b && b < c))
    error("bad arguments");
}
```

First, we stated (in a comment) what the function required of its arguments, and then we checked that this *requirement* held (throwing an exception if it did not). A requirement of a function upon its argument is often called a *pre-condition*: it must be true for the function to perform its action correctly.

The reasons most often given for not checking pre-conditions are:

- *Nobody would give bad arguments*: reasonable only when we happen to know “who” calls a function — and in real-world code that can be very hard to know.
- *It would slow down my code*: should most often be ignored as an example of *premature optimization*.
- It is too complicated to check.

Writing pre-conditions (even as comments) also has a significant benefit for the quality of your programs: it forces you to think about what a function requires. If you can't state that simply and precisely in a couple of comment lines, you probably haven’t thought hard enough about what you are doing. Experience shows that writing those pre-condition comments and the pre-condition tests helps you avoid many design mistakes.

### Post-conditions

Stating pre-conditions helps us improve our design and catch usage errors early. Can this idea of explicitly stating requirements be used elsewhere? Yes, one more place immediately springs to mind: the return value! After all, we typically have to state what a function returns; that is, if we return a value from a function we are always making a *promise* about the return value (how else would a caller know what to expect?).

```c++
int area(int length, int width)
// calculate area of a rectangle;
// pre-conditions: length and width are positive
// post-condition: returns a positive value that is the area
{
  if (length <= 0 || width <= 0) error("area() pre-condition");
  int a =  length * width;
  if (a <= 0) error("area() post-condition");
  return a;
}
```

> TRY THIS: Find a pair of values so that the pre-condition of this version holds, but the post-condition doesn't.

When `length * width` overflows, a negative value is assigned to the variable `a`; thus, the post-condition doesn't hold.
