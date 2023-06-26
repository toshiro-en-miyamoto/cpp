# Item 21: Prefer `std::make_unique` and `std::make_shared` to direct use of `new`.

```c++
auto upw1(std::make_unique<Widget>());
std::unique_ptr<Widget> upw2(new Widget);

auto spw1(std::make_shared<Widget>());
std::shared_ptr<Widget> spw2(new Widget);
```

The essential difference:

- the versions using `new` repeat the type being created, but
- the `make` functions don’t.

Compared to direct use of `new`, `make` functions

- eliminate source code duplication,
- improve exception safety, and,
- for `std::make_shared` and `std::allocate_shared`, generate code that’s smaller and faster.

Situations where use of make functions is inappropriate include

- the need to specify custom `delete`rs and
- a desire to pass braced initializers.

For `std::shared_ptr`s, additional situations where `make` functions may be ill-advised include

- classes with custom memory management and
- systems with memory concerns, very large objects, and `std::weak_ptr`s that outlive the corresponding `std::shared_ptr`s.
