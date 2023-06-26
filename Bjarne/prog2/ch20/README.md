# Containers and Iterators

## STL ideals

The *Standard Template Library* or STL is the part of the ISO C++ standard library that provides

- containers such as `vector`, `list`, and `map`, and
- generic algorithms such as `sort`, `find`, and `accumulate`

## Sequences and iterators

From the STL point of view, a collection of data is a *sequence*. An *iterator` is an object that identifies an element of a sequence.

- the `begin` iterator identifies the beginning of the sequence
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

## Generalizing `vector` yet again

```c++
template<typename T>
class vector
{
public:
  using size_type = unsigned long;
  using value_type = T;
  using iterator = T*;
  using const_iterator = const T*;

  iterator begin();
  const_iterator begin() const;
  iterator end();
  const_iterator end() const;

  size_type size();
};
```

All the standard library algorithms are defined in terms of these member type names, such as `iterator` and `size_type`, so that they don’t unnecessarily depend on the implementations of containers or exactly which kind of container they operate on.

```c++
// using iterator = T*;
// using const_iterator = T*;
vector<int>::iterator p = find(v.begin(), v.end(), 32);
// using size_type = unsigned long
for (vector<int>::size_type i = 0; i < v.size(); ++i) print(v[i]);
```

A `using` declaration is a C++11 notation for and a generalization of what was known in C and C++ as a `typedef`.

### Container traversal

We can traverse a standard library `vector` and `list` using the simpler range-`for`-loop:

```c++
void print2(const std::vector<double>& v)
{
  for (double x : v)
    std::cout << x << '\n';
}
```

The trick is that the range-`for`-loop is defined in terms of `begin()` and `end()` functions returning iterators to the first and one beyond the end of our `vector` elements. The range-`for`-loop is simply syntactic sugar for a loop over a sequence using iterators.

### `auto`

When we have to write out loops over a generic structure, naming the iterators can be a real nuisance.

```c++
template<typename T>
void user(std::vector<T>& v, std::list<T>& lst)
{
  for (std::vector<T>::iterator p = v.begin; p != end(); ++p)
    std::cout << *p << '\n';

  list<T>::iterator q = find(lst.begin(), lst.end(), T{42});
}
```

Fortunately, we don’t have to: we can declare a variable `auto`, meaning use the type of the `iterator` as the type of the variable:

```c++
template<typename T>
void user(std::vector<T>& v, std::list<T>& lst)
{
  for (auto p = v.begin; p != end(); ++p)
    std::cout << *p << '\n';

  auto q = find(lst.begin(), lst.end(), T{42});
}
```

One common use of `auto` is to specify the loop variable in a range-`for`-loop.

```c++
template<typename C>    // requires Container<T>
void print3(const C& cont)
{
  for (const auto& x : cont)
    std::cout << x << '\n';
}
```

Here, we use

- `auto` because it is not all that easy to name the element type of the container `cont`, 
- `const` because we are not writing to the container elements, and 
- `&` (for reference) in case the elements are so large that copying them would be costly.

## Adapting built-in arrays to STL

```c++
template <typename T, int N>
class array
{
  T elements[N];
  // no explicit construct/copy/destroy needed
public:
  using value_type = T;
  using iterator = T*;
  using const_iterator = const T*;
  using size_type = unsigned long;

  iterator begin() { return elements; }
  const_iterator begin() const { return elements; }
  iterator end() { return elements + N; }
  const_iterator end() const { return elements + N; }

  size_type size() const { return N; }

  T& operator[](size_type n) { return elements[n]; }
  const T& operator[](size_type n) const { return elements[n]; }

  T* data() { return elements; }
  const T* data() const { return elements; }
};
```

## Iterator categories

Five categories:

- input iterator (IN)
- output iterator (OUT)
- forward iterator (FWD)
- bidirectional iterator (BID)
- random-access iterator (RAND)

| ops   | operation | IN | OUT | FWD  | BID  | RAND |
|:-----:|-----------|:--:|:---:|:----:|:----:|:----:|
| `*`   | read      | v  |     | v    | v    | v    |
| `*`   | write     |    | v   | v    | v    | v    |
| `++`  | forward   | v  | v   | v    | v    | v    |
| `--`  | backward  |    |     |      | v    | v    |
| `+ n` | forward   |    |     |      |      | v    |
| `- n` | backward  |    |     |      |      | v    |
