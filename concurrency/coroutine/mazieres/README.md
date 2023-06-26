# My tutorial and take on C++ coroutines

## Coroutine handles

The `co_await` operator creates a callable object whose invocation continues execution of the current function. The callable object is of type `std::coroutine_handle<>`.

When you evaluate the expression `co_await a`, the compiler creates a coroutine handle and passes it to the method `a.await_suspend(coroutine_handle)`. The type of `a` must support certain methods, and is sometimes referred to as an **awaitable** object or an **awaiter**.

```c++
struct ReturnObject {
  struct promise_type {
    ReturnObject get_return_object() { return {}; }
    std::suspend_never initial_suspend() { return {}; }
    std::suspend_never final_suspend() noexcept { return {}; }
    void unhandled_exception() {}
  };
};

struct Awaiter {
  std::coroutine_handle<> *hp_;
  constexpr bool await_ready() const noexcept { return false; }
  void await_suspend(std::coroutine_handle<> h) { *hp_ = h; }
  constexpr void await_resume() const noexcept {}
};

ReturnObject
counter(std::coroutine_handle<> *continuation_out)
{
  Awaiter a{continuation_out};
  for (unsigned i = 0;; ++i) {
    co_await a;
    std::cout << "counter: " << i << " on thread " << std::this_thread::get_id() << std::endl;
  }
}

int main()
{
  std::coroutine_handle<> h;
  counter(&h);
  for (int i = 0; i < 3; ++i) {
    std::cout << "In main: " << i << " on thread " << std::this_thread::get_id() << std::endl;
    h();
  }
  h.destroy();
  return 0;
}
```

```bash
In main: 0 on thread 0x7ff84994e640
counter: 0 on thread 0x7ff84994e640
In main: 1 on thread 0x7ff84994e640
counter: 1 on thread 0x7ff84994e640
In main: 2 on thread 0x7ff84994e640
counter: 2 on thread 0x7ff84994e640
```

You will note that there are two other methods on Awaiter, because these are required by the language. 

- `await_ready()` is an optimization. If it returns `true`, then `co_await` does not suspend the function. Of course, you could achieve the same effect in `await_suspend()`, by resuming (or not suspending) the current coroutine, but before calling `await_suspend()`, the compiler must bundle all state into the heap object referenced by the coroutine handle, which is potentially expensive.
- `await_resume()` here returns `void`, but if instead it returned a value, this value would be the value of the `co_await` expression.
