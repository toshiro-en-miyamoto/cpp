# Test Driven Approach

Some programs in this folder were implemented by a test driven approach. This article analyzes the approach in terms of:

- lines of code
- effort

## `drill.cpp`

### Lines of code

The program has 301 lines of code, and the lines fall into four categories:

| from  | to    | lines | category        |
|------:|------:|------:|-----------------|
|   3   |  14   |  12   | requirements    |
|  16   | 173   | 158   | test cases      |
| 175   | 271   |  97   | implementation  |
| 273   | 301   |  29   | test code       |

Test cases and test code can be consider as a single category:

| category                        | lines |
|---------------------------------|------:|
| requirements                    |  12   |
| implementation                  |  97   |
| test (test cases and test code) | 187   |

As test (test cases and test code) helps better understand requirements, they can be consider as a single category:

| category                        | lines |
|---------------------------------|------:|
| requirements and test           | 199   |
| implementation                  |  97   |

The amount of lines for requirements and test (199 lines) is about twice as much as the implementation code (97 lines).

### Effort

The total effort to finish the `drill.cpp` code was 10 hours, and the effort can be broken down as follows:

| actual effort                         | hours |
|---------------------------------------|------:|
| technical challenges for test code    | 4     |
| writing test cases                    | 3     |
| writing test code                     | 1     |
| writing and testing solution code     | 2     |

It took four hours to solve technical challenges due to lack of knowledge about:

- `std::string` vs `std::string_view`
- `std:iostream` vs `std::stringstream` and `std::stringbuf`

Should the author be familiar with the above technicalities, the four-hour would not be required at all.

| ideal effort with TDA                 | hours |
|---------------------------------------|------:|
| writing test cases                    | 3     |
| writing test code                     | 1     |
| writing and testing solution code     | 2     |

What if the author had not taken a test driven approach? Based on the author's experience with the approach for the `drill.cpp` program, the author believes:

| assumed effort without TDA                            | hours |
|-------------------------------------------------------|------:|
| initial solution code                                 | 2     |
| initial test cases                                    | 1     |
| manual testing, improving test cases and fixing code  | 3     |

The author assume:

- The total amount of effort with TDA will be almost the same as the one without TDA.
- The initial code is of rushed quality, hence the code should be subject to correction and refactoring.
- The initial test cases might not be enough due to insufficient understanding of requirements, hence the test cases might be subject to improvement.
- With manual testing, a single run of a test case will definitely take more minutes than automated testing. Repetitions of a series of actions (manual testing, fixing code, manual testing for confirming the fix) can undoubtedly make programmers frustrated, leading lower productivity and lower moral.

If programmers' productivity and moral get lower, some of them may be tempted by an easier option &mdash; testing only happy paths. Yoo must not choose the option, of course.

As TDA enforces programmers to consider carefully whether test cases are sufficient, the author believes that the test driven approach has advantages:

- Preparing sufficient test cases helps programmers better understand requirements.
- Automated testing with sufficient test cases gives programmers good opportunities to refactor their code safely and efficiently.
- As a result, when they finish all test cases, they will feel confident in code quality.

In summary, TDA enables you write better code with almost the same effort.

## `ex04.cpp`

### Lines of code

The program has 487 lines of code, and the lines fall into four categories:

| from  | to    | lines | category        |
|------:|------:|------:|-----------------|
|   4   |   7   |   4   | requirements    |
|   9   | 165   | 157   | test cases      |
| 167   | 373   | 180   | code design     |
| 167   | 373   |  18   | implementation  |
| 375   | 435   |  61   | implementation  |
| 437   | 487   |  29   | test code       |

Code design amounts to about 180 comment lines (out of 207, from #167 to #373) in the source code.

| category                    | lines | ratio   |
|-----------------------------|------:|--------:|
| req & test                  | 190   |   42.3% |
| design, coding and testing  | 259   |   57.7% |
| total                       | 449   |  100.0% |

### Effort

| category                    | min | hours | ratio   |
|-----------------------------|----:|------:|--------:|
| req & test                  |  90 | 1.5   |   37.5% |
| design, coding and testing  | 150 | 2.5   |   62.5% |
| total                       | 240 | 4.0   |  100.0% |

## `ex06.cpp`

### Lines of code

The program has 137 lines of code, and the lines fall into four categories:

| from  | to    | lines | category        |
|------:|------:|------:|-----------------|
|   3   |   7   |   5   | requirements    |
|   9   |  14   |   6   | macro design    |
|  16   | 112   |  97   | test cases      |
| 114   | 125   |  12   | implementation  |
| 127   | 137   |  11   | test code       |

| category                    | lines | ratio   |
|-----------------------------|------:|--------:|
| req & test                  | 113   |   86.3% |
| design, coding and testing  |  18   |   13.7% |
| total                       | 131   |  100.0% |

### Effort

| category                    | min | hours | ratio   |
|-----------------------------|----:|------:|--------:|
| req & test                  |  60 | 1.0   |   50.0% |
| design, coding and testing  |  60 | 1.0   |   50.0% |
| total                       | 120 | 2.0   |  100.0% |

