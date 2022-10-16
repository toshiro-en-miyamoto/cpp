# Testing Frameworks

## Standardize testing

Ultimately, automated testing involves nothing other than running an executable that

- sets your *system under test* (or SUT) in a `GIVEN` state,
- `WHEN` performs tested operations, and
- `THEN` checks if the results match expectations.

In the `test` mode, CTest should be executed in the build tree, after building the project with `cmake`.

```bash
$ ctest [<options>]
```

You'd need to execute multiple commands and change the working directory back and forth. To simplify the process, CTest added a second mode: `build-and-test` mode.

To use the `build-and-test` mode, we need to execute `ctest` starting with `--build-and-test`, as follows. Essentially, this is a simple wrapper around the regular test mode that accepts a few build configuration options.

```bash
$ ctest --build-and-test <path-to-source> <path-to-build>
        --build-generator <generator> [<options>...]
        [--build-options <opts>...]
        [--test-command <command> [<args>...]]
```

The only requirement here is to pass the full command after the `--test-command argument`. The `build-and-test` mode won't actually run any tests unless provided with `ctest` keyword after `--test-command`:

```bash
$ ctest --build-and-test    proj/src-tree proj/build-tree \
        --build-generator   "Unix Makefiles" \
        --test-command      ctest
```

In this command, we specify source and build paths, and select a build generator. All three are required.

> `"Unix Makefiles"` is the default generator of `cmake`. It tells `cmake` to generate standard UNIX makefiles. (`cmake --help`)

CTest offers an `-N` option, which disables execution and only prints a list:

```bash
$ ctest -N
```

There are plenty of reasons to run only a subset of all tests—the most common one might be the need to debug a single failing test or a module you're working on.

- `-R <r>`, `--tests-regex <r>`: Only run tests with names matching `<r>`
- `-E <r>`, `--exclude-regex <r>`: Skip tests with names matching `<r>`
- `-L <r>`, `--label-regex <r>`: Only run tests with labels matching `<r>`
- `-LE <r>`, `--label-exclude <regex>`: Skip tests with labels matching `<r>`

> Fail early, fail often, but always fail forward.

- `--debug`: Displaying more verbose internals of CTest.
- `--output-on-failure`: Output anything outputted by the test program if the test should fail.
- `--stop-on-failure`: Stop running the tests when the first failure happens.
- `-O <file>`, `--output-log <file>`: Output to log file. CTest writes all its output to a <file> log file.

## The most basic unit

```c++
// chapter08/02-structured/src/calc.h
#pragma once

class Calc {
public:
    int Sum(int a, int b);
    int Multiply(int a, int b);
};
```

