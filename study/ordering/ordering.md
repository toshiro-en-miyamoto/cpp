# Weak Ordering

Let `(ranked below, ranked above)` represent the ranking of any pair of elements in a set. Then the following relation is an example of the *weak ordering* on a set `{a, b, c, d}`:

- `(a, d)`
- `(a, b)`
- `(a, c)`
- `(b, d)`
- `(c, d)`

When we use `<` to denote the ranking, meaning `ranked below < ranked above`, the ranking shown in the above example can be written as follows:

- `a < d`
- `a < b`
- `a < c`
- `b < d`
- `c < d`

A **strict weak ordering** on a set `S` is a *strict partial order* `<` on `S` for which the *incomparability relation* induced on `S` by `<` is a transitive relation.

Two elements `x` and `y` of a set `S` are said to be **incomparable** with respect to `<` if neither `x < y` nor `y < x` is true.

Reference: [Weak ordering](https://en.wikipedia.org/wiki/Weak_ordering)


$A_{i}$, $B_{j}$, $C_{k}$
