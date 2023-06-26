# Testing

Write a binary search. A binary search is a search in a sorted sequence that starts at the middle:

- If the middle element is equal to what we are searching for, we are finished.
- If the middle element is less than what we are searching for, we look at the right-hand half, doing a binary search on that.
- If the middle element is greater than what we are searching for, we look at the left-hand half, doing a binary search on that.
- The result is an indicator of whether the search was successful and something that allows us to modify the element, if found, such as an index, a pointer, or an iterator.

Use less than (`<`) as the comparison (sorting) criterion. Feel free to use any data structure you like, any calling conventions you like, and any way of returning the result that you like.

You need to write a specification that conforms above mentioned requirements:

```c++
/**
 * Searches an array of char elements (data[] of sz elements) for a char (val).
 * 
 * pre-conditions:
 * - Elements of the array shall be sorted by '<' (less than).
 * - 0 < sz < std::numeric_limits<std::size_t>::max().
 * post-conditions:
 * - none
 * returns:
 * - an index to point to an element in data[] if val is found, or
 *   the same value as sz otherwise
 */
std::size_t binary_search(const char data[], std::size_t sz, char val);
```

Then you might start writing code to implement the function. Once you done, you must ask yourself; How do you know that your function is correct?

An approach to convince yourself that your code is correct is *Test Driven Development* (TDD).

> Test-Driven Development (TDD): A way of developing software where the test cases are developed, and often automated, before the software is developed to run those test cases. (ISTQB Glossary)

## Test Driven Development (TDD)

TDD has been around for a long time and can be traced back to Extreme Programming practices developed in the late 1990s. TDD relies on two simple principles:

- Don’t write any code until you’ve written a failing test that demonstrates why you need this code.
- Refactor regularly to avoid duplication and keep the code quality high.

The cornerstone of TDD is the idea of writing a unit test before you write the corresponding code. But TDD is much more than just a guarantee that every class has a corresponding set of unit tests. With enough discipline, any experienced developer writing unit tests after writing the code can achieve that outcome as well.

TDD’s killer feature is that it forces developers to think about the code they’re going to write before they write it, in practical and unambiguous terms — you need to understand the functionality before you can write a unit test for it. This way, developers resist the temptation to just start coding something and actively think about what they need to achieve. In this respect, a better term for this practice might be something like *Test-Driven Design.*

Code developed using TDD also benefits from a comprehensive set of regression tests, which, coupled with the clean design that TDD encourages, makes the application easier to change and cheaper to maintain. This, along with the lower bug count, reduces maintenance costs and the total cost of ownership significantly.

(§10.1. BDD, TDD, and unit testing, *BDD in Action*, John Ferguson Smart, September 2014)

## Behavior Driven Development (BDD)

One of the core practices used in BDD is *outside-in development*. This involves using acceptance criteria to drive the implementation details you need to build business features. You start with the outcomes you expect and use these outcomes to determine what code you need to write.

The process typically iterates over the following steps:

1. Start with a high-level acceptance criterion that you want to implement.
2. Automate the acceptance criterion as pending scenarios, breaking the acceptance criterion into smaller steps.
3. Implement the acceptance criterion step definitions, imagining the code you’d like to have to make each step work.
4. Use these step definitions to flesh out unit tests that specify how the application code will behave.
5. Implement the application code, and refactor as required.

The last two steps, where you use unit tests to describe low-level behavior and build the corresponding application code, are what many developers would describe as a form of TDD. In an outside-in approach, the description of the low-level technical behavior of the application flows naturally from discussing, thinking about, automating, and implementing the high-level acceptance criteria.

(§10.2. Going from acceptance criteria to implemented features, *BDD in Action*, John Ferguson Smart, September 2014)

## Iterating testing, design, coding

So write the initial version of test code. Checking obvious boundary conditions often should be a good start.

```c++
int main()
{
  using namespace boost::ut;
  using namespace boost:ut::bdd;

  "binary_search"_test = [] {
    given("a number of elements") = [] {
    const char data[] = "cdefghijklmno";
    when("one before the first") = [&] {
      expect(my::binary_search(data, 13, 'b') == 13_ul);
    };
    when("the first") = [&] {
      expect(my::binary_search(data, 13, 'c') == 0_ul);
    };
    when("one after the first") = [&] {
      expect(my::binary_search(data, 13, 'd') == 1_ul);
    };
    when("one before the last") = [&] {
      expect(my::binary_search(data, 13, 'n') == 11_ul);
    };
    when("the last") = [&] {
      expect(my::binary_search(data, 13, 'o') == 12_ul);
    };
    when("one after the last") = [&] {
      expect(my::binary_search(data, 13, 'p') == 13_ul);
    };
    };
  };
}
```