[`#pragma once`](https://en.cppreference.com/w/cpp/preprocessor/impl) is a non-standard pragma that is supported by the vast majority of modern compilers. If it appears in a header file, it indicates that it is only to be parsed once, even if it is (directly or indirectly) included multiple times in the same source file.

```c++
// chapter08/02-structured/src/calc.cpp
#include "calc.h"

int Calc::Sum(int a, int b) {
    return a + b;
}

int Calc::Multiply(int a, int b) {
    return a * a; // a mistake!
}
```

We introduced a mistake! `Multiply` is ignoring the `b` argument and returns `a` squared instead. That should be detected by correctly written unit tests.

The entry point `main()` of the release build looks like:

```c++
// chapter08/02-structured/src/bootstrap.cpp
int run(); // declaration
int main() {
    run();
}
```

The `main()` is to provide an entry point and call the `run()` forwarding command-line arguments (if any). All that's left is to link everything together, and we've got ourselves a testable project.

```c++
// chapter08/02-structured/src/run.cpp
#include <iostream>
#include "calc.h"
using namespace std;

int run() {
    Calc c;
    cout << "2 + 2 = " << c.Sum(2, 2) << endl;
    cout << "3 * 3 = " << c.Multiply(3, 3) << endl;
    return 0;
}
```

The mistake in `Multiply()` should be detected by correctly written unit tests:

```c++
// chapter08/02-structured/test/calc_test.cpp
#include "calc.h"
#include <cstdlib>

void SumAddsTwoIntegers() {
    Calc sut;
    if (4 != sut.Sum(2, 2))
        std::exit(1);
}

void MultiplyMultipliesTwoIntegers() {
    Calc sut;
    if(3 != sut.Multiply(1, 3))
        std::exit(1);
}
```

We could use `assert()`, `abort()`, or `terminate()` here, but that would result in a less explicit `Subprocess aborted` message in the output of `ctest`, instead of the more readable `Failed` message.

```c++
// chapter08/02-structured/test/run_test.cpp
#include <string>
#include <iostream>
#include <sstream>

using namespace std;
int run(); // declaration
void RunOutputsCorrectEquations() {
    string expected {"2 + 2 = 4\n3 * 3 = 9\n"};
    stringstream buffer;
    // redirect cout
    auto prevcoutbuf = cout.rdbuf(buffer.rdbuf());

    run();

    auto output = buffer.str();
    // restore original buffer
    cout.rdbuf(prevcoutbuf);
    if (expected != output)
        exit(1);
}
```

The entry point for unit tests call not only tests for `calc_test` but also `run()`.

```c++
// chapter08/02-structured/test/unit_tests.cpp
#include <string>
void SumAddsTwoIntegers();
void MultiplyMultipliesTwoIntegers();
int RunOutputsCorrectEquations();

int main(int argc, char *argv[]) {
    if (argc < 2 || argv[1] == std::string("1"))
        SumAddsTwoIntegers();

    if (argc < 2 || argv[1] == std::string("2"))
        MultiplyMultipliesTwoIntegers();

    if (argc < 2 || argv[1] == std::string("3"))
        RunOutputsCorrectEquations();
}
```

We simplified the code as much as we could, and it still turned out to be pretty hard to read. Anyone who might need to maintain this section isn't going to have a great time after adding a few more tests, not to mention that this functionality is pretty raw—debugging such a test suite will be a lot of work.

Then, we can use it with CTest:

```cmake
# chapter08/02-structured/CMakeLists.txt
cmake_minimum_required(VERSION 3.20.0)
project(NoFrameworkTestsStructured CXX)
enable_testing()
add_subdirectory(src bin)
add_subdirectory(test)
```

`enable_testing()` is to tell CTest that we'd like to enable tests in this directory and its subdirectories. Next, we'll include two nested listfiles in each of the subdirectories: `src` and `test`.

The `bin` value in `add_subdirectory(src bin)` states that we'd like the binary output of `src` subdirectories to be placed in `<build_tree>/bin`. Otherwise, binary files would end up in `<build_tree>/src`.

```cmake
# chapter08/02-structured/src/CMakeLists.txt
add_library(sut STATIC calc.cpp run.cpp)
target_include_directories(sut PUBLIC .)

add_executable(bootstrap bootstrap.cpp)
target_link_libraries(bootstrap PRIVATE sut)
```

> Note: The target names, `sut` and `bootstrap`, are chosen from the perspective of testing. In real-life projects, you should pick names that match the context of the production code.

```cmake
# chapter08/02-structured/test/CMakeLists.txt
add_executable(unit_tests
    unit_tests.cpp
    calc_test.cpp
    run_test.cpp)
target_link_libraries(unit_tests PRIVATE sut)

add_test(NAME SumAddsTwoInts COMMAND unit_tests 1)
add_test(NAME MultiplyMultipliesTwoInts COMMAND unit_tests 2)
add_test(NAME RunOutputsCorrectEquations COMMAND unit_tests 3)
```

We have now defined a second target `unit_tests` that also uses the `calc.cpp`, `run.cpp` and respective header in the SUT library `sut`.

Don't get too attached to `add_test()` — in the *Unit-testing frameworks* section, we'll discover a much better way of dealing with test cases.

Let's run the test:

```bash
$ cmake -B build
-- The CXX compiler identification is GNU 10.2.1
-- 
-- Configuring done
-- Generating done
-- Build files have been written to: /.../02-structured/build

$ cmake --build build/
--
[100%] Built target unit_tests
```

Artifacts from the build step goes into:

- `build/bin`:
  - `bootstrap`
  - `libsut.a`
- `build/test`:
  - `unit_tests`

Having configured and built the project, we are goot to run test cases:

```bash
$ cd build
$ ctest
Test project /.../02-structured/build
    Start 1: SumAddsTwoInts
1/3 Test #1: SumAddsTwoInts ...................   Passed    0.00 sec
    Start 2: MultiplyMultipliesTwoInts
2/3 Test #2: MultiplyMultipliesTwoInts ........***Failed    0.01 sec
    Start 3: RunOutputsCorrectEquations
3/3 Test #3: RunOutputsCorrectEquations .......   Passed    0.01 sec

67% tests passed, 1 tests failed out of 3

Total Test time (real) =   0.03 sec

The following tests FAILED:
          2 - MultiplyMultipliesTwoInts (Failed)
Errors while running CTest
Output from these tests are in: /.../02-structured/build/Testing/Temporary/LastTest.log
Use "--rerun-failed --output-on-failure" to re-run the failed cases verbosely.
```

Although CTest writes the `LastTest.log`, it only tells `2/3 Test #2: MultiplyMultipliesTwoInts` failed and does not show what was wrong:

```bash
$ cat Testing/Temporary/LastTest.log
Start testing: Oct 14 17:56 PST
----------------------------------------------------------
1/3 Testing: SumAddsTwoInts
1/3 Test: SumAddsTwoInts
Command: "/.../02-structured/build/test/unit_tests" "1"
Directory: /.../02-structured/build/test
"SumAddsTwoInts" start time: Oct 14 17:56 PST
Output:
----------------------------------------------------------
<end of output>
Test time =   0.00 sec
----------------------------------------------------------
Test Passed.
"SumAddsTwoInts" end time: Oct 14 17:56 PST
"SumAddsTwoInts" time elapsed: 00:00:00
----------------------------------------------------------

2/3 Testing: MultiplyMultipliesTwoInts
2/3 Test: MultiplyMultipliesTwoInts
Command: "/.../02-structured/build/test/unit_tests" "2"
Directory: /.../02-structured/build/test
"MultiplyMultipliesTwoInts" start time: Oct 14 17:56 PST
Output:
----------------------------------------------------------
<end of output>
Test time =   0.01 sec
----------------------------------------------------------
Test Failed.
"MultiplyMultipliesTwoInts" end time: Oct 14 17:56 PST
"MultiplyMultipliesTwoInts" time elapsed: 00:00:00
----------------------------------------------------------

3/3 Testing: RunOutputsCorrectEquations
3/3 Test: RunOutputsCorrectEquations
Command: "/.../02-structured/build/test/unit_tests" "3"
Directory: /.../02-structured/build/test
"RunOutputsCorrectEquations" start time: Oct 14 17:56 PST
Output:
----------------------------------------------------------
<end of output>
Test time =   0.01 sec
----------------------------------------------------------
Test Passed.
"RunOutputsCorrectEquations" end time: Oct 14 17:56 PST
"RunOutputsCorrectEquations" time elapsed: 00:00:00
----------------------------------------------------------

End testing: Oct 14 17:56 PST
```

Let's fix the mistake anyway and rerun the faild test:

```bash
$ cd ..
$ vi src/calc.cpp

$ ctest --build-and-test    ./ ./build \
        --build-generator   "Unix Makefiles" \
        --test-command      ctest \
        --rerun-failed --output-on-failure

Internal cmake changing into directory: /.../02-structured/build
======== CMake output     ======
Configuring done
Generating done
Build files have been written to: /.../02-structured/build
======== End CMake output ======
Change Dir: /.../02-structured/build

Run Clean Command:/usr/bin/gmake -f Makefile clean

Run Build Command(s):/usr/bin/gmake -f Makefile ...
...
[100%] Built target unit_tests

Running test command: "/usr/local/cmake-3.23.4/bin/ctest" "--rerun-failed" "--output-on-failure"
Test project /.../02-structured/build
    Start 2: MultiplyMultipliesTwoInts
1/1 Test #2: MultiplyMultipliesTwoInts ........   Passed    0.01 sec

100% tests passed, 0 tests failed out of 1

Total Test time (real) =   0.01 sec
```

As the option `--rerun-failed` runs only previously failed tests, let's run all test cases to make sure everything is fine:

```bash
$ cd build
$ ctest
Test project /.../02-structured/build
    Start 1: SumAddsTwoInts
1/3 Test #1: SumAddsTwoInts ...................   Passed    0.01 sec
    Start 2: MultiplyMultipliesTwoInts
2/3 Test #2: MultiplyMultipliesTwoInts ........   Passed    0.01 sec
    Start 3: RunOutputsCorrectEquations
3/3 Test #3: RunOutputsCorrectEquations .......   Passed    0.00 sec

100% tests passed, 0 tests failed out of 3

Total Test time (real) =   0.02 sec
```

## Unit-test frameworks

### Structures

Files of the `src` don't change when using GTest:

```bash
$ for f in `ls 04-gtest/src`; \
  do echo "$f" && diff 02-structured/src/"$f" 04-gtest/src/"$f";  \
  done
bootstrap.cpp
calc.cpp
calc.h
CMakeLists.txt
run.cpp
```

`unit_tests.cpp` is no longer required when using GTest.

```bash
$ ls 02-structured/test
calc_test.cpp  CMakeLists.txt  run_test.cpp  unit_tests.cpp

ch08 $ ls 04-gtest/test
calc_test.cpp  CMakeLists.txt  run_test.cpp
```

As a reminder, `unit_tests.cpp` is the entry point for running unit tests:

```c++
// chapter08/02-structured/test/unit_tests.cpp
#include <string>
void SumAddsTwoIntegers();
void MultiplyMultipliesTwoIntegers();
int RunOutputsCorrectEquations();

int main(int argc, char *argv[]) {
    if (argc < 2 || argv[1] == std::string("1"))
        SumAddsTwoIntegers();

    if (argc < 2 || argv[1] == std::string("2"))
        MultiplyMultipliesTwoIntegers();

    if (argc < 2 || argv[1] == std::string("3"))
        RunOutputsCorrectEquations();
}
```

### Configurations

As another reminder, `test/CMakeLists.txt` for testing without test-framework is:

```cmake
# chapter08/02-structured/test/CMakeLists.txt
add_executable(unit_tests
    unit_tests.cpp
    calc_test.cpp
    run_test.cpp)
target_link_libraries(unit_tests PRIVATE sut)

add_test(NAME SumAddsTwoInts COMMAND unit_tests 1)
add_test(NAME MultiplyMultipliesTwoInts COMMAND unit_tests 2)
add_test(NAME RunOutputsCorrectEquations COMMAND unit_tests 3)
```

When using Gtest, `test/CMakeLists.txt` looks like:

```cmake
# chapter08/04-gtest/test/CMakeLists.txt
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

add_executable(unit_tests
    calc_test.cpp
    run_test.cpp)
target_link_libraries(unit_tests PRIVATE sut gtest_main)

include(GoogleTest)
gtest_discover_tests(unit_tests)
```

In the first part of `test/CMakeLists.txt` for GTest, CMake should fetch GTest:

```cmake
# chapter08/04-gtest/test/CMakeLists.txt - the first part
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)
```

Let's compare the middle part of `test/CMakeLists.txt`:


```cmake
# chapter08/02-structured/test/CMakeLists.txt - the first part
add_executable(unit_tests
    unit_tests.cpp
    calc_test.cpp
    run_test.cpp)
target_link_libraries(unit_tests PRIVATE sut)
```

Without a testing framework, you need to provide an entry point (`main()` in `unit_tests.cpp`). On the other hand, GTest provides with a general entry point in the library `gtest_main`.

```cmake
# chapter08/04-gtest/test/CMakeLists.txt - the middle part
add_executable(unit_tests
    calc_test.cpp
    run_test.cpp)
target_link_libraries(unit_tests PRIVATE sut gtest_main)
```

Let's compare the last part of `test/CMakeLists.txt`:

```cmake
# chapter08/02-structured/test/CMakeLists.txt - the last part
add_test(NAME SumAddsTwoInts COMMAND unit_tests 1)
add_test(NAME MultiplyMultipliesTwoInts COMMAND unit_tests 2)
add_test(NAME RunOutputsCorrectEquations COMMAND unit_tests 3)
```

Without a testing framework, you need to add tests explicitly. On the other hand, GTest provides with the `gtest_discover_tests` command to register tests defined in the target `unit_tests`.

```cmake
# chapter08/04-gtest/test/CMakeLists.txt - the last part
include(GoogleTest)
gtest_discover_tests(unit_tests)
```

### Test codes

```c++
// chapter08/02-structured/test/calc_test.cpp
#include "calc.h"
#include <cstdlib>

void SumAddsTwoIntegers() {
    Calc sut;
    if (4 != sut.Sum(2, 2))
        std::exit(1);
}

void MultiplyMultipliesTwoIntegers() {
    Calc sut;
    if(3 != sut.Multiply(1, 3))
        std::exit(1);
}
```

```c++
// chapter08/04-gtest/test/calc_test.cpp
#include <gtest/gtest.h>
#include "calc.h"

class CalcTestSuite : public ::testing::Test {
protected:
    Calc sut_;
};

TEST_F(CalcTestSuite, SumAddsTwoInts) {
    EXPECT_EQ(4, sut_.Sum(2, 2));
}

TEST_F(CalcTestSuite, MultiplyMultipliesTwoInts) {
    EXPECT_EQ(12, sut_.Multiply(3, 4));
}
```

```c++
#include <string>
#include <iostream>
#include <sstream>

using namespace std;
int run(); // declaration
void RunOutputsCorrectEquations() {
    string expected {"2 + 2 = 4\n3 * 3 = 9\n"};
    stringstream buffer;
    // redirect cout
    auto prevcoutbuf = cout.rdbuf(buffer.rdbuf());

    run();

    auto output = buffer.str();
    // restore original buffer
    cout.rdbuf(prevcoutbuf);
    if (expected != output)
        exit(1);
}
```

```c++
#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include <sstream>
#include "calc.h"

using namespace std;
int run(); // declaration
TEST(RunTest, RunOutputsCorrectEquations) {
    string expected {"2 + 2 = 4\n3 * 3 = 9\n"};
    stringstream buffer;
    // redirect cout
    auto prevcoutbuf = cout.rdbuf(buffer.rdbuf());

    run();

    auto output = buffer.str();
    // restore original buffer
    cout.rdbuf(prevcoutbuf);
    EXPECT_EQ(expected, output);
}
```

Without the testing framework, we used `if` and `exit` statements
in your function such as `RunOutputsCorrectEquations()`, which should be added explicitly in the `test/CMakeList.txt`.

With Gtest, we use the `EXPECT_EQ` macro to compare the actual and expect values. The `TEST_F` or `TEST` macro defines the name of the testcase `RunOutputsCorrectEquations`, which will be registered by the `gtest_discover_tests` command specified in the `test/CMakeList.txt`.

```bash
$ cmake -S . -B build
$ cmake --build build
$ cd build
$ ctest
Test project /home/toshiro/repo/build/cmake/ch08/04-gtest/build
    Start 1: CalcTestSuite.SumAddsTwoInts
1/3 Test #1: CalcTestSuite.SumAddsTwoInts ..............   Passed    0.01 sec
    Start 2: CalcTestSuite.MultiplyMultipliesTwoInts
2/3 Test #2: CalcTestSuite.MultiplyMultipliesTwoInts ...***Failed    0.01 sec
    Start 3: RunTest.RunOutputsCorrectEquations
3/3 Test #3: RunTest.RunOutputsCorrectEquations ........   Passed    0.01 sec

67% tests passed, 1 tests failed out of 3

Total Test time (real) =   0.05 sec

The following tests FAILED:
          2 - CalcTestSuite.MultiplyMultipliesTwoInts (Failed)
Errors while running CTest
Output from these tests are in: /home/toshiro/repo/build/cmake/ch08/04-gtest/build/Testing/Temporary/LastTest.log
Use "--rerun-failed --output-on-failure" to re-run the failed cases verbosely.
```

Or run the executable `unit_tests`:

```bash
$ cd build
$ ./test/unit_tests
Running main() from /home/toshiro/repo/build/cmake/ch08/04-gtest/build/_deps/googletest-src/googletest/src/gtest_main.cc
[==========] Running 3 tests from 2 test suites.
[----------] Global test environment set-up.
[----------] 2 tests from CalcTestSuite
[ RUN      ] CalcTestSuite.SumAddsTwoInts
[       OK ] CalcTestSuite.SumAddsTwoInts (0 ms)
[ RUN      ] CalcTestSuite.MultiplyMultipliesTwoInts
/home/toshiro/repo/build/cmake/ch08/04-gtest/test/calc_test.cpp:14: Failure
Expected equality of these values:
  12
  sut_.Multiply(3, 4)
    Which is: 9
[  FAILED  ] CalcTestSuite.MultiplyMultipliesTwoInts (0 ms)
[----------] 2 tests from CalcTestSuite (0 ms total)

[----------] 1 test from RunTest
[ RUN      ] RunTest.RunOutputsCorrectEquations
[       OK ] RunTest.RunOutputsCorrectEquations (0 ms)
[----------] 1 test from RunTest (0 ms total)

[----------] Global test environment tear-down
[==========] 3 tests from 2 test suites ran. (0 ms total)
[  PASSED  ] 2 tests.
[  FAILED  ] 1 test, listed below:
[  FAILED  ] CalcTestSuite.MultiplyMultipliesTwoInts

 1 FAILED TEST
```

With GTest, the log file `Testing/Temporary/LastTest.log` gives some details, which are not enough to diagnose what was wrong.

```bash
$ cd build
$ cat Testing/Temporary/LastTest.log
Start testing: Oct 15 18:54 PST
----------------------------------------------------------
1/3 Testing: CalcTestSuite.SumAddsTwoInts
1/3 Test: CalcTestSuite.SumAddsTwoInts
Command: "/home/toshiro/repo/build/cmake/ch08/04-gtest/build/test/unit_tests" "--gtest_filter=CalcTestSuite.SumAddsTwoInts" "--gtest_also_run_disabled_tests"
Directory: /home/toshiro/repo/build/cmake/ch08/04-gtest/build/test
"CalcTestSuite.SumAddsTwoInts" start time: Oct 15 18:54 PST
Output:
----------------------------------------------------------
Running main() from /home/toshiro/repo/build/cmake/ch08/04-gtest/build/_deps/googletest-src/googletest/src/gtest_main.cc
Note: Google Test filter = CalcTestSuite.SumAddsTwoInts
[==========] Running 1 test from 1 test suite.
[----------] Global test environment set-up.
[----------] 1 test from CalcTestSuite
[ RUN      ] CalcTestSuite.SumAddsTwoInts
[       OK ] CalcTestSuite.SumAddsTwoInts (0 ms)
[----------] 1 test from CalcTestSuite (0 ms total)

[----------] Global test environment tear-down
[==========] 1 test from 1 test suite ran. (0 ms total)
[  PASSED  ] 1 test.
<end of output>
Test time =   0.01 sec
----------------------------------------------------------
Test Passed.
"CalcTestSuite.SumAddsTwoInts" end time: Oct 15 18:54 PST
"CalcTestSuite.SumAddsTwoInts" time elapsed: 00:00:00
----------------------------------------------------------

2/3 Testing: CalcTestSuite.MultiplyMultipliesTwoInts
2/3 Test: CalcTestSuite.MultiplyMultipliesTwoInts
Command: "/home/toshiro/repo/build/cmake/ch08/04-gtest/build/test/unit_tests" "--gtest_filter=CalcTestSuite.MultiplyMultipliesTwoInts" "--gtest_also_run_disabled_tests"
Directory: /home/toshiro/repo/build/cmake/ch08/04-gtest/build/test
"CalcTestSuite.MultiplyMultipliesTwoInts" start time: Oct 15 18:54 PST
Output:
----------------------------------------------------------
Running main() from /home/toshiro/repo/build/cmake/ch08/04-gtest/build/_deps/googletest-src/googletest/src/gtest_main.cc
Note: Google Test filter = CalcTestSuite.MultiplyMultipliesTwoInts
[==========] Running 1 test from 1 test suite.
[----------] Global test environment set-up.
[----------] 1 test from CalcTestSuite
[ RUN      ] CalcTestSuite.MultiplyMultipliesTwoInts
/home/toshiro/repo/build/cmake/ch08/04-gtest/test/calc_test.cpp:14: Failure
Expected equality of these values:
  12
  sut_.Multiply(3, 4)
    Which is: 9
[  FAILED  ] CalcTestSuite.MultiplyMultipliesTwoInts (0 ms)
[----------] 1 test from CalcTestSuite (0 ms total)

[----------] Global test environment tear-down
[==========] 1 test from 1 test suite ran. (0 ms total)
[  PASSED  ] 0 tests.
[  FAILED  ] 1 test, listed below:
[  FAILED  ] CalcTestSuite.MultiplyMultipliesTwoInts

 1 FAILED TEST
<end of output>
Test time =   0.01 sec
----------------------------------------------------------
Test Failed.
"CalcTestSuite.MultiplyMultipliesTwoInts" end time: Oct 15 18:54 PST
"CalcTestSuite.MultiplyMultipliesTwoInts" time elapsed: 00:00:00
----------------------------------------------------------

3/3 Testing: RunTest.RunOutputsCorrectEquations
3/3 Test: RunTest.RunOutputsCorrectEquations
Command: "/home/toshiro/repo/build/cmake/ch08/04-gtest/build/test/unit_tests" "--gtest_filter=RunTest.RunOutputsCorrectEquations" "--gtest_also_run_disabled_tests"
Directory: /home/toshiro/repo/build/cmake/ch08/04-gtest/build/test
"RunTest.RunOutputsCorrectEquations" start time: Oct 15 18:54 PST
Output:
----------------------------------------------------------
Running main() from /home/toshiro/repo/build/cmake/ch08/04-gtest/build/_deps/googletest-src/googletest/src/gtest_main.cc
Note: Google Test filter = RunTest.RunOutputsCorrectEquations
[==========] Running 1 test from 1 test suite.
[----------] Global test environment set-up.
[----------] 1 test from RunTest
[ RUN      ] RunTest.RunOutputsCorrectEquations
[       OK ] RunTest.RunOutputsCorrectEquations (0 ms)
[----------] 1 test from RunTest (0 ms total)

[----------] Global test environment tear-down
[==========] 1 test from 1 test suite ran. (0 ms total)
[  PASSED  ] 1 test.
<end of output>
Test time =   0.01 sec
----------------------------------------------------------
Test Passed.
"RunTest.RunOutputsCorrectEquations" end time: Oct 15 18:54 PST
"RunTest.RunOutputsCorrectEquations" time elapsed: 00:00:00
----------------------------------------------------------

End testing: Oct 15 18:54 PST
```

## Test coverage reports
