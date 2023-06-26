# 18 Concurrency

## 18.5 Communicating Tasks

### 18.5.1 `future` and `promise`

If we have a `future<X>` called `fx`, we can `get()` a value of type `X` from it:

```c++
void g(std::future<X>& fx)
{
    try {
        X value = fx.get();
        // use value ...
    } catch (...) {
        // handle error ...
    }
}
```

If the value isn't there yet, our thread is blocked until it arrives.

The main purpose of a `promise` is to provide simple "put" operations (called `set_value()` and `set_exception()`) to match `future`'s `get()`.

```c++
#include <exception>

void f(promise<X>& px)
{
    try {
        X res = compute_a_value_for_res();
        px.set_value(res);
    } catch (...) {
        px.set_exception(std::current_exception());
    }
}
```

### 18.5.3 `async()`

To launch tasks to potentially run asynchronously,

```c++
using it = std::vector<double>::iterator;
double accum(it begin, it end, double init);

double comp4(std::vector<double>& v)
{
    auto beg = v.begin();
    auto sz = v.size() / 4;
    auto p1 = beg + sz;
    auto p2 = beg + 2 * sz;
    auto p3 = beg + 3 * sz;

    auto f0 = std::async(accum, beg, p1, 0.0);
    auto f1 = std::async(accum, p1, p2, 0.0);
    auto f2 = std::async(accum, p2, p3, 0.0);
    auto f3 = std::async(accum, p3, v.end(), 0.0);

    return f0.get() + f1.get() + f2.get() + f3.get();
}
```

Using `async()`, you don’t have to think about threads and locks. Instead, you think in terms of tasks that potentially compute their results asynchronously.

There is an obvious limitation: don’t even think of using `async()` for tasks that share resources needing locking.

Please note that `async()` is not just a mechanism specialized for parallel computation for increased performance. For example, it can also be used to spawn a task for getting information from a user, leaving the "main program" active with something else.

### 18.5.4 Stopping a `thread`

The standard library provides a mechanism for politely requesting a thread to clean up and go away: a `stop_token`.

```c++

using namespace std::chrono_literals;
constexpr auto INTERVAL = 200ms;
constexpr int CYCLE = 5;

int counting(std::stop_token token, int value)
{
    while (!token.stop_requested()) {
        value++;
        std::this_thread::sleep_for(INTERVAL);
    }
    return value;
}
```

Here, `!token.stop_requested()` tests whether some other `thread` has requested this `thread` to terminate.

```c++
int stop_counting()
{
    std::stop_source ss {};
    auto count = std::async(counting, ss.get_token(), 0);

    std::this_thread::sleep_for(INTERVAL * CYCLE);
    ss.request_stop();

    return count.get();
}
```

The `stop_sources` produces the `stop_tokens` through which requests to stop are communicated to `thread`s.
