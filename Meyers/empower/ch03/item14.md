# Declare functions `noexcept`

Most functions are *exception-neutral*. Such functions throw no exceptions themselves, but functions they call might emit one. When that happens, the exception-neutral function allows the emitted exception to pass through on its way to a handler further up the call chain. Exception-neutral functions are never `noexcept`, because they may emit such *just passing through* exceptions. Most functions, therefore, quite properly lack the `noexcept` designation.

In C++11, all memory deallocation functions and all destructors — both user-defined and compiler-generated — are implicitly `noexcept`. There's thus no need to declare them `noexcept`. Doing so doesn’t hurt anything, it's just unconventional.

`noexcept` is
- part of a function's interface, and that means that callers may depend on it.
- particularly valuable for:
  - move operations,
  - `swap`,
  - memory deallocation functions, and
  - destructors.

## `noexcept` is part of a function interface

You should declare a function `noexcept` only if you are willing to commit to a `noexcept` implementation over the long term. If you declare a function `noexcept` and later regret that decision, your options are bleak.

- You can remove `noexcept` from the function's declaration (i.e., change its interface), thus running the risk of breaking client code.
- You can change the implementation such that an exception could escape, yet keep the original (now incorrect) exception specification. If you do that, your program will be terminated if an exception tries to leave the function.

## `noexcept` and C++11 move semantics

When a new element is added to a `std::vector`, it's possible that the `std::vector` lacks space for it, i.e., that the `std::vector`'s size is equal to its capacity. When that happens, the `std::vector` allocates a new, larger, chunk of memory to hold its elements, and it transfers the elements from the existing chunk of memory to the new one.

In C++11, a natural optimization would be to replace the copying of `std::vector` elements with moves. Unfortunately, doing this runs the risk of violating `push_back`'s exception safety guarantee. If `n` elements have been moved from the old memory and an exception is thrown moving element `n+1`, the `push_back` operation can't run to completion. But the original `std::vector` has been modified: `n` of its elements have been moved from. Restoring their original state may not be possible, because attempting to move each object back into the original memory may itself yield an exception.

Therefore, C++11 implementations can't silently replace copy operations inside `push_back` with moves unless it's known that the move operations won't emit exceptions. In that case, having moves replace copies would be safe, and the only side effect would be improved performance.

`std::vector::push_back` takes advantage of this *move if you can, but copy if you must* strategy. Other functions sporting the strong exception safety guarantee (e.g., `std::vector::reserve`, `std::deque::insert`, etc.) behave the same way. All these functions replace calls to copy operations in C++98 with calls to move operations in C++11 only if the move operations are known to not emit exceptions.

Some functions, however, have natural implementations that emit no exceptions, and for a few more — notably the *move* operations and `swap` — being `noexcept` can have such a significant payoff, it's worth implementing them in a `noexcept` manner if at all possible. When you can honestly say that a function should never emit exceptions, you should definitely declare it `noexcept`.

# [Exceptions](https://en.cppreference.com/w/cpp/language/exceptions)

An exception can be thrown by a

- throw-expression,
- `dynamic_cast`,
- `typeid`,
- new-expression,
- allocation function,

and any of the standard library functions that are specified to throw exceptions to signal certain error conditions (e.g. `std::vector::at`, `std::string::substr`, etc).

In particular, this implies that the failures of constructors (see also RAII) and most operators should be reported by throwing exceptions.

In addition, so-called wide contract functions use exceptions to indicate *unacceptable inputs*, for example, `std::string::at` has no pre-conditions, but throws an exception to indicate index out of range.

## Error handling

Throwing an exception is used to signal errors from functions, where *errors* are typically limited to only the following

1. Failures to meet the post-conditions, such as failing to produce a valid return value object
2. Failures to meet the pre-conditions of another function that must be called
3. (for non-private member functions) Failures to (re)establish a class invariant

## Exception safety

After the error condition is reported by a function, additional guarantees may be provided with regards to the state of the program. The following four levels of exception guarantee are generally recognized:

1.  No throw (or nofail) exception guarantee — the function never throws exceptions. The destructors are `noexcept` by default.
    - No throw (errors are reported by other means or concealed) is expected of destructors and other functions that may be called during stack unwinding.
    - Nofail (the function always succeeds) is expected of swaps, move constructors, and other functions used by those that provide strong exception guarantee.
2.  Strong exception guarantee — If the function throws an exception, the state of the program is rolled back to the state just before the function call.
3.  Basic exception guarantee — If the function throws an exception, the program is in a valid state. No resources are leaked, and all objects' invariants are intact.
4.  No exception guarantee — If the function throws an exception, the program may not be in a valid state: resource leaks, memory corruption, or other invariant-destroying errors may have occurred.

Generic components may, in addition, offer exception-neutral guarantee: if an exception is thrown from a template parameter, it is propagated, unchanged, to the caller.

## Exception objects

While objects of any complete type and cv pointers to void may be thrown as exception objects, all standard library functions throw anonymous temporary objects **by value**, and the types of those objects are derived (directly or indirectly) from `std::exception`. User-defined exceptions usually follow this pattern.

To avoid unnecessary copying of the exception object and object slicing, the best practice for catch clauses is to catch **by reference**.

# C++ Core Guideline

## [I.10](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#i10-use-exceptions-to-signal-a-failure-to-perform-a-required-task): Use exceptions to signal a failure to perform a required task

What is an error?

An error means that the function cannot achieve its advertised purpose (including establishing post-conditions). Calling code that ignores an error could lead to wrong results or undefined systems state. For example,

- not being able to connect to a remote server is not by itself an error: the server can refuse a connection for all kinds of reasons, so the natural thing is to return a result that the caller should always check.
- However, if failing to make a connection is considered an error, then a failure should throw an exception.

If you can't use exceptions (e.g., because your code is full of old-style raw-pointer use or because there are hard-real-time constraints), consider using a style that returns a pair of values:

```c++
auto [val, error_code] = do_something();
if (error_code) {
  // ... handle the error or exit ...
}
// ... use val ...
```

## [E.15](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#e15-throw-by-value-catch-exceptions-from-a-hierarchy-by-reference): Throw by value, catch exceptions from a hierarchy by reference

Throwing by value (not by pointer) and catching by reference prevents copying, especially slicing base sub-objects.

```c++
struct MyError {};
enum class alert {RED, YELLOW, GREEN};

void f()
{
  try {
    // throw new MyError{};               // don't: by raw pointer
    throw MyError{};
    throw std::runtime_error("something bad");
    throw alert::RED;
  // } catch (MyError e) {                // don't: copying
  } catch (const MyError& e) {
  } catch (const std::exception& e) {     // catches runtime_error
    e.what();
    // throw e;                           // don't: throwing a sliced copy
    throw;
  } catch (alert e) {
    if (e == alert::RED);
  }
}
```

`enum` classes are also allowed. Catch by value can be appropriate for a small value type such as an `enum` value.

To rethrow a caught exception use `throw;` not `throw e;`. Using `throw e;` would throw a new copy of `e` (sliced to the static type `std::exception`, when the exception is caught by `catch (const std::exception& e)`) instead of re-throwing the original exception.
