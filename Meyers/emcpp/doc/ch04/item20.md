# Item 20: Use `std::weak_ptr` for `std::shared_ptr`-like pointers that can dangle.

`std::weak_ptr`s are typically created from `std::shared_ptr`s.

```c++
auto spw = std::make_shared<Widget>();
std::weak_ptr<Widget> wpw(spw);
spw = nullptr;
if (wpw.expired()) ...
```

Weak pointers that dnagle are ssid to be have *expired*.

One form is `std::weak_ptr::lock`, which returns a `std::shared_ptr`. The `std::shared_ptr` is `null` if the weak pointer has expired:

```c++
std::shared_ptr<Widget> spw1 = wpw.lock();
```

The other form is the `std::shared_ptr` constructor taking a weak pointer as an argument. In this case, if the weak pointer has expired, an exception is thrown:

```c++
std::shared_ptr<Widget> spw3(wpw);
```

But you’re probably still wondering about how `std::weak_ptr`s can be useful.

## Caching factory function

Callers should certainly receive smart pointers to cached objects, and callers should certainly determine the lifetime of those objects, but the cache needs a pointer to the objects, too.

The cache’s pointers need to be able to detect when they dangle, because when factory clients are finished using an object returned by the factory, that object will be destroyed, and the corresponding cache entry will dangle.

The cached pointers should therefore be `std::weak_ptr`s.

## The Observer design pattern

In most implementations, each subject contains a data member holding pointers to its observers. That makes it easy for subjects to issue state change notifications.

Subjects have no interest in controlling the lifetime of their observers (i.e., when they’re destroyed), but they have a great interest in making sure that if an observer gets destroyed, subjects don’t try to subsequently access it.

A reasonable design is for each subject to hold a container of `std::weak_ptr`s to its observers, thus making it possible for the subject to determine whether a pointer dangles before using it.

## Doubly linked shared-ownership

```
+-------+  std::shared_ptr  +-------+  std::shared_ptr  +-------+
|      @|------------------>|       |<------------------|@      |
|   A   |                   |   B   |                   |   C   |
|       |<------------------|?     ?|------------------>|       |
+-------+       ????        +-------+       ????        +-------+
```

Suppose it’d be useful to also have a pointer from `B` back to `A`. What kind of pointer should this be? There are three choices:

- A raw pointer: If `A` is destroyed, but `C` continues to point to `B`, `B` will contain a pointer to `A` that will dangle.
- A shared pointer: `std::shared_ptr` cycle (`A` to `B` and `B` to `A`) will prevent both `A` and `B` from being destroyed.
- A weak pointer: It is clearly the best of these choices.

A weak pointer of `B` avoids both problems above.

- If `A` is destroyed, `B`’s pointer back to it will dangle, but `B` will be able to detect that.
- Furthermore, though `A` and `B` will point to one another, `B`’s pointer won’t affect `A`’s reference count, hence can’t keep `A` from being destroyed when `std::shared_ptr`s no longer point to it.

`std::weak_ptr`s don’t participate in the *shared ownership* of objects and hence don’t *affect the pointed-to object’s reference count*. There’s actually a second reference count in the control block, and it’s this second reference count that `std::weak_ptr`s manipulate.