The binary search for the initial test cases is:

```c++
std::size_t binary_search(const char data[], std::size_t sz, char val)
{
  return sz;    // if val is not found in data[]
}
```

Most test cases fail:

```bash
$ build/test/ch26/binary_search_ut
Running "binary_search"...
 "a number of elements"...
 "one before the first"...
 "the first"...
  binary_search_ut.cpp:16:FAILED [13 == 0]
 "one after the first"...
  binary_search_ut.cpp:19:FAILED [13 == 1]
 "one before the last"...
  binary_search_ut.cpp:33:FAILED [13 == 11]
 "the last"...
  binary_search_ut.cpp:36:FAILED [13 == 12]
 "one after the last"...
```

Now you can start designing the binary search:

```c++
int main()
{
  using namespace boost::ut;
  using namespace boost:ut::bdd;

  "binary_search"_test = [] {
    given("a number of elements") = [] {
    //       0123456789012
    const char data[] = "cdefghijklmno";
    // searching the range [begin, end)
    // std::size_t begin = 0;
    // std::size_t end = sz;
    // std::size_t i;               // current middle element

    when("one before the first") = [&] {
    //                      0123456789012
    // const char data[] = "cdefghijklmno";
      expect(my::binary_search(data, 13, 'b') == 13_ul);
      // while (begin < end)        // 0 < 13 = true
      // i = (begin + end - 1)/2;   // (0+13-1)/2 = 6
      // if (data[i] < val)
      //   begin = i + 1;
      // else if (val < data[i])    // 'b' < 'i'
      //   end = i;                 // end = 6;
      // else
      //   return i;
      // while (begin < end)        // 0 < 6 = true
      // i = (begin + end - 1)/2;   // (0+6-1)/2 = 2
      // if (data[i] < val)
      //   begin = i + 1;
      // else if (val < data[i])    // 'b' < 'e'
      //   end = i;                 // end = 2;
      // else
      //   return i;
      // while (begin < end)        // 0 < 2 = true
      // i = (begin + end - 1)/2;   // (0+2-1)/2 = 0
      // if (data[i] < val)
      //   begin = i + 1;
      // else if (val < data[i])    // 'b' < 'a'
        //   end = i;               // end = 0;
        // else
        //   return i;
        // while (begin < end)      // 0 < 0 = false
        // return sz;               // return 13;
      };
      when("the first") = [&] {
      //                      0123456789012
      // const char data[] = "cdefghijklmno";
        expect(my::binary_search(data, 13, 'c') == 0_ul);
        // while (begin < end)      // 0 < 13 = true
        // i = (begin + end - 1)/2; // (0+13-1)/2 = 6
        // if (data[i] < val)
        //   begin = i + 1;
        // else if (val < data[i])  // 'c' < 'i'
        //   end = i;               // end = 6;
        // else
        //   return i;
        // while (begin < end)      // 0 < 6 = true
        // i = (begin + end - 1)/2; // (0+6-1)/2 = 2
        // if (data[i] < val)
        //   begin = i + 1;
        // else if (val < data[i])  // 'c' < 'e'
        //   end = i;               // end = 2;
        // else
        //   return i;
        // while (begin < end)      // 0 < 2 = true
        // i = (begin + end - 1)/2; // (0+2-1)/2 = 0
        // if (data[i] < val)
        //   begin = i + 1;
        // else if (val < data[i])
        //   end = i;
        // else                     // 'c' == 'c'
        //   return i;              // return 0;
      };
      when("one after the first") = [&] {
      //                      0123456789012
      // const char data[] = "cdefghijklmno";
        expect(my::binary_search(data, 13, 'd') == 1_ul);
        // while (begin < end)        // 0 < 13 = true
        // i = (begin + end - 1)/2;   // (0+13-1)/2 = 6
        // if (data[i] < val)
        //   begin = i + 1;
        // else if (val < data[i])    // 'd' < 'i'
        //   end = i;                 // end = 6;
        // else
        //   return i;
        // while (begin < end)        // 0 < 6 = true
        // i = (begin + end - 1)/2;   // (0+6-1)/2 = 2
        // if (data[i] < val)
        //   begin = i + 1;
        // else if (val < data[i])    // 'd' < 'e'
        //   end = i;                 // end = 2;
        // else
        //   return i;
        // while (begin < end)        // 0 < 2 = true
        // i = (begin + end - 1)/2;   // (0+2-1)/2 = 0
        // if (data[i] < val)         // 'c' < 'd'
        //   begin = i + 1;           // begin = 1;
        // else if (val < data[i])
        //   end = i;
        // else
        //   return i;
        // while (begin < end)        // 1 < 2 = true
        // i = (begin + end - 1)/2;   // (1+2-1)/2 = 1
        // if (data[i] < val)
        //   begin = i + 1;
        // else if (val < data[i])
        //   end = i;
        // else                       // 'd' == 'd'
        //   return i;                // return 1;
      };
      when("one before the last") = [&] {
      //                      0123456789012
      // const char data[] = "cdefghijklmno";
        expect(my::binary_search(data, 13, 'n') == 11_ul);
        // while (begin < end)        // 0 < 13 = true
        // i = (begin + end - 1)/2;   // (0+13-1)/2 = 6
        // if (data[i] < val)         // 'i' < 'n'
        //   begin = i + 1;           // begin = 7;
        // else if (val < data[i])
        //   end = i;
        // else
        //   return i;
        // while (begin < end)        // 7 < 13 = true
        // i = (begin + end - 1)/2;   // (7+13-1)/2 = 9
        // if (data[i] < val)         // 'l' < 'n'
        //   begin = i + 1;           // begin = 10;
        // else if (val < data[i])
        //   end = i;
        // else
        //   return i;
        // while (begin < end)        // 10 < 13 = true
        // i = (begin + end - 1)/2;   // (10+13-1)/2 = 11
        // if (data[i] < val)
        //   begin = i + 1;
        // else if (val < data[i])
        //   end = i;
        // else                       // 'n' == 'n'
        //   return i;                // return 11;
      };
      when("the last") = [&] {
      //                      0123456789012
      // const char data[] = "cdefghijklmno";
        expect(my::binary_search(data, 13, 'o') == 12_ul);
        // while (begin < end)        // 0 < 13 = true
        // i = (begin + end - 1)/2;   // (0+13-1)/2 = 6
        // if (data[i] < val)         // 'i' < 'o'
        //   begin = i + 1;           // begin = 7;
        // else if (val < data[i])
        //   end = i;
        // else
        //   return i;
        // while (begin < end)        // 7 < 13 = true
        // i = (begin + end - 1)/2;   // (7+13-1)/2 = 9
        // if (data[i] < val)         // 'l' < 'o'
        //   begin = i + 1;           // begin = 10;
        // else if (val < data[i])
        //   end = i;
        // else
        //   return i;
        // while (begin < end)        // 10 < 13 = true
        // i = (begin + end - 1)/2;   // (10+13-1)/2 = 11
        // if (data[i] < val)         // 'n' < 'o'
        //   begin = i + 1;           // begin = 12;
        // else if (val < data[i])
        //   end = i;
        // else
        //   return i;
        // while (begin < end)        // 12 < 13 = true
        // i = (begin + end - 1)/2;   // (12+13-1)/2 = 12
        // if (data[i] < val)
        //   begin = i + 1;
        // else if (val < data[i])
        //   end = i;
        // else                       // 'o' == 'o'
        //   return i;                // return 12;
      };
      when("one after the last") = [&] {
      //                      0123456789012
      // const char data[] = "cdefghijklmno";
        expect(my::binary_search(data, 13, 'p') == 13_ul);
        // while (begin < end)        // 0 < 13 = true
        // i = (begin + end - 1)/2;   // (0+13-1)/2 = 6
        // if (data[i] < val)         // 'i' < 'p'
        //   begin = i + 1;           // begin = 7;
        // else if (val < data[i])
        //   end = i;
        // else
        //   return i;
        // while (begin < end)        // 7 < 13 = true
        // i = (begin + end - 1)/2;   // (7+13-1)/2 = 9
        // if (data[i] < val)         // 'l' < 'p'
        //   begin = i + 1;           // begin = 10;
        // else if (val < data[i])
        //   end = i;
        // else
        //   return i;
        // while (begin < end)        // 10 < 13 = true
        // i = (begin + end - 1)/2;   // (10+13-1)/2 = 11
        // if (data[i] < val)         // 'n' < 'p'
        //   begin = i + 1;           // begin = 12;
        // else if (val < data[i])
        //   end = i;
        // else
        //   return i;
        // while (begin < end)        // 12 < 13 = true
        // i = (begin + end - 1)/2;   // (12+13-1)/2 = 12
        // if (data[i] < val)         // 'o' < 'p'
        //   begin = i + 1;           // begin = 13;
        // else if (val < data[i])
        //   end = i;
        // else
        //   return i;
        // while (begin < end)        // 13 < 13 = false
        // return sz;                 // return 13;
      };
    };
  };
}
```

