# Containers and Iterators

## STL ideals

The *Standard Template Library* or STL is the part of the ISO C++ standard library that provides

- containers such as `vector`, `list`, and `map`, and
- generic algorithms such as `sort`, `find`, and `accumulate`

## Sequences and iterators

From the STL point of view, a collection of data is a *sequence*. An *iterator` is an object that identifies an element of a sequence.

- the `begin` iterator identifies the beginning of the sequece
- the `end` iterator identifies the end of the sequence

An STL sequence is what is usually called *half-open*; the is, the element identified by `begin` is part of the sequence, but `end` points one beyond the end of sequence. the usual mathematical notation for such sequences (ranges) is `[begin, end)`.

If `p` and `q` are iterators to elements of the same sequence:

| operation  | meaning
|------------|-------------
| `p == q`   | true if `p` and `q` point to the same element (*1)
| `p != q`   | `!(p == q)`
| `*p`       | refers to the element pointed to by `p`
| `*p = val` | writes to the element pointed to by `p`
| `val = *p` | reads from the element pointed to by `p`
| `++p`      | makes `p` refer to the next element (*1)
|            | (*1): or one beyond the last element (i.e., `end`)


