# Range Adaptors

*[Range adaptor closure objects](https://en.cppreference.com/w/cpp/named_req/RangeAdaptorClosureObject)* are function objects that are callable via the pipe operator: if `C` is a range adaptor closure object and `R` is a range, these two expressions are equivalent:

```c++
C(R)
R | C
```

Two range adaptor closure objects can be chained by `operator|` to produce another range adaptor closure object: if `C` and `D` are range adaptor closure objects, then `C | D` produces a range adaptor closure object `E`.

Let `c` and `d` be the stored copies (with the same constness and value category as `E`), and `R` be a range object, the following expressions are equivalent:

```c++
d(c(R))
R | c | d
```