Now that we know how the binary search should work, we can implement it:

```c++
std::size_t binary_search(const char data[], std::size_t sz, char val)
{
  // searching the range [begin, end)
  std::size_t begin = 0;
  std::size_t end = sz;
  std::size_t i;        // current middle element

  while (begin < end) {
    i = (begin + end - 1)/2;
    if (data[i] < val)
      begin = i + 1;
    else if (val < data[i])
      end = i;
    else  // data[i] == val
      return i;
  }

  return sz;          // if val is not found in data[]
}
```

With the binary search function, run the initial test code:

```bash
$ build/test/ch26/binary_search_ut
All tests passed (6 asserts in 1 tests)
```

## Hidden conditions

The initial test cases only test obvious boundary conditions and it happen to cover all branches including an implicit branching of the `while` loop. Are we dne? No. Recall the algorithm:

- If the middle element is equal to what we are searching for, we are finished.
- If the middle element is less than what we are searching for, we look at the right-hand half, doing a binary search on that.
- If the middle element is greater than what we are searching for, we look at the left-hand half, doing a binary search on that.

The binary search algorithm partitions the argument `data[]` into the left-hand half and the right-hand half. When we partition a sequence, the point where we split it becomes the end of a subsequence, and we know that errors are likely near ends of sequences.

