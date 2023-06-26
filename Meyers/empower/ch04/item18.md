# Item 18: Use `std::unique_ptr` for exclusive-ownership resource management.

```c++
class Investment
{
public:
  virtual ~Investment();
};

template<typename... Ts>
auto makeInvestment(Ts&&... params)
{
  auto delInvestment = [](Investment* pInvestment)
  {
    makeLogEntry(pInvestment);
    delete pInvestment;
  }

  std::unique_ptr<Investment, decltype(delInvestment)>
  pInv(nullptr, delInvestment);

  if (...)
    pInv.reset(new Stock(std::forward<Ts>(params)...));
  else if (...)
    pInv.reset(new Bond(std::forward<Ts>(params)...));

  return pInv;
}
```

Attempting to assign a raw pointer (e.g., from `new`) to a `std::unique_ptr` won’t compile, because it would constitute an implicit conversion from a raw to a smart pointer. Such implicit conversions can be problematic, so C++11’s smart pointers prohibit them. That’s why `reset` is used to have `pInv` assume ownership of the object created via `new`.

`std::uniq_ptr` comes in two forms:

- `std::unique_ptr<T>`: no indexing operator (`[]`)
- `std::unique_ptr<T[]>`: no dereferencing operators (`*`, `->`)

Only when a `std::unique_ptr<T[]>` would make sense would be when you’re using a C-like API that returns a raw pointer to a heap array that you assume ownership of. `std::array`, `std::vector`, and `std::string` are virtually always better data structure choices than raw arrays.

Factory functions can’t know whether callers will want to use exclusive-ownership semantics for the object they return or whether shared ownership (i.e., `std::shared_ptr`) would be more appropriate. By returning a std::unique_ptr, factories provide callers with the most efficient smart pointer, but they don’t hinder callers from replacing it with its more flexible sibling.

It is easily and efficiently converts to `std::shared_ptr`:

```c++
std::shared_ptr<Investment> sp = makeInvestment(args);
```