And the point where the `data[]` is split moves one forwards or backwards if the size of `data[]` is even or odd respectively:

```c++
    i = (begin + end - 1)/2;
```

Thus, we need to upgrade the test cases to cover the hidden boundary and branching conditions:

```c++
int main()
{
  using namespace boost::ut;
  using namespace boost::ut::bdd;

  "binary_search"_test = [] {
    given("odd number of elements") = [] {
      const char data[] = "cdefghijklmno";
      when("one before the first") = [&] {
        expect(my::binary_search(data, 13, 'b') == 13_ul);
      };
      when("the first") = [&] {
        expect(my::binary_search(data, 13, 'c') == 0_ul);
      };
      when("one after the first") = [&] {
        expect(my::binary_search(data, 13, 'd') == 1_ul);
      };
      when("one before the middle") = [&] {
        expect(my::binary_search(data, 13, 'h') == 5_ul);
      };
      when("the middle") = [&] {
        expect(my::binary_search(data, 13, 'i') == 6_ul);
      };
      when("one after the middle") = [&] {
        expect(my::binary_search(data, 13, 'j') == 7_ul);
      };
      when("one before the last") = [&] {
        expect(my::binary_search(data, 13, 'n') == 11_ul);
      };
      when("the last") = [&] {
        expect(my::binary_search(data, 13, 'o') == 12_ul);
      };
      when("one after the last") = [&] {
        expect(my::binary_search(data, 13, 'p') == 13_ul);
      };
    };
    given("even number of elements") = [] {
      const char data[] = "cdefghijklmnop";
      when("one before the first") = [&] {
        expect(my::binary_search(data, 14, 'b') == 14_ul);
      };
      when("the first") = [&] {
        expect(my::binary_search(data, 14, 'c') == 0_ul);
      };
      when("one after the first") = [&] {
        expect(my::binary_search(data, 14, 'd') == 1_ul);
      };
      when("one before the left-middle") = [&] {
        expect(my::binary_search(data, 14, 'h') == 5_ul);
      };
      when("the left-middle") = [&] {
        expect(my::binary_search(data, 14, 'i') == 6_ul);
      };
      when("the right-middle") = [&] {
        expect(my::binary_search(data, 14, 'j') == 7_ul);
      };
      when("one after the right-middle") = [&] {
        expect(my::binary_search(data, 14, 'k') == 8_ul);
      };
      when("one before the last") = [&] {
        expect(my::binary_search(data, 14, 'o') == 12_ul);
      };
      when("the last") = [&] {
        expect(my::binary_search(data, 14, 'p') == 13_ul);
      };
      when("one after the last") = [&] {
        expect(my::binary_search(data, 14, 'q') == 14_ul);
      };
    };
  };
}
```

Luckily, the binary search works:

```c++
$ build/test/ch26/binary_search_ut
All tests passed (19 asserts in 1 tests)
```
